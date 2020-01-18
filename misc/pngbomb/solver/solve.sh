#!/bin/sh
zcat ../files/q.png.gz | node step1.js | tail +840 | head -83 | node step2.js | zbarimg -- -
