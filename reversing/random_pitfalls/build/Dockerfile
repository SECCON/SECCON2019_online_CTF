FROM debian:buster

RUN apt-get update && apt-get -y --no-install-recommends install xinetd libseccomp2 &&\
    rm -r /var/cache/apt /var/lib/apt/lists
COPY server xinetd.conf FLAG /

EXPOSE 10101

CMD ["/usr/sbin/xinetd", "-dontfork", "-f", "/xinetd.conf"]
