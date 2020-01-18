FROM ubuntu:18.04

RUN apt update && apt upgrade -y && \
    apt install -y python3.7 curl build-essential

RUN groupadd -r user && useradd -m -r -g user user

COPY --chown=root:user ./files /MAL
COPY --chown=root:user ./flag.txt /MAL/flag.txt

RUN chmod 444 /MAL/flag.txt

USER user
EXPOSE 31000

WORKDIR /MAL/run


RUN curl https://sh.rustup.rs -sSf | sh -s -- -y --no-modify-path \
        --default-toolchain nightly-2019-10-14

ENV PATH /home/user/.cargo/bin:$PATH

CMD ["./run.py"]
