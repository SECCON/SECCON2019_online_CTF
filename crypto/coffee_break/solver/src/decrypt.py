import sys
from Crypto.Cipher import AES
import base64


def decrypt(key, text):
    s = ''
    for i in range(len(text)):
        t = (ord(text[i]) - 0x20) - (ord(key[i % len(key)]) - 0x20)
        if t < 0:
            t += (0x7e - 0x20 + 1)
        s += chr((t % (0x7e - 0x20 + 1)) + 0x20)
    return s


key1 = "SECCON"
key2 = "seccon2019"
text = sys.argv[1]

cipher = AES.new(key2 + chr(0x00) * (16 - (len(key2) % 16)), AES.MODE_ECB)
t = cipher.decrypt(base64.b64decode(text))
r1 = t[:-t[-1]].decode()
r2 = decrypt(key1, r1)
print(r2)
