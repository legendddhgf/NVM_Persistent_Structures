MAIN = project
TEXPDF = $(MAIN).pdf
TEXSRC = $(MAIN).tex
TEXOBJ = *.aux *.log *.toc *.out *.fdb_latexmk *.fls *.bak *.blg *.bbl
COMPILETEX = latexmk -pdf
PDFREADER = evince
SPELLCHECK = aspell -t -c

DELETE = rm -f

run: $(TEXPDF)
	$(PDFREADER) $<

$(TEXPDF): $(TEXSRC)
	$(SPELLCHECK) $(TEXSRC) && $(COMPILETEX) $^

clean:
	$(DELETE) $(TEXOBJ)

spotless: clean
	$(DELETE) $(TEXPDF)

.PHONY: clean spotless
