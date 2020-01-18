from hashlib import sha256

def bytes2longs(bs):
    def _bytes2quads(bs):
        if len(bs) > 32:
            bs = bs[:32]
        if len(bs) < 32:
            bs = (32 - len(bs)) * b"\x00" + bs
        ba  = [int(e) for e in bytearray(bs)]
        ret = [ba[i:i+8] for i in range(0, len(ba), 8)]
        return ret
    def _quads2longs(quads):
        ret = []
        for quad in quads:
            ret += [sum([quad[i] *(1<<((8-1-i)*8)) for i in range(len(quad))])]
        return list(map(str, ret))
    return _quads2longs(_bytes2quads(bs))

def packHash(sha256str):
    ret = []
    for part in [sha256str[:32], sha256str[32:]]:
        bins = format(int(part, 16), "0128b")
        ret += [int(bins[::-1], 2)]
    return ret

def packedSHA256(username, password):
    if len(username) > 32:
        username = username[:32]
    if len(password) > 32:
        password = password[:32]
    usernameBytes = b"\x00" * (32 - len(username)) + username.encode("utf-8")
    passwordBytes = b"\x00" * (32 - len(password)) + password.encode("utf-8")
    return sha256(usernameBytes+passwordBytes).hexdigest()

def str2int(string):
    return int(''.join(c for c in "0" + string if c.isdigit()))
