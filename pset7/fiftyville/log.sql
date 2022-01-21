-- Keep a log of any SQL queries you execute as you solve the mystery.

SELECT description FROM crime_scene_reports
    WHERE day = 28 AND year = 2020 AND month = 7
    AND street = "Chamberlin Street";


SELECT transcript FROM interviews
    WHERE day = 28 AND year = 2020 AND month = 7;


-- THE TRANSCRIPT WAS A LITTLE MESSY SO I USED THIS TO CHECK HOW MANY PEOPLE WERE INTERVIEED THAT DAY
SELECT COUNT(name) FROM interviews
   ...> WHERE day = 28 AND year = 2020 AND month = 7;


SELECT name FROM people
WHERE passport_number IN (SELECT passport_number FROM passengers
JOIN flights ON passengers.flight_id = flights.id
JOIN airports ON flights.origin_airport_id = airports.id
WHERE flights.day = 29 AND flights.month = 7 AND flights.year = 2020
AND airports.city = "Fiftyville" AND flights.hour = (SELECT MIN(flights.hour) FROM flights WHERE flights.day = 29 AND flights.month = 7 AND flights.year = 2020)
)
INTERSECT
SELECT name FROM people
WHERE license_plate IN (SELECT license_plate FROM courthouse_security_logs
WHERE day = 28 AND month = 7 AND year = 2020 AND hour = 10 AND activity = "exit"
AND minute <= 30
)
INTERSECT
SELECT name FROM people
WHERE phone_number IN (SELECT caller FROM phone_calls
                        WHERE day = 28 and month = 7 and year = 2020 and duration <= 60
)
INTERSECT
SELECT name FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE account_number IN (SELECT account_number FROM atm_transactions
                        WHERE day = 28 AND month = 7 AND year = 2020
                        AND atm_location = "Fifer Street" AND transaction_type = "withdraw"
);

SELECT city FROM airports
WHERE id IN (SELECT destination_airport_id FROM flights
            WHERE day = 29 AND month = 7 AND year = 2020 AND hour = (SELECT MIN(flights.hour) FROM flights WHERE flights.day = 29 AND flights.month = 7 AND flights.year = 2020)
);

SELECT name FROM people
WHERE phone_number IN (SELECT receiver FROM phone_calls
                        WHERE caller = (SELECT phone_number FROM people WHERE name = "Ernest")
                        AND day = 28 AND month = 7 AND year = 2020 AND duration <= 60
);