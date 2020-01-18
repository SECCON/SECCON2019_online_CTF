import subprocess
import sqlite3

from utils import bytes2longs

class Prover:
    def __init__(self, PROVER_PATH, PK_PATH, DB_PATH):
        self.PROVER_PATH = PROVER_PATH
        self.PK_PATH     = PK_PATH
        self.DB_PATH     = DB_PATH

    def prove(self, username, password):
        cmd  = [self.PROVER_PATH] + [self.PK_PATH] + bytes2longs(username.encode()) + bytes2longs(password.encode())
        proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT);
        stdout, stderr = proc.communicate()
        lines = "".join(stdout.decode("utf-8"))
        lines = lines.split("\n")
        for line in lines:
            if "Proof:" in line:
                proof = line.split(":")[1]
        with sqlite3.connect(self.DB_PATH) as con:
            cur = con.cursor()
            cur.execute("UPDATE user SET proof = ? WHERE username = ?", (proof, username))

