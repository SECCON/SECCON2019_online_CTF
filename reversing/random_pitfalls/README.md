# 問題文
${HOSTNAME}:10101

# 問題概要
メモリ空間に散りばめられたフラグの材料を集めるシェルコード問題。
連続するメモリ領域64ページのうちランダムに選んだ32ページを読み書き可能、
残りはアクセス不可な状態にして、読み書き可能な領域にフラグの材料を分散させておく。
具体的には31ページにランダムな値（r0〜r30）、残りの1ページにr0〜r30と
フラグの値(f)、ぜんぶのXORをとった値（r0^r1^...^r30^f）を書いておく。
アクセス不可なページに一切触らずに32個のフラグの材料を集めればフラグを計算できる。

# 備考
- サーバは起動から5秒経つとSIGTERMを受け取って終了する
    - timeoutコマンドを使ってる
    - セッション張ったままの人対策
- １セッション当たり使えるCPU時間は1秒だけ
    - xinetd.confでrlimit_cpu=1としてる
    - busy loopとかうざいシェルコード対策
- 呼び出せるシステムコールはwrite、許されているパラメータはfd=1, buf=特定アドレス, len=40のみ
    - fork bombとかいらんことされないように
    - writeに指定可能なアドレスを任意にしてしまうと、アクセス可否のチェックに使われてしまうので、アドレスも縛ってる

# 難易度
TSX-NIのxbegin/xendを使ってページフォルトを発生させずにメモリスキャンをする。
"SGX-ROP: Practical Enclave Malware with Intel SGX"っていう論文にあったテクニック。
知らない人にはつらそうなので「CPUの機能とか関係あるのかな？」と思ってもらえるように、
ヒントとしてバナー代わりに/proc/cpuinfoを表示するようにした。

# 問題サーバのセットアップ
## コンパイル
cd build/src
make
## サーバ作成
cd build
docker build -t random_pitfalls_server .
## サーバ起動
docker run -d -p 10101:10101 random_pitfalls_server

# en
# Challenge text
${HOSTNAME}:10101

# Challenge summary

A shellcode challenge that collects the material of flags scattered in memory space.
32 pages randomly selected from 64 pages in the continuous memory area are readable and writable, and the rest are inaccessible, and the material of the flag is dispersed in the readable and writable area.
Specifically, a random value (r0-r30) for 31 pages, r0-r30 and a flag value (f) for the remaining one page, and a value obtained by XORing all (r0 ^ r1 ^ ... ^ r30 ^ Write down f).
You can calculate the flags by gathering 32 flags without touching any inaccessible pages.



# Remarks
- The server receives SIGTERM and terminates after 5 seconds from startup
    - I use the timeout command
    - Countermeasures for people who leave a session
- Only 1 second of CPU time can be used per session
    - rlimit_cpu = 1 in xinetd.conf
    - Countermeasures against busy loop and busy shell code
- Only system calls that can be called are write, only fd = 1, buf = specific address, len = 40
    - Don't be a fork bomb
    - If the address that can be specified for write is made arbitrary, it will be used for checking whether access is possible, so the address is also tied

# Difficulty
Scan memory without generating page faults using xbegin / xend of TSX-NI.
Technique from the paper "SGX-ROP: Practical Enclave Malware with Intel SGX".
As it seems to be difficult for people who do not know, as if you think "Is it related to CPU functions?"
To help people who don't know, think "Is there anything to do with CPU features?", So that hints now show / proc / cpuinfo instead of a banner.



# Set up challenge server
## Compile
cd build/src
make
## Create server
cd build
docker build -t random_pitfalls_server .
## Server start
docker run -d -p 10101:10101 random_pitfalls_server
