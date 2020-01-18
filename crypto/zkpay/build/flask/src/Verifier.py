import os
import sys
import subprocess
import sqlite3

from utils import bytes2longs, packHash

class Verifier:
    def __init__(self, VERIFIER_PATH, VK_PATH):
        self.VERIFIER_PATH = VERIFIER_PATH
        self.VK_PATH       = VK_PATH

    def verify(self, username, proof, hashstr):
        packedHash = packHash(hashstr)
        cmd  = [self.VERIFIER_PATH] + [self.VK_PATH] + list(map(str, packedHash)) + bytes2longs(username.encode()) + [proof]
        proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT);
        stdout, stderr = proc.communicate()
        lines = "".join(stdout.decode("utf-8"))
        lines = lines.split("\n")
        for line in lines:
            if "success" in line:
                return True
        raise Exception("Verification failed.")
