# NPSB = NVM Persistent Structures Benchmark
EXE = NPSB
MAIN = Bench
MAINSRC = $(MAIN:%=%.c)
MODULE = SLL Vector BTree Common
HEADER = $(MODULE:%=%.h)
MODULESRC = $(MODULE:%=%.c)
SRC = $(MAINSRC) $(MODULESRC)
OBJ = $(SRC:%.c=%.o)
COMPILE = gcc -c --std=c99 -Wall -Werror -Wextra -pedantic
LINK = gcc -o
REMOVE = rm -f

$(EXE): $(OBJ)
	$(LINK) $@ $^

$(OBJ): $(SRC) $(HEADER)
	$(COMPILE) $(SRC)

clean:
	$(REMOVE) $(OBJ)

spotless: clean
	$(REMOVE) $(EXE)

.PHONY: clean spotless
