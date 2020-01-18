FROM ubuntu:18.04

ENV APP_BASE_DIR /app

ENV TMP_DIR ${APP_BASE_DIR}/tmp
ENV TRITON_ROOT ${APP_BASE_DIR}/Triton
RUN mkdir -p $TMP_DIR $TRITON_ROOT

WORKDIR ${APP_BASE_DIR}

### Install requirements
RUN apt update && apt install -y bash git build-essential cmake libboost-dev python3-distutils python3 python-pip python python-dev

### Install z3
RUN cd $TMP_DIR && git clone https://github.com/Z3Prover/z3.git && cd z3 && \
    git clean -fx src && \
    mkdir build && \
    cd build && \
    cmake -G "Unix Makefiles" ../ && \
    make -j4 && \
    make install
RUN ls /usr/local/lib/cmake/z3/*.cmake

### Install capstone
RUN cd $TMP_DIR && \
    git clone https://github.com/aquynh/capstone.git && \
    cd capstone && \
    ./make.sh && \
    ./make.sh install

### Install keystone
RUN cd $TMP_DIR && \
    git clone https://github.com/keystone-engine/keystone.git && \
    cd keystone && \
    mkdir build && \
    cd build && \
    ../make-share.sh && \
    make install

### Install triton without Intel Pin
RUN git clone https://github.com/JonathanSalwan/Triton.git $TRITON_ROOT && \
    cd $TRITON_ROOT && \
    git checkout bd738d018de511a5cc66cb4b6870d376145de605 && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j4 && \
    make install

WORKDIR ${APP_BASE_DIR}/follow-me/solver
COPY ../../requirements.txt .
RUN pip install -r requirements.txt
