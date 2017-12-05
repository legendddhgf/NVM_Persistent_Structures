MAIN = project
TEXPDF = $(MAIN).pdf
TEXSRC = $(MAIN).tex
TEXOBJ = *.aux *.log *.toc *.out *.fdb_latexmk *.fls
COMPILETEX = latexmk -pdf
PDFREADER = evince

DELETE = rm -f

run: $(TEXPDF)
	$(PDFREADER) $<

$(TEXPDF): $(TEXSRC)
	$(COMPILETEX) $^

clean:
	$(DELETE) $(TEXOBJ)

spotless: clean
	$(DELETE) $(TEXPDF)

.PHONY: clean spotless
