FROM python:2.7

RUN pip install pwntools
ADD solver/exp.py /root

ENV TARGET_HOST "XXX.XXX.XXX.XXX"

WORKDIR /root

CMD python ./exp.py
