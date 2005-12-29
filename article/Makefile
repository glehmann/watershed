
NAME = Article

# you should use the title of the article instead
# spaces are not allowed
PDF_NAME = A_Great_Article_For_The_Insight_Journal

BIBS=$(wildcard *.bib)

PNGS=$(wildcard ../image/*.png)
JPGS=$(wildcard ../images/*.jpg)
DIAS=$(wildcard ../images/*.dia)
FIGS=$(wildcard ../images/*.fig)
EPSS=$(wildcard ../images/*.eps)

OUTPUT_EPS=$(PNGS:png=eps) $(JPGS:jpg=eps) $(DIAS:dia=eps) $(FIGS:fig=eps) $(EPSS)

all: pdf


ps: ${NAME}.ps
pdf: ${PDF_NAME}.pdf
dvi: ${NAME}.dvi
bbl: ${NAME}.bbl
eps: $(OUTPUT_EPS)


%.eps: ../images/%.png
	convert $< $@

%.eps: ../images/%.jpg
	convert $< $@

%.eps: ../images/%.dia
	dia --nosplash -e $@ $<

%.eps: ../images/%.fig
	fig2dev -L eps $< $@


${NAME}.ps: ${NAME}.dvi
	dvips -R0 -o ${NAME}.ps ${NAME}.dvi


${PDF_NAME}.pdf: ${NAME}.ps
	ps2pdf ${NAME}.ps ${PDF_NAME}.pdf


${NAME}.bbl: ${BIBS} ${NAME}.aux
	bibtex ${NAME}


${NAME}.dvi: ${NAME}.tex  ${BIBS}
	$(MAKE) eps
	latex --interaction batchmode ${NAME}.tex
	bibtex ${NAME}
	latex --interaction batchmode ${NAME}.tex
	latex --interaction batchmode ${NAME}.tex


${NAME}.aux: ${NAME}.tex  ${BIBS}
	latex ${NAME}.tex


clean:
	rm -f ${NAME}.dvi ${NAME}.ps ${PDF_NAME}.pdf ${NAME}.aux ${NAME}.log ${NAME}.bbl ${NAME}.blg ${NAME}.toc ${NAME}.out  ${NAME}.brf $(DIAS:dia=eps) $(PNGS:png=eps) $(FIGS:fig=eps)

