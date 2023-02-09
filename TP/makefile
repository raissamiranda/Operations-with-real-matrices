#---------------------------------------------------------------------
# Arquivo	: Makefile
# Conteúdo	: compilar o programa main
#---------------------------------------------------------------------
# Opções	: make all - compila tudo
#               : make mem - gera arquivos de registro de acesso
#               : make prof - faz medidas de tempo de desempenho
#               : make gprof - ilustra uso do gprof para depuração
#		: make clean - remove objetos e executável
#---------------------------------------------------------------------

CC = gcc
LIBS = -lm
SRC = src
OBJ = obj
INC = include
BIN = bin
OBJS = $(OBJ)/main.o $(OBJ)/mat.o $(OBJ)/memlog.o
HDRS = $(INC)/mat.h $(INC)/memlog.h $(INC)/msgassert.h
CFLAGS = -pg -Wall -c -I$(INC)

EXE = $(BIN)/main

all:  mem perf gprof

mem: $(EXE)
	$(EXE) -s -1 m1.txt -2 m2.txt -o somares.out -p somalog.out -l
	$(EXE) -m -1 m1.txt -2 m2.txt -o multres.out -p multlog.out -l
	$(EXE) -t -1 m1.txt -o transpres.out -p transplog.out -l

perf: $(EXE)
	$(EXE) -s -1 m1.txt -2 m2.txt -o somares.out -p somalog.out
	$(EXE) -m -1 m1.txt -2 m2.txt -o multres.out -p multlog.out
	$(EXE) -t -1 m1.txt -o res.out -p transplog.out

gprof: $(BIN)/main
	$(EXE) -s -1 m1.txt -2 m2.txt -o somares.out -p somalog.out -l
	gprof $(EXE) gmon.out > somalog.out
	$(EXE) -m -1 m1.txt -2 m2.txt -o multres.out -p multlog.out -l
	gprof $(EXE) gmon.out > multlog.out
	$(EXE) -t -1 m1.txt -o transres.out -p transplog.out -l
	gprof $(EXE) gmon.out > transplog.out

$(BIN)/main: $(OBJS)
	$(CC) -pg -o $(BIN)/main $(OBJS) $(LIBS)

$(OBJ)/main.o: $(HDRS) $(SRC)/main.c
	$(CC) $(CFLAGS) -o $(OBJ)/main.o $(SRC)/main.c

$(OBJ)/mat.o: $(HDRS) $(SRC)/mat.c
	$(CC) $(CFLAGS) -o $(OBJ)/mat.o $(SRC)/mat.c

$(OBJ)/memlog.o: $(HDRS) $(SRC)/memlog.c
	$(CC) $(CFLAGS) -o $(OBJ)/memlog.o $(SRC)/memlog.c
	
clean:
	rm -f $(EXE) $(OBJS) gmon.out