SRC_DIR=src

all: 		main.exe

main.exe:	main.o bfs.o
			gcc -o main.exe main.o bfs.o

main.o:		${SRC_DIR}/main.c ${SRC_DIR}/bfs.h
			gcc -c -std=c90 -Wall -Wpedantic ${SRC_DIR}/main.c

bfs.o: 		${SRC_DIR}/bfs.c
			gcc -c -std=c90 -Wall -Wpedantic ${SRC_DIR}/bfs.c

