import os

SNARK_DIR   = os.environ["SNARK_DIR"]
PROVER_PATH = os.path.join(SNARK_DIR, "prover")
VERIFIER_PATH = os.path.join(SNARK_DIR, "verifier")

DB_PATH  = os.environ["DB_PATH"]

KEY_DIR  = os.environ["KEY_DIR"]
PK_NAME  = os.environ["PK_NAME"]
VK_NAME  = os.environ["VK_NAME"]
PK_PATH  = os.path.join(KEY_DIR, PK_NAME)
VK_PATH  = os.path.join(KEY_DIR, VK_NAME)

ADMIN_HASH = os.environ["ADMIN_HASH"]

SECRET_KEY = os.environ["SECRET_KEY"]

FLAG = os.environ["FLAG"]
DUMMY_FLAG = os.environ["DUMMY_FLAG"]
