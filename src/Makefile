# NPSB = NVM Persistent Structures Benchmark
# I set this up so the main C src has a header file which is included with CPP
# CPP code calls C code in order to utilize pilot

EXE = NPSB
MAINC = Bench
MAINCPP = Main
MAINSRCC = $(MAINC:%=%.c)
MAINSRCCPP = $(MAINCPP:%=%.cpp)
MODULEC = SLL Vector BTree LSM Common
MODULECPP =
HEADERC = $(MODULEC:%=%.h)
HEADERCPP = $(MAINC:%=%.h) $(MODULECPP:%=%.hpp)
MODULESRCC = $(MODULEC:%=%.c)
MODULESRCCPP = $(MODULECPP:%=%.cpp)
SRCC = $(MAINSRCC) $(MODULESRCC)
SRCCPP = $(MAINSRCCPP) $(MODULESRCCPP)
OBJC = $(SRCC:%.c=%.o)
OBJCPP = $(SRCCPP:%.cpp=%.o)
COMPILEC = gcc -c
COMPILECPERSIST = -DPMEM
COMPILECPP = g++ -c
COMPILECFLAGS = -g --std=c99 -Wall -Werror -Wextra -Wpedantic
COMPILECFLAGS += -msse2 -march=native
COMPILECPPFLAGS = -g --std=c++11 -Wall -Werror -Wextra -Wpedantic
LINKC = gcc -o
LINKCFLAGS = -lpmemobj
LINKCPP = g++ -o
LINKCPPFLAGS = $(LINKCFLAGS) -lpilot
REMOVE = rm -f

# non-persistent tests
$(EXE): $(OBJCPP) $(OBJC)
	$(LINKCPP) $(EXE) $(OBJCPP) $(OBJC) $(LINKCPPFLAGS)

# persistent tests
pexe: $(OBJCPP) OBJCPERSIST
	$(LINKCPP) $(EXE) $(OBJCPP) $(OBJC) $(LINKCPPFLAGS)

run-all: clean run clean prun

run: $(EXE)
	./$(EXE)
	./get_results.sh

prun: pexe
	sudo ./$(EXE)
	./get_results.sh

check: $(EXE)
	valgrind --leak-check=full --show-leak-kinds=all -v ./$(EXE)

$(OBJCPP): $(SRCCPP) $(HEADERCPP)
	$(COMPILECPP) $(SRCCPP) $(COMPILECPPFLAGS)

$(OBJC): $(SRCC) $(HEADERC)
	$(COMPILEC) $(SRCC) $(COMPILECFLAGS)

OBJCPERSIST: $(SRCC) $(HEADERC)
	$(COMPILEC) $(COMPILECPERSIST) $(SRCC) $(COMPILECFLAGS)

clean:
	$(REMOVE) $(OBJC) $(OBJCPP)

spotless: clean
	$(REMOVE) $(EXE)

.PHONY: clean spotless run prun run-all check OBJCPERSIST
