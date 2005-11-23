#!/bin/sh -x

# build article
(
cd article
pdflatex article.tex
pdflatex article.tex

mv article.pdf ..
rm -f article.{aux,dvi,log,ps}
)

rm -f predist.sh
