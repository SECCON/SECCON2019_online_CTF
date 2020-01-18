#!/usr/bin/env python3.7
import sys
import os
import binascii
import re
import subprocess
import signal


def handler(x, y):
    sys.exit(-1)


signal.signal(signal.SIGALRM, handler)
signal.alarm(30)


def gen_filename():
    tmp = '/tmp/'
    return tmp + binascii.hexlify(os.urandom(16)).decode('utf-8')


def is_bad_str(code):
    code = code.lower()
    for s in ['!', '#', 'unsafe', 'use', 'extern', 'fs', 'io', 'cell', 'file',
            'open', 'process', 'file', 'prelude', 'env']:
        if s in code:
            return True
    return False


def is_bad(code):
    return is_bad_str(code)


place_holder = '/** code **/'
template_file = 'template.rs'
template_toml = 'Cargo.template.toml'


def main():
    print('input your source code size')
    size = int(input())
    if size > 100000:
        print('too big')
        return
    if size <= 0:
        print('invalid size')
        return

    code = sys.stdin.read(size)

    if is_bad(code):
        print('bad code')
        return

    workdir = gen_filename()
    print('workdir: ', workdir)
    cmds = [
        'mkdir -p ' + workdir + '/src',
        ' '.join(['cp', template_toml, workdir + '/Cargo.toml']),
        'cp -r ../mal /tmp',
        'cp ../libmal.a /tmp'
    ]
    for cmd in cmds:
        ret = os.system(cmd)
        if ret != 0:
            return

    with open("template.rs", "r") as f:
        template = f.read()

    main_rs = template.replace(place_holder, code)

    os.chdir(workdir)

    with open("src/main.rs", 'w') as f:
        f.write(main_rs)

    os.system('timeout --foreground -k 20s 15s cargo build')
    os.system('timeout --foreground -k 10s 5s cargo run')
    #ret = subprocess.run(['cargo', 'run'], timeout=10, capture_output=True)
    #if ret.returncode != 0:
    #    return
    #print(ret.stdout)


main()
