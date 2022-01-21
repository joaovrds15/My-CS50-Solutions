import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from datetime import datetime
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash



from helpers import apology, login_required, lookup, usd, verifyDuplicateUsers

# Configure application
app = Flask(__name__)
#implement the option to the user to search for a stock using its name ajax
# Fix buy so users can Buy more shares of determined stock

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():

    registers = db.execute("SELECT * FROM stocks WHERE personID = ?",session.get("user_id"))
    totalCash = db.execute("SELECT SUM(totalCost) FROM stocks WHERE personID = ?",session.get("user_id"))
    cash = db.execute("SELECT cash FROM users WHERE id = ?",session.get("user_id"))
    cash = cash[0]["cash"]

    if not registers:
        totalCash = cash
    else:
        totalCash = totalCash[0]['SUM(totalCost)'] + cash

    return render_template("index.html",registers=registers,cash=cash,totalCash=totalCash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():

    if request.method == "POST":

        symbol = request.form.get("symbol")
        result = lookup(symbol)
        date = str(datetime.now().strftime("%Y-%m-%d %H:%M:%S"))

        try:
            shares = int(request.form.get("shares"))
        except(ValueError):
            return apology("Number must be integer",400)

        if result is not None and shares > 0:

            availableCash = db.execute("SELECT cash FROM users WHERE id = ?",session.get("user_id"))
            #db returns a dictionary inside a list
            availableCash = availableCash[0]["cash"]
            historyQuery = "INSERT INTO history(personID,symbol,shares,price,date) VALUES(?,?,?,?,?)"
            totalCost = shares * result["price"]

            if availableCash >= totalCost:

                db.execute("UPDATE users SET cash = ? WHERE id = ? ",availableCash - totalCost,session.get("user_id"))
                stockSearch = db.execute("SELECT * FROM stocks WHERE symbol = ? AND personID = ?",symbol,session.get("user_id"))

                if stockSearch:

                    query = "UPDATE stocks SET shares = ?, price = ?, totalCost = ? WHERE personID= ? AND symbol = ?"
                    totalShares = stockSearch[0]["shares"] + shares
                    db.execute(query,totalShares,result["price"],result["price"] * totalShares,session.get("user_id"),symbol)
                    db.execute(historyQuery,session.get("user_id"),symbol,shares,result["price"],date)
                    return redirect("/")

                else:

                    query = "INSERT INTO stocks(personID,companyName,symbol,price,shares,totalCost) VALUES(?,?,?,?,?,?)"
                    db.execute(query,session.get("user_id"),result["name"],symbol,result["price"],shares, shares * result["price"])
                    db.execute(historyQuery,session.get("user_id"),symbol,shares,result["price"],date)
                    return redirect("/")

            else:
                return apology("Not enough money",400)
        else:
            return apology("Symbol not found or shares < 1",400)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    registers = db.execute("SELECT * FROM history WHERE personID = ?",session.get("user_id"))
    return render_template("history.html",registers=registers)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

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

    if request.method == "GET":
        return render_template("quote.html")

    else:

        symbol = request.form.get("symbol")
        result = lookup(symbol)

        if result is not None:

            companyName = result["name"]
            price = result["price"]
            return render_template("quoted.html", company=companyName,symbol=symbol,price=price)

        return apology("symbol not found",400)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "GET":
        return render_template("register.html")

    else:
        name = request.form.get("username")
        password = generate_password_hash(request.form.get("password"))

        if not request.form.get("username") or not request.form.get("password") or not request.form.get("confirmation"):
            return apology("All fields must be filled",400)

        verification = db.execute("SELECT * from users WHERE username = ?",name)

        if verification:
            return apology("User already taken")

        if not check_password_hash(password,request.form.get("confirmation")):
            return apology("passwords differ",400)

        db.execute("INSERT INTO users(username,hash) VALUES(?,?)",name,password)
        return redirect("/login")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():

    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        historyQuery = "INSERT INTO history(personID,symbol,shares,price,date) VALUES(?,?,?,?,?)"
        sellOrder = db.execute("SELECT * FROM stocks WHERE personID = ? AND symbol = ?",session.get("user_id"),symbol)
        updatedData = lookup(symbol)

        if not sellOrder:
            return apology("Stock don't found",400)

        if shares > sellOrder[0]["shares"] or shares < 1:
            return apology("Not enought shares or shares less than 1")

        else:
            if sellOrder[0]["shares"] - shares == 0:

                amountToSum = updatedData["price"] * shares
                db.execute("DELETE FROM stocks WHERE symbol = ? AND personID = ?",symbol,session.get("user_id"))
                query = "UPDATE users SET cash = (SELECT cash) + ? WHERE id = ? "
                db.execute(query,amountToSum,session.get("user_id"))
                db.execute(historyQuery,session.get("user_id"),symbol,-shares,updatedData["price"],str(datetime.now().strftime("%Y-%m-%d %H:%M:%S")))
                return redirect("/")

            else:

                amountToSum = updatedData["price"] * shares
                finalShares = sellOrder[0]["shares"] - shares
                queryUpdateCash = "UPDATE users SET cash = (SELECT cash) + ? WHERE id = ?"
                queryUpdateTotalCost = "UPDATE stocks SET totalCost = (SELECT price) * (SELECT shares) WHERE personID = ? AND symbol = ?"
                db.execute("UPDATE stocks SET shares = ? WHERE personID = ? AND symbol = ?",finalShares,session.get("user_id"),symbol)
                db.execute(queryUpdateCash,amountToSum,session.get("user_id"))
                db.execute(queryUpdateTotalCost,session.get("user_id"),symbol)
                db.execute(historyQuery,session.get("user_id"),symbol,-shares,updatedData["price"],str(datetime.now().strftime("%Y-%m-%d %H:%M:%S")))
                return redirect("/")

    else:

        symbols = db.execute("SELECT symbol FROM stocks WHERE personID = ?",session.get("user_id"))
        return render_template("sell.html",symbols=symbols)

@app.route("/addCash",methods=["GET","POST"])
@login_required
def addCash():

    if request.method == "POST":
        amount = float(request.form.get("amount"))

        if amount < 0 or amount > 100_000:
            return apology("Amount less than 0 or exceeded maximum amount ",400)

        db.execute("UPDATE users SET cash = (SELECT cash) + ? WHERE id = ?",amount,session.get("user_id"))
        return redirect("/")

    else:
        return render_template("addCash.html")

@app.route("/changePassword",methods=["GET","POST"])
@login_required
def changePassword():

    if request.method == "POST":

        rows = db.execute("SELECT * FROM users WHERE id = ?",session.get("user_id"))

        if not check_password_hash(rows[0]["hash"],request.form.get("password")):
            return apology("Wrong password")

        if not request.form.get("newPassword") or check_password_hash(request.form.get("newPassword"),request.form.get("confirmation")):
            return apology("Passwords differs or not entered")

        db.execute("UPDATE users SET hash = ? WHERE id = ?",generate_password_hash(request.form.get("newPassword")),session.get("user_id"))
        return redirect("/logout")


    else:
        return render_template("changePassword.html")

@app.route("/unregister",methods=["GET","POST"])
@login_required
def unregister():

    if request.method == "POST":

        db.execute("DELETE FROM users WHERE id = ?",session.get("user_id"))
        db.execute("DELETE FROM stocks WHERE personID = ?",session.get("user_id"))
        db.execute("DELETE FROM history WHERE personID = ?",session.get("user_id"))
        return redirect("/login")

    else:
        return render_template("unregister.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
