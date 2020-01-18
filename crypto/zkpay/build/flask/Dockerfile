FROM ubuntu:16.04

RUN apt-get update
RUN apt-get install -y python3 python3-pip libzbar0 libzbar-dev
RUN apt-get install -y build-essential pkg-config cmake git libgmp3-dev libprocps4-dev python-markdown libboost-all-dev libssl-dev

RUN pip3 install --upgrade pip
RUN pip3 install setuptools --upgrade

ADD ./src/requirements.txt /
RUN pip3 install -r requirements.txt
RUN rm requirements.txt

RUN useradd flask
ADD ./src/ /usr/src/flask/
RUN chown -R flask:flask /usr/src/flask/
USER flask
ENV HOME /usr/src/flask/

WORKDIR /usr/src/flask/snark/
RUN mkdir build
WORKDIR /usr/src/flask/snark/build/
RUN cmake .. && make

WORKDIR /usr/src/flask/

RUN python3 init.py

CMD gunicorn app:app -b :8000 --name app -k gevent --worker-connections 1000
