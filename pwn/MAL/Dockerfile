# hashcashで消耗したくないんで、Ubuntu使いますごめんなさい
FROM python:2.7

RUN apt-get update && apt-get install -y hashcash && pip install pwntools

ADD . /MAL

WORKDIR /MAL

ENV HOST mal.chal.seccon.jp
ENV PORT 10001

CMD ["python","exploit.py"]
