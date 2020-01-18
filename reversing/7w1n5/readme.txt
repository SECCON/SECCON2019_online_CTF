問題名:7w1n5

■問題文
Hi, we are brothers! Can you catch our flag??

■実行方法

chmod u+x Brother1
./Brother1　# straceで解析できる

chmod u+x Brother2
./Brother2 # straceで解析できない

■ソルバ

・solver1_analysis.sh
Brother1に対して実施

execveでコマンド実行しているのが読み取れる

・solver2_roopcmd.sh
Brother2を無限ループで呼び出し続ける

・solver3_catch_process.sh
psコマンドでコマンドの引数情報を取得する
タイミングがシビアなので環境によっては暫く待つ


# en
Challenge Title:7w1n5

■Challenge text
Hi, we are brothers! Can you catch our flag??

■How to execute

chmod u+x Brother1
./Brother1　# can be analyzed with strace

chmod u+x Brother2
./Brother2 # cannot be analyzed with strace

■Solver

・solver1_analysis.sh
Conducted for Brother1

You can read that the command is executed by execve

・solver2_roopcmd.sh
Keep calling Brother2 in an infinite loop

・solver3_catch_process.sh
Obtaining command argument information with the ps command
Because timing is severe, wait for a while depending on the environment
