import sqlite3
from datetime import datetime

from utils import packedSHA256
from const import * 

class AccountManager:
    def __init__(self, DB_PATH, ADMIN_HASH):
        self.DB_PATH    = DB_PATH
        self.ADMIN_HASH = ADMIN_HASH

    def signup(self, username, password):
        with sqlite3.connect(self.DB_PATH) as con:
            cur = con.cursor()
            try:
                cur.execute("INSERT INTO user VALUES (?, ?, ?, ?)",
                            (username, packedSHA256(username, password), 500, "generating"))
                cur.execute("INSERT INTO trx (time, sender, senderHash, receiver, amount) VALUES (?, ?, ?, ?, ?)",
                            (int(datetime.now().timestamp()), "admin", self.ADMIN_HASH, username, 500))
            except:
                raise Exception("The username is duplicated. Use another username.")

    def signin(self, username, password):
        with sqlite3.connect(self.DB_PATH) as con:
            cur = con.cursor()
            cur.execute("SELECT * FROM user WHERE username = ? AND hash = ?",
                        (username, packedSHA256(username, password)))
            if cur.fetchone() == None:
                raise Exception("The username or password is incorrect.")

    def getBalance(self, username):
        with sqlite3.connect(self.DB_PATH) as con:
            cur = con.cursor()
            cur.execute("SELECT balance FROM user WHERE username = ?", (username, ))
            result = cur.fetchone()
        return result[0]

    def getTransactions(self, username):
        with sqlite3.connect(self.DB_PATH) as con:
            cur = con.cursor()
            cur.execute("SELECT * FROM trx WHERE receiver = ? ORDER BY id DESC LIMIT 5", (username, ))
            result = cur.fetchall()
        return result

    # ---

    def checkBalance(self, username, amount):
        if username == "admin":
            return ["admin", "", 0, "", 0, 0]
        with sqlite3.connect(self.DB_PATH) as con:
            cur = con.cursor()
            cur.execute("SELECT * FROM user WHERE username = ? AND balance >= ?",
                        (username, amount))
            result = cur.fetchone()
            if result == None:
                raise Exception("Don't cheat!")
            return result

    def updateBalance(self, senderName, senderHash, receiverName, amount):
        with sqlite3.connect(self.DB_PATH) as con:
            cur = con.cursor()
            if senderName != "admin":
                cur.execute("UPDATE user SET balance=balance-? WHERE username = ?", (amount, senderName))
            cur.execute("UPDATE user SET balance=balance+? WHERE username = ?", (amount, receiverName))
            cur.execute("INSERT INTO trx (time, sender, senderHash, receiver, amount) VALUES (?, ?, ?, ?, ?)", (int(datetime.now().timestamp()), senderName, senderHash, receiverName, amount))
            con.commit()
            cur.execute("SELECT balance FROM user WHERE username = ?", (receiverName, ))
            result = cur.fetchone()
        return result[0]

    def checkAccount(self, username, hashstr):
        if username == "admin" and hashstr == self.ADMIN_HASH:
            return
        with sqlite3.connect(self.DB_PATH) as con:
            cur = con.cursor()
            cur.execute("SELECT * FROM user WHERE username = ? AND hash = ?",
                        (username, hashstr))
            result = cur.fetchone()
            if result == None:
                raise Exception("This account does not exist.")
        return
