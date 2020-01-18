import io
import base64
from PIL import Image
import qrcode
import zbarlight

def create(username, amount, proof, hashstr):
    def createQRDOM(contents):
        encoded = base64.b64encode(contents)
        dom = '<img src="data:image/png;base64, %s" alt="QR" class="img-responsive center-block" />' % encoded.decode("utf-8")
        return dom
    def createQRImgOnMemory(username, amount, proof, hashstr):
        qrData = "username=%s&amount=%s&proof=%s&hash=%s" % (username, amount, proof, hashstr)
        img = qrcode.make(qrData)
        with io.BytesIO() as output:
            img.save(output, format="PNG")
            contents = output.getvalue()
        return contents
    return createQRDOM(createQRImgOnMemory(username, amount, proof, hashstr))

def read(rawImg):
    def readQRImg(rawImg):
        qrImg = Image.open(io.BytesIO(rawImg))
        codes = zbarlight.scan_codes(['qrcode'], qrImg)
        return codes
    def decode(codes):
        data = codes[0].decode("utf-8").split("&")
        decoded = {}
        for datum in data:
            splitted = datum.split("=")
            if splitted[0] in ["username", "amount", "proof", "hash"]:
                decoded[splitted[0]] = splitted[1]
        return decoded
    codes    = readQRImg(rawImg)
    decoded  = decode(codes)
    return (decoded["username"], decoded["amount"], decoded["proof"], decoded["hash"])

