FROM ubuntu:16.04

RUN apt-get update
RUN apt-get install -y python3 python3-pip libzbar0 libzbar-dev
RUN apt-get install -y build-essential pkg-config cmake git libgmp3-dev libprocps4-dev python-markdown libboost-all-dev libssl-dev libjson-c-dev

RUN pip3 install --upgrade pip
RUN pip3 install setuptools --upgrade

ADD ./src/requirements.txt /
RUN pip3 install -r requirements.txt
RUN rm requirements.txt

RUN useradd solver
ADD ./src/ /usr/src/solver/
RUN chown -R solver:solver /usr/src/solver/

USER solver
ENV HOME /usr/src/solver/

WORKDIR /usr/src/solver/snark/
RUN mkdir build

WORKDIR /usr/src/solver/snark/build/
RUN cmake .. && make

WORKDIR /usr/src/solver/

CMD python3 Solver.py
