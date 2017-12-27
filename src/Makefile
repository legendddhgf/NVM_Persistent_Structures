# NPSB = NVM Persistent Structures Benchmark
EXE = NPSB
MAIN = Bench
MAINSRC = $(MAIN:%=%.c)
MODULE = SLL Vector BTree Common
HEADER = $(MODULE:%=%.h)
MODULESRC = $(MODULE:%=%.c)
SRC = $(MAINSRC) $(MODULESRC)
OBJ = $(SRC:%.c=%.o)
COMPILE = gcc -c -g --std=c99 -Wall -Werror -Wextra -Wpedantic
COMPILE += -msse2 -march=native
# because valgrind doesn't like seeing you use cacheline flush and what not
#COMPILE += -DVALGRIND
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
