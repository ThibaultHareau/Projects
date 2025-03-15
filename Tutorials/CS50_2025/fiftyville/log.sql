-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Filter on date and location of the crime to get teh description
SELECT * FROM crime_scene_reports WHERE year = 2024 AND month = 7 AND day = 28 AND street = "Humphrey Street";
-- The crime took place at 10:15am at the Bakery.

-- Get the interviews from that day as well
SELECT * FROM interviews WHERE year = 2024 AND month = 7 AND day = 28;
-- Ruth: "Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away."
-- Eugene: "I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money."
-- Raymond: "As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket."

-- KEY POINTS:
--  The thief's car was in the parking lot (check parking lot)
--  The thief was at the ATM on Leggett Street earlier (check this ATM on July 28th before 10:15am)
--  They called someone for less than a minute, purchase flight ticket (check flight ticket purchase right after or at 10:15am)

-- Use all the data to retrieve the thief
SELECT * FROM people WHERE id in
    (SELECT person_id FROM bank_accounts WHERE account_number IN
        (SELECT account_number FROM atm_transactions WHERE atm_location = "Leggett Street" AND transaction_type = "withdraw" AND year = 2024 AND month = 7 AND day = 28))
AND license_plate IN
    (SELECT license_plate FROM bakery_security_logs WHERE year = 2024 AND month = 7 AND day = 28 AND hour = 10 AND minute >=15 AND minute < 30 AND activity = "exit")
AND passport_number IN
    (SELECT passport_number FROM passengers WHERE flight_id IN
        (SELECT id FROM flights WHERE year = 2024 AND month = 7 AND day = 29 AND origin_airport_id IN
            (SELECT id FROM airports WHERE city = "Fiftyville")
        ORDER BY month, day, hour, minute
        LIMIT 1))
AND phone_number IN
    (SELECT caller FROM phone_calls WHERE month = 7 AND day = 28 AND duration <= 60);

-- Get the accomplice
SELECT * FROM people WHERE phone_number IN
    (SELECT receiver FROM phone_calls WHERE month = 7 AND day = 28 AND duration <= 60 AND caller = "(367) 555-5533");

-- get the location
SELECT * FROM airports WHERE id IN
    (SELECT destination_airport_id FROM flights WHERE year = 2024 AND month = 7 AND day = 29 AND origin_airport_id IN
        (SELECT id FROM airports WHERE city = "Fiftyville")
    ORDER BY month, day, hour, minute
    LIMIT 1);
