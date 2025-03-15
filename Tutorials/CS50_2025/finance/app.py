import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    holdings = db.execute(
        "SELECT symbol, SUM(quantity) AS quantity FROM transactions WHERE user_id = ? GROUP BY symbol", session["user_id"])
    shares = list()
    value_sum = 0
    for holding in holdings:
        try:
            symbol_data = lookup(holding["symbol"])
        except:
            continue
        value = symbol_data["price"] * holding["quantity"]
        value_sum += value
        shares.append({
            "symbol": holding["symbol"],
            "quantity": holding["quantity"],
            "price": usd(symbol_data["price"]),
            "total": usd(value)
        })
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

    return render_template("index.html", shares=shares, cash=usd(cash), total=usd(cash+value_sum))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")

    symbol = request.form.get("symbol")
    shares = request.form.get("shares")

    if not symbol or not shares:
        return apology("Empty input")
    symbol_data = lookup(symbol)
    if symbol_data is None:
        return apology("Invalid symbol")
    try:
        quantity = int(shares)
    except:
        return apology("Share must be a number")
    if quantity <= 0:
        return apology("Wrong share number, must be a positive integer")
    purchase_price = symbol_data["price"] * quantity
    user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    if purchase_price > user_cash:
        return apology("Can't afford")
    # Add order and update user cash amount
    try:
        db.execute("INSERT INTO transactions (user_id, symbol, share_price, quantity) VALUES (?, ?, ?, ?)",
                   session["user_id"], symbol_data["symbol"], symbol_data["price"], quantity)
        db.execute("UPDATE users SET cash = ? WHERE id = ?",
                   user_cash-purchase_price, session["user_id"])
    except:
        return apology("Issue during transaction")

    return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    try:
        data = db.execute(
            "SELECT * FROM transactions WHERE user_id = ? ORDER BY time", session["user_id"])
    except:
        return apology("Issue while retrieving data")
    transactions = list()
    for row in data:
        transactions.append({
            "symbol": row["symbol"],
            "quantity": row["quantity"],
            "price": usd(row["share_price"]),
            "time": row["time"]
        })
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        # Validate input data
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Invalid symbol")
        data = lookup(symbol)
        if data is None:
            return apology("Invalid symbol")
        return render_template("quoted.html", name=data["name"], price=usd(data["price"]), symbol=data["symbol"])

    if request.method == "GET":
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method != "POST":
        return render_template("register.html")

    # Validate input data
    username = request.form.get("username")
    password = request.form.get("password")
    confirmation = request.form.get("confirmation")

    # If there's an empty input
    if not username or not password:
        return apology("Missing input")

    # If password and confirmation doesn't match
    if password != confirmation:
        return apology("Password and confirmation are different")

    # Try to add user
    try:
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)",
                   username, generate_password_hash(password))
    except:
        return apology("Username already used")

    return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        quantity = request.form.get("shares")
        if not symbol:
            return apology("Missing symbol")
        if not quantity:
            return apology("Missing shares")
        quantity = int(quantity)
        try:
            available = db.execute(
                "SELECT SUM(quantity) AS quantity FROM transactions WHERE user_id = ? AND symbol = ? GROUP BY symbol", session["user_id"], symbol)[0]["quantity"]
        except:
            return apology("Error while retrieving data")
        if quantity > available:
            return apology("Missing shares")
        try:
            symbol_data = lookup(symbol)
            sell_price = symbol_data["price"] * quantity
        except:
            return apology("Unable to get symbol data")
        try:
            user_cash = db.execute("SELECT cash FROM users WHERE id = ?",
                                   session["user_id"])[0]["cash"]
            db.execute("INSERT INTO transactions (user_id, symbol, share_price, quantity) VALUES (?, ?, ?, ?)",
                       session["user_id"], symbol_data["symbol"], symbol_data["price"], -quantity)
            db.execute("UPDATE users SET cash = ? WHERE id = ?",
                       user_cash+sell_price, session["user_id"])
        except:
            return apology("Issue during transaction")

        return redirect("/")
    holdings = db.execute(
        "SELECT * FROM (SELECT symbol, SUM(quantity) AS quantity FROM transactions WHERE user_id = ? GROUP BY symbol ORDER BY symbol) WHERE quantity > 0", session["user_id"])
    symbols = [data["symbol"] for data in holdings]
    return render_template("sell.html", symbols=symbols)
