#
# Makefile para programas exemplo
# 

CC=gcc
LIB_DIR=../lib
DBFLAGS=-ggdb3 -O0 -g

all: exemplo mandel teste_vetor series

exemplo: exemplo.c $(LIB_DIR)/libcthread.a
	$(CC) -o exemplo exemplo.c -L$(LIB_DIR) -lcthread -Wall $(DBFLAGS)

teste_vetor: teste_vetor.c $(LIB_DIR)/libcthread.a
	$(CC) -o teste_vetor teste_vetor.c -L$(LIB_DIR) -lcthread -Wall $(DBFLAGS)

mandel: mandel.c $(LIB_DIR)/libcthread.a
	$(CC) -o mandel mandel.c -L$(LIB_DIR) -lcthread -lm -Wall $(DBFLAGS)

barbeiro: barbeiro.c $(LIB_DIR)/libcthread.a
	$(CC) -o barbeiro barbeiro.c -L$(LIB_DIR) -lcthread -lm -Wall $(DBFLAGS)

filosofos: filosofos.c $(LIB_DIR)/libcthread.a
	$(CC) -o filosofos filosofos.c -L$(LIB_DIR) -lcthread -lm -Wall $(DBFLAGS)

prodcons: prodcons.c $(LIB_DIR)/libcthread.a
	$(CC) -o prodcons prodcons.c -L$(LIB_DIR) -lcthread -lm -Wall $(DBFLAGS)

series: series.c $(LIB_DIR)/libcthread.a
	$(CC) -o series series.c -L$(LIB_DIR) -lcthread -lm -Wall $(DBFLAGS)
	
clean:
	rm -rf exemplo mandel teste_vetor barbeiro filosofos prodcons series *.o *~




