# Compiler and flags
COMPILER       = g++
COMPILER_FLAGS = -std=c++0x
EXECUTABLE     = RhoBot

# Executable name and general rule
.SUFFIXES:
.SUFFIXES: .cpp .o

# Some lists
LFILES = init io socket chat
SFILES = ${patsubst %, src/%.cpp, ${LFILES}}
OFILES = ${patsubst %, obj/%.o, ${LFILES}}

VPATH  = src 

# General rule
obj/%.o     : %.cpp
	${COMPILER} ${COMPILER_FLAGS} -c -o $@ $<

# Compile main program 
${EXECUTABLE}: ${OFILES} obj/main.o
	${COMPILER} -o ${EXECUTABLE} ${OFILES} obj/main.o 

# Dependencies
obj/main.o: ${OFILES}

# Cleanup
clean: 
	rm -f obj/*.o ${EXECUTABLE}

#Test DBs
sql: 
	${COMPILER} -o sql src/sqlite/parse.cpp -l sqlite3
