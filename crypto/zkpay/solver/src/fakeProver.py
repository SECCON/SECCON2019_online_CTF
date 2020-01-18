import subprocess

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



def generateProof(FAKE_PROVER_PATH, PK_PATH, username, password=""):
    cmd  = [FAKE_PROVER_PATH] + [PK_PATH] + bytes2longs(username.encode())
    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT);
    stdout, stderr = proc.communicate()

    proofOrd = [ord(c) for c in "Proof:"]
    proofIdx      = 0
    proof = ""

    for e in stdout:
        if proofIdx == len("Proof:"):
            if e == ord("\n"):
                proofIdx = -1
            proof += chr(e)
            continue
        if e == proofOrd[proofIdx]:
            proofIdx += 1
        else:
            proofIdx = 0

    return proof

