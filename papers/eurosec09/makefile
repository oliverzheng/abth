FILE = abthmsnp
EXT = pdf,ps,dvi,aux,log,toc,bbl,blg

ALL: $(FILE).pdf

abthmsnp.pdf: $(FILE).ps
	ps2pdf $(FILE).ps

abthmsnp.ps: $(FILE).dvi
	dvips $(FILE).dvi

abthmsnp.dvi: $(FILE).tex graphics/infrastructure.eps graphics/plot.eps
	latex $(FILE)
	bibtex $(FILE)
	latex $(FILE)
	latex $(FILE)

clean:
	rm -f $(FILE).{$(EXT)}
