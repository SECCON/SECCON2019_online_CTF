import os
from pathlib import Path
import sqlite3

from HTMLLogger import HTMLLogger
from keygen   import generateKeys

LOG_PATH    = "/usr/src/flask/static/log.html"
SNARK_DIR   = "/usr/src/flask/snark/build/src/"
KEYGEN_PATH = os.path.join(SNARK_DIR, "keygen")
KEY_DIR     ="/usr/src/flask/keys/"
PK_NAME     ="pk_data"
VK_NAME     ="vk_data"
PK_PATH     = os.path.join(KEY_DIR, PK_NAME)
VK_PATH     = os.path.join(KEY_DIR, VK_NAME)
DB_PATH     = "zkpay.db" 

def main():
    if not Path(LOG_PATH).exists():
        logger = HTMLLogger(name="app", html_filename=LOG_PATH)
        logger.debug("initialisation start")
        generateKeys(KEYGEN_PATH, PK_PATH, VK_PATH)
        logger.debug("key generated")
        with sqlite3.connect(DB_PATH) as con:
            cur = con.cursor()
            cur.execute("CREATE TABLE IF NOT EXISTS user (username TEXT PRIMARY KEY, hash TEXT, balance INTEGER, proof TEXT)")
            cur.execute("CREATE TABLE IF NOT EXISTS trx (id INTEGER PRIMARY KEY, time INTEGER, sender TEXT, senderHash TEXT, receiver TEXT, amount INTEGER)")
        logger.debug("database created")

if __name__ == "__main__":
    main()
