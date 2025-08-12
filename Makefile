# Makefile for APITOF codes

CC = g++
CFLAGS = -std=c++11 -O3 -Wall -fopenmp -march=native
COMMON_UTILS = src/utils.h
BINS_DEFAULT = bin/apitof_pinhole bin/densityandrate_win bin/skimmer_win

ICXFLAGS = -fiopenmp -std=c++11 -O3 -Wall

# Compile all codes
.PHONY: all
all: ${BINS_DEFAULT}
all-compilers: $(BINS_DEFAULT:=.icx) $(BINS_DEFAULT:=.gcc) $(BINS_DEFAULT:=.clang)

# Default compiler
bin/%: src/%.cpp ${COMMON_UTILS}
	${CC} ${CFLAGS} $< ${OPENMP} -o $@

# Intel compiler
bin/%.icx: src/%.cpp ${COMMON_UTILS}
	icpx ${ICXFLAGS} $< -o $@

# GCC
bin/%.gcc: src/%.cpp ${COMMON_UTILS}
	g++ ${CFLAGS} $< -o $@

# Clang
bin/%.clang: src/%.cpp ${COMMON_UTILS}
	clang++ ${CFLAGS} $< -o $@

# Remove executables
clean:
	rm -f bin/*
