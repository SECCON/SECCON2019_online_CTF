# Monoid Operator

## 実行環境

Ubuntu 19.04

Ubuntu GLIBC 2.29-0ubuntu2

(Ubuntu 18.04ではだいたい1/256の確率でしかうまく行かないが一応解くことはできて、コンパイルオプションやLD_PRELOADなどでUbuntu 19.04と同じlibcとld-linux.soを使えば同じように確実に解けるようになります)

実行例
```
socat TCP-L:3001,reuseaddr,nodelay,fork EXEC:./files/monoid_operator,stderr
```

# en
# Monoid Operator

## Execution environment

Ubuntu 19.04

Ubuntu GLIBC 2.29-0ubuntu2

(Ubuntu 18.04 works only with a probability of 1/256, but it can be solved for a while,If you use the same libc and ld-linux.so as Ubuntu 19.04 with compile options and LD_PRELOAD etc., you will be able to solve reliably in the same way)


Execution example
```
socat TCP-L:3001,reuseaddr,nodelay,fork EXEC:./files/monoid_operator,stderr
```
