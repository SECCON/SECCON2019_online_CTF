FROM alpine:latest

RUN apk --update add python3 py2-pip python2-dev git libssl1.1 openssl-dev libffi-dev make gcc musl-dev bash &&\
    rm -rf /var/cache/apk/* &&\
    pip2 install pwntools

COPY ./exploit.py /exploit.py

CMD ["python2","/exploit.py","r"]
