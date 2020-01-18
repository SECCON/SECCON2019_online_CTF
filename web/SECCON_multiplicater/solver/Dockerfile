FROM ubuntu
LABEL maintainer="kanata <kanata2003@yahoo.co.jp>"
RUN echo "now building..."
RUN apt update
RUN apt install -y curl
RUN apt install -y nkf
RUN apt install -y netcat
RUN apt install -y xxd
RUN apt install -y bc
COPY solver.sh /root
RUN chmod 777 /root/solver.sh
ENTRYPOINT ["/root/solver.sh"]

# ビルド手順
# solver.shをカレントディレクトリに置いて、以下を実行
# docker build -t [任意のイメージ名] .

# 使い方
# docker run --rm --name [任意のコンテナ名] [ビルド時に命名したイメージ名] [問題サーバのIP] [コネクトバック先IP]
# ※コネクトバック先のIPアドレスは、グローバルIPである必要があります(あとiptablesとかで9888ポートを塞がれているとフラグが降ってこない)
