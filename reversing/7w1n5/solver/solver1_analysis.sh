#!/bin/bash

# 参考：よく使うstraceのオプション
# https://qiita.com/yoheia/items/22f7a85ac1b426e34d4e

strace -ff -s 1500000 -o log.txt ./Brother1
cat log.txt* |grep SECCON
