FROM alpine

RUN apk --no-cache add python3 bash && \
    pip3 install --upgrade pip && \
    pip3 install requests

COPY ./src/* /

CMD /solver.sh
