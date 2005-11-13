#!/bin/sh -x

# build article
(
cd article
latex article.tex
dvips article.dvi -o article.ps
ps2pdf article.ps

mv article.pdf ..
rm -f article.{aux,dvi,log,ps}
)

rm -f predist.sh
