# NPSB = NVM Persistent Structures Benchmark
EXE = NPSB
MAIN = Bench
MAINSRC = $(MAIN:%=%.c)
MODULE = SLL Vector BTree Common
HEADER = $(MODULE:%=%.h)
MODULESRC = $(MODULE:%=%.c)
SRC = $(MAINSRC) $(MODULESRC)
OBJ = $(SRC:%.c=%.o)
COMPILE = gcc -c
COMPILEFLAGS = -g --std=c99 -Wall -Werror -Wextra -Wpedantic
COMPILEFLAGS += -msse2 -march=native
#COMPILE += -DPMEM
LINK = gcc -o
LINKFLAGS = -lpmemobj
REMOVE = rm -f

$(EXE): $(OBJ)
	$(LINK) $@ $^ $(LINKFLAGS)

$(OBJ): $(SRC) $(HEADER)
	$(COMPILE) $(SRC) $(COMPILEFLAGS)

clean:
	$(REMOVE) $(OBJ)

spotless: clean
	$(REMOVE) $(EXE)

.PHONY: clean spotless
