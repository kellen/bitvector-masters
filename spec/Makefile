TEX_FILE=specification

all: $(TEX_FILE)

$(TEX_FILE):
	pdflatex $(TEX_FILE).tex && bibtex $(TEX_FILE).aux && pdflatex $(TEX_FILE).tex && pdflatex $(TEX_FILE).tex

spellcheck:
	hunspell -l -t -i utf-8 $(TEX_FILE).tex | sort | uniq

clean:
	rm -f $(TEX_FILE).aux
	rm -f $(TEX_FILE).dvi
	rm -f $(TEX_FILE).log
	rm -f $(TEX_FILE).out
	rm -f $(TEX_FILE).bbl
	rm -f $(TEX_FILE).blg
	rm -f texput.aux
	rm -f texput.log
	rm -f $(TEX_FILE)-blx.bib
	rm -f $(TEX_FILE).run.xml

