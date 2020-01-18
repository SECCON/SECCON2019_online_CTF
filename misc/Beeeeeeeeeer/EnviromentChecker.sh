#!/usr/bin/env bash

bash --version|grep bash|grep -q -e " 4" -e " 5" || echo "recommend bash ver.4"

LIST="base64
clear
cut
grep
gunzip
md5sum
openssl
rev
sleep
tput
tr
whoami"

for COMMAND in $LIST
do
  which $COMMAND &> /dev/null
  echo $? | grep -q 0 || echo "need $COMMAND"
done

# openssl detail check
if openssl version|grep "^OpenSSL 1.0" >/dev/null
then
  openssl list-message-digest-commands|grep -q md5 || echo -e "Openssl is not supported md5\nThis is not working at your enviroment."
elif openssl version|grep "^OpenSSL 1.1" >/dev/null
then
  openssl help 2>&1|grep -A 10 '`dgst'|grep -q md5 || echo -e "Openssl is not supported md5\nThis is not working at your enviroment."
else
  echo "Openssl is too old."
fi
