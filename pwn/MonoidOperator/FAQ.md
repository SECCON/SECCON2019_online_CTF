# ja
# 起動方法
```
docker run -d -p 27182:27182 seccon/monoid
```

再起動はcontainerをkillして立て直せばいい。

# コンテナの作り方

問題バイナリ、Dockerfile、.conf、flag.txtを同じディレクトリに入れて、

```
docker build -t seccon/monoid .
```

# en
# how to start
```
docker run -d -p 27182:27182 seccon/monoid
```

To restart, kill the container and rebuild it.

# How to make a container

Put the challenge binaries, Dockerfile, .conf, flag.txt in the same directory,

```
docker build -t seccon/monoid .
```
