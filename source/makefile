# Makefile for APITOF codes

CC = g++
CFLAGS = -std=c++11 -O3 -Wall
OPENMP = -Xpreprocessor -fopenmp #-lomp

# Compile all codes
.PHONY: all
all: apitof_pinhole.x densityandrate_win.x skimmer_win.x

# Run main code
.PHONY: runapitof
runapitof: apitof_pinhole.x
	./apitof_pinhole.x < input.in

# Run densityandrate
.PHONY: rundensity
rundensity: densityandrate_win.x
	./densityandrate_win.x < input.in

# Run skimmer
.PHONY: runskimmer
runskimmer: skimmer_win.x
	./skimmer_win.x < input.in

# Main code
apitof_pinhole.x: apitof_pinhole_original.cpp
	${CC} ${CFLAGS} ${OPENMP}  apitof_pinhole_original.cpp -o apitof_pinhole.x

# Density and fragmentation rate code
densityandrate_win.x: densityandrate_win.cpp
	${CC} ${CFLAGS} ${OPENMP}  densityandrate_win.cpp -o densityandrate_win.x

# Fluid dynamics code
skimmer_win.x: skimmer_win.cpp
	${CC} ${CFLAGS} ${OPENMP}  skimmer_win.cpp -o skimmer_win.x

# DSMC code
#dsmc.x: dsmc.cpp
#	${CC} ${CFLAGS} dsmc.cpp -o dsmc.x

#parallel: apitof.cpp
#	${CC} ${CFLAGS} ${OPENMP} apitof.cpp -o apitof.x

# Remove executables
clean:
	rm -f *.x
