MAIN = COMPARISON_NVM_PERSISTENT_STRUCTURES
TEXPDF = $(MAIN).pdf
TEXSRC = $(MAIN).tex
TEXOBJ = *.aux *.log *.toc *.out
COMPILETEX = pdflatex
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
