import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")
#Implement the personal touch of being apple to search for company name
#pk_fd0da499e77742408eed3f9f978d0601

@app.route("/", methods=["GET", "POST"])
def index():
    birthdays = db.execute("SELECT name,month,day FROM birthdays")
    if request.method == "POST":
        name = request.form.get("name")
        month = request.form.get("month")

        if(int(month) > 12 or int(month) < 1):
            return render_template("index.html",message="Month must be between 1 and 12",birthdays=birthdays)

        day = request.form.get("day")
        if(int(day) > 31 or int(day) < 1):
            return render_template("index.html",message = "Days must be between 1 and 31",birthdays=birthdays)

        db.execute("INSERT INTO birthdays (name,month,day) VALUES(?,?,?)",name,month,day)
        return redirect("/")

    else:
        return render_template("index.html", birthdays=birthdays)


