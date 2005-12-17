#!/bin/sh -x

# build article
(
cd article
make
cp Article.pdf ..
make clean
)

rm -f predist.sh
