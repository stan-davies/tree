CMD = gcc

FILES = main.c

OUTPUT = -o tree

all : compile
compile:
	${CMD} ${FILES} ${OUTPUT}
