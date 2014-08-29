SHELL=/bin/bash

TEX_FILE=report
TEST_FILE=report-test
PRES_FILE=presentation
DESTDIR=/var/www/constraint
DEST=$(DESTDIR)/report-masters.pdf
SHORT_FILE=short-presentation

TARGET=./target

MK=lualatex -shell-escape -output-directory=$(TARGET)

all: $(TEX_FILE)

min:
	$(MK) min.tex
	$(MK) min.tex
	$(MK) min.tex
	cp $(TARGET)/min.pdf $(DEST)

test:
	$(MK) $(TEST_FILE) 
	cp $(TARGET)/$(TEST_FILE).pdf $(DEST)

presentation:
	$(MK) $(PRES_FILE) 
	cp $(TARGET)/$(PRES_FILE).pdf $(DESTDIR)

short:
	$(MK) $(SHORT_FILE)
	cp $(TARGET)/$(SHORT_FILE).pdf $(DESTDIR)

dist:
	make $(TEX_FILE) 
	cp $(TARGET)/$(TEX_FILE).pdf $(DEST)
	make spellcheck
	make fixmes

edit:
	vi $(TEX_FILE).tex
	make dist

fixmes:
	grep FIXME *.tex | grep -v newcommand

$(TEX_FILE):
	$(MK) $(TEX_FILE).tex 
	biber --output_directory=$(TARGET) $(TEX_FILE)
	$(MK) $(TEX_FILE).tex 
	$(MK) $(TEX_FILE).tex 

spellcheck:
	sed 's|.*|^&$$|g' wordlist > wordlist.grep
	find . -name "*.tex" -print0 | grep -Ezv "(desround|results-table|results-graphs|results)\.tex" | xargs -0 hunspell -a -l -t -i utf-8 | sort | uniq | grep -v -f wordlist.grep | cat - <(echo -e "\n\n") | cat <(echo -e "\n\nSPELLCHECK:") -
	rm -f wordlist.grep

clean:
	rm -rf $(TARGET)/*
	mkdir $(TARGET)/figures
