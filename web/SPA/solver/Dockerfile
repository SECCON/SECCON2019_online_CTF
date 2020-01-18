FROM golang:latest as builder
ENV CGO_ENABLED=0
ENV GOOS=linux
ENV GOARCH=amd64

WORKDIR /go/src/github.com/SECCON/2019_online_TSG/Web/SPA/solver
COPY . .
RUN GO111MODULE=on GOOS=linux GOARCH=amd64 go build -o solver main.go

FROM frolvlad/alpine-glibc

RUN apk add --no-cache ca-certificates
COPY --from=builder /go/src/github.com/SECCON/2019_online_TSG/Web/SPA/solver/solver /usr/local/bin/solver

ENV LISTEN_ADDR=:8690
ENV HOST=check.noc.seccon.jp:8690

ENTRYPOINT ["/usr/local/bin/solver"]