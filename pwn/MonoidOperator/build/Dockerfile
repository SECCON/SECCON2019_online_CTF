FROM ubuntu:19.04
RUN apt-get update && apt-get upgrade -y &&\
    apt-get install xinetd -y &&\
    useradd -m monoid
COPY ./monoid_xinet.conf /etc/xinetd.d/monoid
COPY ./monoid_operator /home/monoid
COPY ./flag.txt /home/monoid/flag.txt
RUN chown monoid:monoid /home/monoid/monoid_operator &&\
    chmod 444 /etc/xinetd.d/monoid &&\
    chmod 444 /home/monoid/flag.txt
EXPOSE 27182/tcp
USER monoid
ENTRYPOINT ["xinetd","-dontfork"]
