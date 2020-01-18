ソルバー
=====

（ソルバーを起動し）、判定サーバーに提出するコマンド

```
make -e SERVER=localhost:8000
```


解く方法
----
`?`はその第n引数をその数だけシンボル化するという意味。

```
time ./solver.py ../trace/calc.trace ../sample/calc `python -c "print '?'*200"`
```