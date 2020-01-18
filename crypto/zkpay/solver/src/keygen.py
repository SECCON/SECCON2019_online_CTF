import subprocess

def generateKeys(KEYGEN_PATH, PK_PATH, VK_PATH, TAU_PATH, initTime):
    cmd  = [KEYGEN_PATH] + [PK_PATH, VK_PATH, TAU_PATH, str(initTime)]
    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT);
    stdout, stderr = proc.communicate()
    return
