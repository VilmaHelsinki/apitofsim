# Makefile for APITOF codes

CC = g++
CFLAGS = -std=c++11 -O3 -Wall -fopenmp -march=native
COMMON_UTILS = src/utils.h
BINS_DEFAULT = bin/apitof_pinhole bin/densityandrate_win bin/skimmer_win
BINS_ICX = $(BINS_DEFAULT:=.icx)
BINS_GCC = $(BINS_DEFAULT:=.gcc)
BINS_CLANG = $(BINS_DEFAULT:=.clang)
BINS_ALL_COMPILERS = $(BINS_ICX) $(BINS_GCC) $(BINS_CLANG)

ICXFLAGS = -fiopenmp -std=c++11 -O3 -Wall

# Compile all codes
.PHONY: all all-compilers clean clean-compilers clean-all
all: ${BINS_DEFAULT}
all-icx: ${BINS_ICX}
all-gcc: ${BINS_GCC}
all-clang: ${BINS_CLANG}
all-compilers: ${BINS_ALL_COMPILERS}

# Default compiler
bin/%: src/%.cpp ${COMMON_UTILS}
	${CC} ${CFLAGS} $< -o $@

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
	rm -f $(BINS_DEFAULT)

clean-compilers:
	rm -f $(BINS_ALL_COMPILERS)

clean-all: clean clean-compilers
