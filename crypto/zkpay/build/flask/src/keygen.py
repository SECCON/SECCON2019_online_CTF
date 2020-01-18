import subprocess

def generateKeys(KEYGEN_PATH, PK_PATH, VK_PATH):
    cmd  = [KEYGEN_PATH] + [PK_PATH, VK_PATH]
    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT);
    stdout, stderr = proc.communicate()
    return
