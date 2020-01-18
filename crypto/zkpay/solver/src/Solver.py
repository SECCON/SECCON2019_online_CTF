import os
import re
import time
from urllib.parse import urljoin
from datetime import datetime
from hashlib import sha256
import random, string

import requests

from PIL import Image
import qrcode

from keygen import generateKeys
from fakeProver import generateProof

KEYGEN_PATH = "/usr/src/solver/snark/build/src/keygen"
PK_PATH     = "/usr/src/solver/pk_data"
VK_PATH     = "/usr/src/solver/vk_data"
TAU_PATH    = "/usr/src/solver/tau_data"

FAKE_PROVER_PATH = "/usr/src/solver/snark/build/src/fake_prover"

QR_PATH     = "/usr/src/solver/qr.png"

BASE_URL = os.environ["URL"]

def packHash(sha256str):
    ret = []
    for part in [sha256str[:32], sha256str[32:]]:
        bins = format(int(part, 16), "0128b")
        ret += [int(bins[::-1], 2)]
    return ret

class Solver:
    def __init__(self):
        self.session  = requests.Session()

        self.username = ""
        self.password = ""
        self.generateUsernameAndPassword()

        self.adminHash = ""
        self.initTime  = 0

        self.nudge     = 0

    def generateUsernameAndPassword(self):
        self.username = "".join(random.choice(string.ascii_letters + string.digits) for i in range(16))
        self.password = "".join(random.choice(string.ascii_letters + string.digits) for i in range(16))

    def execute(self, func, name):
        print("[+] " + name)
        cnt = 0
        status = func()
        while (not status[0]) or cnt > 3:
            time.sleep(10)
            status = func()
            cnt += 1
            if cnt == 3:
                print("-> error")
                exit()
        return status

    def signup(self):
        def _signup():
            url = urljoin(BASE_URL, "/api/signup")
            res = self.session.post(url, json={"user": self.username, "pass": self.password})
            if res.status_code == 200:
                data = res.json()
                if data["status"] == "ok":
                    print("-> ok")
                    return (True, True)
                else:
                    return (True, False)
            return (False, False)
        return self.execute(_signup, "signup")

    def getAdminHash(self):
        def _getAdminHash():
            url = urljoin(BASE_URL, "/api/accountData")
            res = self.session.get(url)
            if res.status_code == 200:
                data = res.json()
                self.adminHash = data["trxList"][0][3]
                print("-> ok")
                return (True, True)
            else:
                return (True, False)
            return (False, False)
        return self.execute(_getAdminHash, "admin hash")

    def getInitTime(self):
        def _getInitTime():
            url  = urljoin(BASE_URL, "/static/log.html")
            res = self.session.get(url)
            if res.status_code == 200:
                lines = "".join(res.text).split("\n")
                for line in lines:
                    matched = re.match(r'<td width="200">(.+)</td>', line)
                    if matched != None:
                        initTimeStr = matched.groups(1)[0]
                        initTime = datetime.strptime(initTimeStr, "%Y-%m-%d %H:%M:%S,%f")
                        self.initTime = int(initTime.timestamp())
                        break
                if self.initTime != 0:
                    print("-> ok")
                    return (True, True)
                else:
                    return (True, False)
            return (False, False)
        return self.execute(_getInitTime, "init time")

    def downloadPK(self):
        def _downloadPK():
            url  = urljoin(BASE_URL, "/pk")
            res = self.session.get(url)
            if res.status_code == 200:
                open(PK_PATH, "wb").write(res.content)
                print("-> ok")
                return (True, True)
            return (False, False)
        return self.execute(_downloadPK, "download pk")

    def generateKeys(self):
        print("[+] tau")
        generateKeys(KEYGEN_PATH, PK_PATH, VK_PATH, TAU_PATH, self.initTime + self.nudge)
        print("-> ok")

    def generateProof(self):
        print("[+] fake proof")
        self.proof = generateProof(FAKE_PROVER_PATH, PK_PATH, "admin")
        print("-> ok")

    def createQRImg(self):
        print("[+] qr")
        qrData = "username=%s&amount=%s&proof=%s&hash=%s" % ("admin", 10000000000, self.proof, self.adminHash)
        img = qrcode.make(qrData)
        img.save(QR_PATH, format="PNG")
        print("-> ok")

    def uploadQR(self):
        def _uploadQR():
            url  = urljoin(BASE_URL, "/api/readQR")
            with open(QR_PATH, "rb") as fd:
                files = {"file": fd}
                res = self.session.post(url, files=files)
                if res.status_code == 200:
                    data = res.json()
                    if data["status"] == "ok":
                        print("-> ok")
                        return (True, True)
                    else:
                        return (True, False)
                return (False, False)
        return self.execute(_uploadQR, "upload")

    def checkFlag(self):
        def _checkFlag():
            url = urljoin(BASE_URL, "/api/accountData")
            res = self.session.get(url)
            if res.status_code == 200:
                data = res.json()
                print(data["flag"])
                return (True, True)
            else:
                return (False, False)
        return self.execute(_checkFlag, "flag")


if __name__ == "__main__":
    solver = Solver()
    solver.signup()
    solver.getAdminHash()
    solver.adminHash
    solver.getInitTime()
    solver.downloadPK()
    for i in range(3):
        solver.generateKeys()
        solver.generateProof()
        solver.createQRImg()
        status = solver.uploadQR()
        if status[1]:
            solver.checkFlag()
            break
        solver.nudge += 1
        print("...+%s" % solver.nudge)
