# Makefile for APITOF codes

CC = g++
CFLAGS = -std=c++11 -O3 -Wall
OPENMP = -Xpreprocessor -fopenmp #-lomp

# Compile all codes
.PHONY: all
all: bin/apitof_pinhole.x bin/densityandrate_win.x bin/skimmer_win.x

# Main code
bin/apitof_pinhole.x: src/apitof_pinhole_original.cpp
	${CC} ${CFLAGS} ${OPENMP} $< -o $@

# Density and fragmentation rate code
bin/densityandrate_win.x: src/densityandrate_win.cpp
	${CC} ${CFLAGS} ${OPENMP} $< -o $@

# Fluid dynamics code
bin/skimmer_win.x: src/skimmer_win.cpp
	${CC} ${CFLAGS} ${OPENMP} $< -o $@

# Remove executables
clean:
	rm -f bin/*.x
