import re
import base64
from datetime import datetime
import threading
import sqlite3

from flask import Flask, render_template, request, session, redirect, Response, jsonify, send_from_directory

from const import *

import QR
from AccountManager import AccountManager
from Prover   import Prover
from Verifier import Verifier
from utils     import str2int

accountManager = AccountManager(DB_PATH, ADMIN_HASH)
prover         = Prover(PROVER_PATH, PK_PATH, DB_PATH)
verifier       = Verifier(VERIFIER_PATH, VK_PATH)

app = Flask(__name__)
app.config["SECRET_KEY"] = SECRET_KEY
app.config['MAX_CONTENT_LENGTH'] = 1 * 1024 * 1024

@app.route("/")
def index():
    username = session.get("username")
    if not username == None:
        return redirect("/home", code=302)
    return render_template("index.html")

@app.route("/home")
def home():
    username = session.get("username")
    if username == None:
        return redirect("/", code=302)
    return render_template("home.html")

# ---

@app.route("/api/signup", methods = ["POST"])
def signup():
    content  = request.json
    (username, password) = (content["user"], content["pass"])
    try:
        if username == "admin":
            raise Exception("Error: reserved")
        if re.match(r"^[A-Za-z0-9]+$", username) == None or re.match(r"^[A-Za-z0-9]+$", password) == None:
            raise Exception("username, password: ^[A-Za-z0-9]+$")
        accountManager.signup(username, password)
    except Exception as e:
        return jsonify({"status": "ng", "error": str(e)})
    session["username"] = username
    thread = threading.Thread(target=prover.prove, args=(username, password))
    thread.start()
    return jsonify({"status":"ok"})

@app.route("/signin", methods = ["POST"])
def signin():
    username = request.form["user"]
    password = request.form["pass"]
    try:
        accountManager.signin(username, password)
        session["username"] = username
        return redirect("/home", code=302)
    except:
        return redirect("/", code=302)

@app.route("/pk")
def pk():
    return send_from_directory(directory=KEY_DIR, filename=PK_NAME)

@app.route("/api/accountData")
def accountData():
    username = session.get("username")
    if username == None:
        return redirect("/", code=302)
    balance = accountManager.getBalance(username)
    flag = DUMMY_FLAG
    if balance > 1000000:
        flag = FLAG
    trxList = accountManager.getTransactions(username)
    return jsonify({"balance": balance, "flag": flag, "trxList": trxList})

@app.route("/api/createQR", methods = ["GET", "POST"])
def createQR():
    username = session.get("username")
    if username == None:
        return redirect("/", code=302)
    content  = request.json
    amount   = str2int(content["amount"])
    ret = {}
    try:
        dbData = accountManager.checkBalance(username, amount)
        (proof, hashstr) = (dbData[3], dbData[1])
        ret["status"] = "ok"
        ret["dom"] = QR.create(username, amount, proof, hashstr)
    except Exception as e:
        ret["status"] = "ng"
        ret["error"] = "<p>%s</p>" % str(e)
    return jsonify(ret)

@app.route("/api/readQR", methods = ["POST"])
def readQR():
    receiverName = session.get("username")
    if receiverName == None:
        return redirect("/", code=302)
    ret = {}
    try:
        if 'file' not in request.files:
            Exception('No file Uploaded.')
        file = request.files['file']
        if file.filename == '':
            Exception('No file Uploaded.')
    except Exception as e:
        ret["status"] = "ng"
        ret["error"] = "<p>%s</p>" % str(e)
        return jsonify(ret)
    rawImg = file.read()
    (senderName, amount, proof, hashstr) = QR.read(rawImg)
    ret = {}
    try:
        accountManager.checkAccount(senderName, hashstr)
        verifier.verify(senderName, proof, hashstr)
        accountManager.checkBalance(senderName, amount)
        balance = accountManager.updateBalance(senderName, hashstr, receiverName, amount)
        ret["status"] = "ok"
        ret["dom"] = "You received $%s from %s. $%s is in your account now." % (amount, senderName, balance)
    except Exception as e:
        ret["status"] = "ng"
        ret["error"] = "%s" % str(e)
    return jsonify(ret)

if __name__ == "__main__":
    app.run(debug=False, host="0.0.0.0", port=8000)
