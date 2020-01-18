#!/bin/bash

# 無限ループで実行中のコマンドからforkされるプロセスの情報を出力する
# タイミングがシビアだがしばらく待つとフラグが得られる

while :
do
  ps -elf|grep date|grep -v grep
done
