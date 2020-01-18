follow-me
====


世界観
----
- 電卓を模したプログラムの分岐命令の挙動を記録した実行トレースから、当時入力された計算式を復元する

### 前提知識
- シンボリック実行を理解している
    - 実行トレースをもとに入力を復元するようなシンボリック実行の方法論を知っている。
    - ソルバーの作り方によっては、ローダーおよびmain関数呼び出し前の処理の内容、分岐命令のセマンティクスなどなどを理解している必要あり。
- Intel Pinを扱える
    - PinToolをコンパイルできたり、APIマニュアルを読解できる程度の知識でOK。


問題文
----
```
I have an execution trace of calc, but I forgot what I inputted.
Can you submit the input (formula) which follows my execution trace to my server?

Challenge server

- Server compares branch instructions' behavior of your input's and original execution traces, and gives you flag if these are the same.
- NOTE: You MUST NOT attack challenge server (including too frequent access).
- Location: http://follow-me.chal.seccon.jp/
- Sample curl command to submit formula: `curl -q -H 'Content-Type:application/json' -d "{\"input\": \"your formula comes here\"}" http://follow-me.chal.seccon.jp/submit/quals/0`

Attached files

- Executed binary (excluding dynamic libraries): calc
- Execution trace generated by tracer: calc.trace
- Source code of tracer developed on the top of Pin: branchtrace.cpp
```


難易度
----
- 想定解法の場合、Medium Hard。想定所要時間6～12時間。
    - シンボリック実行を理解しているとする。
- 万が一ブルートフォース可能の場合、Medium Easy。想定所要時間70時間（1並列の場合）。
    - 文字の種類をM、計算式の長さをN、数値の長さの上限をlとすると、計算量は $$O(M^l \times N)$$
    - M = 10, l = 3, N = 90, 1回の実行時間を 0.5秒 とする。



follow-me (en)
=======

View of the world
----
- Restores the equation entered at the time from the execution trace that records the behavior of branch instructions in a program imitating a calculator

### Prerequisite knowledge
- Knowledge of symbolic execution
    - Know symbolic execution methodologies that restore input based on execution traces.
    - Depending on how the solver is created, it is necessary to understand the contents of processing before calling the loader and main function, the semantics of branch instructions, and so on.
- Knowledge of Intel Pin
    - It's enough if you can compile PinTool and understand the API manual.


Challenge text
----
```
I have an execution trace of calc, but I forgot what I inputted.
Can you submit the input (formula) which follows my execution trace to my server?

Challenge server

- Server compares branch instructions' behavior of your input's and original execution traces, and gives you flag if these are the same.
- NOTE: You MUST NOT attack challenge server (including too frequent access).
- Location: http://follow-me.chal.seccon.jp/
- Sample curl command to submit formula: `curl -q -H 'Content-Type:application/json' -d "{\"input\": \"your formula comes here\"}" http://follow-me.chal.seccon.jp/submit/quals/0`

Attached files

- Executed binary (excluding dynamic libraries): calc
- Execution trace generated by tracer: calc.trace
- Source code of tracer developed on the top of Pin: branchtrace.cpp
```


Difficulty
----
- Medium Hard for the assumed solution. Estimated required time is 6 to 12 hours.
    - Suppose you understand symbolic execution. 
- If brute force is possible, Medium Easy. Estimated required time 70 hours (in case of 1 parallel).
    - If the character type is M, the length of the calculation formula is N, and the upper limit of the number length is l, the calculation amount is $$ O (M ^ l \ times N) $$
    - M = 10, l = 3, N = 90, and one execution time is 0.5 seconds.