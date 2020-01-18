FROM alpine

RUN apk --no-cache add python3 build-base python3-dev && \
    pip3 install --upgrade pip && \
    pip3 install pycrypto

COPY ./src/* /

CMD /solver.sh
