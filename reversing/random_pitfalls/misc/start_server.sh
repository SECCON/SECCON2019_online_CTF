#!/bin/sh
socat tcp-listen:10101,reuseaddr,fork exec:./server
