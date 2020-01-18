FROM ubuntu:18.04

RUN apt-get update && \
        apt-get -y upgrade && \
        apt-get install -y \
            xinetd \
            iproute2

RUN groupadd -r user && useradd -r -g user user

COPY --chown=root:root ./build/ctf.conf /etc/xinetd.d/ctf
COPY --chown=root:user ./flag.txt /flag.txt
COPY --chown=root:user ./files/sum /sum
COPY --chown=root:user ./build/start.sh /start.sh

RUN chmod 444 /etc/xinetd.d/ctf && \
    chmod 444 /flag.txt && \
    chmod 555 /sum && \
    chmod 555 /start.sh

USER user
EXPOSE 10001

CMD ["xinetd", "-dontfork"]

