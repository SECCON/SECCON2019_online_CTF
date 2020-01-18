Writeup
=====


想定解法
-----
### シンボリック実行
以下は実行トレースを扱えるTritonを想定した流れ。ソルバーの実行時間は2分程度。実装は割と難しい。

1. Tritonの [テンプレートコード](https://github.com/JonathanSalwan/Triton/blob/master/src/examples/python/small_x86-64_symbolic_emulator.py) を改造していく。
    * まずこのコードの存在を知っているかどうかが一つのポイント。
2. バイナリをロード＆実行し、実行トレースに記録された各ブランチがその挙動をするための制約を収集する。
    * 他にもやることはあるが省略。
3. 制約を解くと問題で与えたトレースと分岐命令レベルで実行トレースが一致する入力を得る。
4. （任意）問題で与えたトレース生成PinToolをコンパイル＆実行し、生成した入力に対する実行トレースを得る。
5. （任意）問題で与えたトレースと生成したトレースが分岐命令の挙動レベルで一致するすることを確認する。
4. 判定サーバーにその入力を送り、実行トレースの一致を確認したらフラグを渡す。

### 別解
思いつかない。


許容解法
----
### ブルートフォース【未実証】
入力は90文字程度なのでうまくいったとしてもかなりしんどいはず…。

* 以下に入力の各位置の文字が他の位置の文字に影響を与えない場合の計算量 $O(MN)$ なアルゴリズム案を示す。
    * 入力の各位置の文字が他の位置の文字に影響を与えるように実装してるつもり。実際は計算量が爆発すると期待しているが…。
* 関数check()は検査する文字列での実行トレースと問題で与えた実行トレースの一致率をいい感じに判定する関数。

```python
answer = ""
while True:
    for x in ["0,", "1,", ..., "1000,", "+", "-", ...]:
        res = check(flag + x)
        if res == <flag + xで実行トレースが一致>:
            break
        elif res == <flag + xで実行トレースの一致率が向上>:
            flag += x        
```



# Writeup (en)
=======

Expected solution
-----
### Symbolic execution
The flow below assumes Triton, which can handle execution traces. Solver execution time is about 2 minutes. Implementation is relatively difficult.

1. Modify Triton [template code](https://github.com/JonathanSalwan/Triton/blob/master/src/examples/python/small_x86-64_symbolic_emulator.py)
    * First, whether or not you know the existence of this code is one point.
2.  Load and execute the binaries and collect the constraints for each branch recorded in the execution trace to behave.
    * I have other things to do, but omit them.
3. When the constraint is solved, an input whose execution trace matches the trace given in the problem at the branch instruction level is obtained.


4. （Optional）Compile and execute the trace generation PinTool given in the problem to obtain an execution trace for the generated input.
5. （Optional）Verify that the trace given in the problem matches the generated trace at the behavior level of the branch instruction.
4. Send the input to the decision server and get the flag when the execution trace matches.


### Another solution
can not think of.


Allowed solution
----
### Brute force【unverified】
The input is about 90 characters, so even if it works, it should be quite tough.

*The following is an algorithm proposal with computational cost $ O (MN) $ when the character at each position of the input does not affect the character at other positions.
    * I will implement it so that the character at each position of the input affects the character at other positions. We actually expect the amount of computation to explode ...
* The function check () is a function that determines the match rate between the execution trace of the character string to be checked and the execution trace given in the problem.

```python
answer = ""
while True:
    for x in ["0,", "1,", ..., "1000,", "+", "-", ...]:
        res = check(flag + x)
        if res == <flag + x matches execution trace>:
            break
        elif res == <flag + xでimproves execution trace match rate>:
            flag += x        
```
