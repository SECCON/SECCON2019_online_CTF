#!/bin/sh

LD_PRELOAD="./libc.so.6" socat TCP-LISTEN:33333,reuseaddr,fork EXEC:./lazy &
