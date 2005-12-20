
NAME = Article

BIBS=$(wildcard *.bib)

PNGS=$(wildcard *.png)
JPGS=$(wildcard *.jpg)
DIAS=$(wildcard *.dia)
FIGS=$(wildcard *.fig)
EPSS=$(wildcard *.eps)

OUTPUT_EPS=$(PNGS:png=eps) $(JPGS:jpg=eps) $(DIAS:dia=eps) $(FIGS:fig=eps) $(EPSS)

all: ps pdf


ps: ${NAME}.ps
pdf: ${NAME}.pdf
dvi: ${NAME}.dvi
bbl: ${NAME}.bbl
eps: $(OUTPUT_EPS)


%.eps: %.png
	convert $< $@

%.eps: %.jpg
	convert $< $@

%.eps: %.dia
	dia --nosplash -e $@ $<

%.eps: %.fig
	fig2dev -L eps $< $@


${NAME}.ps: ${NAME}.dvi
	$(MAKE) eps
	dvips -o ${NAME}.ps ${NAME}.dvi


${NAME}.pdf: ${NAME}.ps
	ps2pdf ${NAME}.ps


${NAME}.bbl: ${BIBS} ${NAME}.aux
	bibtex ${NAME}


${NAME}.dvi: ${NAME}.tex  ${BIBS}
	latex --interaction batchmode ${NAME}.tex
	bibtex ${NAME}
	latex --interaction batchmode ${NAME}.tex
	latex --interaction batchmode ${NAME}.tex


${NAME}.aux: ${NAME}.tex  ${BIBS}
	latex ${NAME}.tex


clean:
	rm ${NAME}.dvi ${NAME}.ps ${NAME}.pdf ${NAME}.aux ${NAME}.log ${NAME}.bbl ${NAME}.blg ${NAME}.toc ${NAME}.out  ${NAME}.brf $(DIAS:dia=eps) $(PNGS:png=eps) $(FIGS:fig=eps)

