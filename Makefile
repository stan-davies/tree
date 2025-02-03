CMD = gcc

FILES = main.c

OUTPUT = tree

all : compile
compile:
	@${CMD} ${FILES} -o ${OUTPUT}
