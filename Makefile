

GZSTREAMLIBPATH = ./filo/src/common/gzstream
TCLAPINCLUDEPATH = ./tclap/include

CXX      = g++   # for Linux RedHat 6.1, g++ version 2.95.2
CXXFLAGSFILO = -I${GZSTREAMLIBPATH} -O2 -Wall
CXXFLAGSMAIN = -I${GZSTREAMLIBPATH} -I${TCLAPINCLUDEPATH} -I. -O2 -Wall

LDFLAGS  = -L${GZSTREAMLIBPATH} -lgzstream -lz
AR       = ar cr


default: 10xToFastq

lib: ${GZSTREAMLIBPATH}/libgzstream.a

clean:
	@echo "Cleaning up."
	@rm -f ${GZSTREAMLIBPATH}/libgzstream.a ${GZSTREAMLIBPATH}/gzstream.o
	@rm -f main.o 10xToFastq

${GZSTREAMLIBPATH}/gzstream.o : ${GZSTREAMLIBPATH}/gzstream.C ${GZSTREAMLIBPATH}/gzstream.h
	${CXX} ${CXXFLAGSFILO} -c -o ${GZSTREAMLIBPATH}/gzstream.o ${GZSTREAMLIBPATH}/gzstream.C
	
${GZSTREAMLIBPATH}/libgzstream.a : ${GZSTREAMLIBPATH}/gzstream.o
	@echo "Building gzstreamlib"
	${AR} ${GZSTREAMLIBPATH}/libgzstream.a ${GZSTREAMLIBPATH}/gzstream.o

main.o: main.cpp ${GZSTREAMLIBPATH}/libgzstream.a
	${CXX} ${CXXFLAGSMAIN} -c -o main.o main.cpp
	
10xToFastq: main.o
	${CXX} -o 10xToFastq main.o ${LDFLAGS}
	

