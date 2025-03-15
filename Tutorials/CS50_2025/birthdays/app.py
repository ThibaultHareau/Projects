import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # Validate input data
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")
        if not name or not month or not day:
            birthdays = db.execute("SELECT * FROM birthdays")
            return render_template("index.html", birthdays=birthdays, message="Missing input data")

        if not check_date(month, day):
            birthdays = db.execute("SELECT * FROM birthdays")
            return render_template("index.html", birthdays=birthdays, message="Wrong date")

        # If input are correct, insert into DB
        month = int(month)
        day = int(day)
        db.execute("INSERT INTO birthdays (name, month, day) VALUES(?, ?, ?)", name, month, day)

        return redirect("/")

    else:

        birthdays = db.execute("SELECT * FROM birthdays")

        return render_template("index.html", birthdays=birthdays, message="")


@app.route("/delete", methods=["POST"])
def delete():
    id = request.form.get("id")
    db.execute("DELETE FROM birthdays WHERE id = ?", id)
    return redirect("/")


def check_date(month_str, day_str):
    #Check if month and day strings are an existing date
    try:
        month = int(month_str)
    except ValueError:
        print('Please enter an integer')
        return False
    try:
        day = int(day_str)
    except ValueError:
        print('Please enter an integer')
        return False
    if month > 12 or month < 1:
        print('Wrong Month value')
        return False
    if month == 2:
        if day < 1 or day > 29:
            print("Wrong day")
            return False
        return True
    if month in [1, 3, 5, 7, 8, 10, 12]:
        if day < 1 or day > 31:
            print("Wrong day")
            return False
        return True
    if day < 1 or day > 30:
        print("Wrong day")
        return False
    return True

