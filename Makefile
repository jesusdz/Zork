.PHONY: default

OUTPUT = zork
SOURCES = main.cpp
COMPILER_FLAGS = -g -Wall
LINKER_FLAGS =

default:
	g++ -o ${OUTPUT} ${COMPILER_FLAGS} ${LINKER_FLAGS} ${SOURCES}

