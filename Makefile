CC=gcc

all: jsteg

jsteg: jsteg.c jsteg.h
	${CC} -o jsteg -g -Wall --std=c99 jsteg.c -ljpeg