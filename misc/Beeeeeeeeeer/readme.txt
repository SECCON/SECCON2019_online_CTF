## ja
問題名:Beeeeeeeeeer

■問題文
Let's decording!

■　実行方法

chmod u+x Beeeeeeeeeer
./Beeeeeeeeeer

■　解法(全て解読できた前提で、実直にやると)

1.ログの取得
 ログを取得する(エスケープシーケンスコードによりフラグが隠蔽されるため)
 script -f log.txt

2.引数チェックの回避
 第一引数にユーザ名(whoamiの出力結果)を入れて実行する
 ./Beeeeeeeeeer `whoami`
 アニメーションするので暫く待つ

2.sleepの開始
 Let's decording!(≧∀≦*)　が出力される
 Enterを一回押す
 sleepで処理が止まるので、別ターミナルからkill -9 によって停止させるのを複数回繰り返す

3.beep回数の入力
 beepが複数回鳴り、鳴った回数を入力するよう求められる
 How many beeps? が出力されて入力を促される。複数回繰り返す。
 (最大10回なので、手動で可。解読できていれば処理のスキップもできる)

4.パスワードの入力
 Enter the password が表示される
 最後にパスワードを入力する bash　と入力
 (これは最後まで解読するとパスワードが読み取れる)

5.フラグ出力
 フラグが出力される、ただしターミナルの折返しを無視してするエスケープシーケンスコードにより、ターミナルから見切れて出力される。
 scriptコマンド等で記録しておかないと見逃す
 exit
 view log.txt

以上を難読化されたシェルスクリプトを解読してすすめる

■解法のテクニック

1.1行ソースコードから複数行に整形

sed -e 's/;/;\n/g' beeeeeeeeeer

2.読み飛ばせそうな所を一度にコメントアウトする

cat << EOS #ヒアドキュメントを利用してコメントアウト

echo -e "\033#8";
sleep 1;
C=$(tput cols);
L=$(tput lines);
for ID in $(seq $(($L*$C*6)));
do x=$(($RANDOM%$C));
y=$(($RANDOM%$L));
printf "\033[${y};
${x}f ";
done;
for ID in $(seq $(($L*$C*6)));
do x=$(($RANDOM%$C));
y=$(($RANDOM%$L));
printf "\033[${y};
${x}fF";
done;

EOS #EOSで囲まれた部分は実行されない

3.echoを利用すると難読化を複合できる

echo $'\164\162\141\160' '' $'\61' $'\62' $'\63' $'\x31\x35' $'\x31\x38' $'\u0031\u0039' $(echo MjAK |base64 -d);
↓
trap  1 2 3 15 18 19 20

4.bash -x の利用

bash -x test.sh

+ echo 'Enter the password'
Enter the password
+ read _____

5.ダミーコマンドの用意

例えば処理を中断されるsleepは、すぐ復帰するダミーコマンドに置き換える事ができる。
その他のコマンドも、引数内容を記録した上で、本来の処理を呼び出す動作をするコマンドに差し替えると、処理の内容が読み取れる。


## en
Challenge Title:Beeeeeeeeeer

■ Challenge Test
Let's decording!

■　How to execute

chmod u+x Beeeeeeeeeer
./Beeeeeeeeeer

■　Writeup(Assuming that everything was decrypted, if you do it honestly)

1.Get log
 Get logs(Because the flag is hidden by the escape sequence code)
 script -f log.txt

2.Avoid argument checking
 Execute with the user name (the output result of whoami) in the first argument
 ./Beeeeeeeeeer `whoami`
 Wait for a while as it will be animated

2.Start sleep
 "Let's decording!(≧∀≦*)""　displayed
 Press Enter once
 Since processing stops with sleep, it is repeated multiple times to stop from another terminal with kill -9

3.Enter the number of beep sounds
 After multiple times beep sound, you are prompted to enter the number of times
 Display "How many beeps?" and prompted for input. Repeat several times.
 (Maximum of 10 times, so you can do manually. If it can be decoded, you can skip this process)


4.Enter the password
 Displayed "Enter the password"
 Finally, enter the password. Enter "bash"
 (This can be read by decrypting the password to the end)


5.Flag output
 Flags are output, but escaped from the terminal by an escape sequence code that ignores terminal wrapping.
 Overlook if not recorded by script command etc.
 exit
 view log.txt

Decrypt the above obfuscated shell script and proceed

■Solution techniques

1.Format one line of source code into multiple lines

sed -e 's/;/;\n/g' beeeeeeeeeer

2.Comment out places you may skip over at once

cat << EOS #Comment out using heredoc

echo -e "\033#8";
sleep 1;
C=$(tput cols);
L=$(tput lines);
for ID in $(seq $(($L*$C*6)));
do x=$(($RANDOM%$C));
y=$(($RANDOM%$L));
printf "\033[${y};
${x}f ";
done;
for ID in $(seq $(($L*$C*6)));
do x=$(($RANDOM%$C));
y=$(($RANDOM%$L));
printf "\033[${y};
${x}fF";
done;

EOS #The part enclosed by EOS is not executed

3.Using echo can combine obfuscation

echo $'\164\162\141\160' '' $'\61' $'\62' $'\63' $'\x31\x35' $'\x31\x38' $'\u0031\u0039' $(echo MjAK |base64 -d);
↓
trap  1 2 3 15 18 19 20

4.Use bash -x

bash -x test.sh

+ echo 'Enter the password'
Enter the password
+ read _____

5.Preparation of dummy command

For example, sleep in which processing is interrupted can be replaced with a dummy command that returns immediately.
For other commands, the contents of the processing can be read by recording the contents of the arguments and replacing the command with a command that performs an operation for calling the original processing.
