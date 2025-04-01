CC = clang -Ofast
CFLAGS = -Wall -Wextra -Werror -Wpedantic

all: identify

node: node.o
	$(CC) -o node node.o

ht: ht.o node.o speck.o
	$(CC) -o ht ht.o node.o speck.o

bv: bv.o
	$(CC) -o bv bv.o

bf: bf.o bv.o speck.o
	$(CC) -o bf bf.o bv.o speck.o

parser: parser.o
	$(CC) -o parser parser.o

text: text.o bf.o ht.o parser.o bv.o node.o speck.o
	$(CC) -o text text.o bf.o ht.o parser.o bv.o node.o speck.o -lm

pq: pq.o
	$(CC) -o pq pq.o

identify: identify.o pq.o text.o bf.o ht.o parser.o bv.o node.o speck.o
	$(CC) -o identify identify.o pq.o text.o bf.o ht.o parser.o bv.o node.o speck.o 

node.o: node.c node.h
	$(CC) $(CFLAGS) -c node.c

speck.o: speck.c 
	$(CC) $(CFLAGS) -c speck.c

ht.o: ht.c ht.h node.h salts.h speck.h
	$(CC) $(CFLAGS) -c ht.c 

bv.o: bv.c bv.h
	$(CC) $(CFLAGS) -c bv.c 

bf.o: bf.c bf.h bv.h salts.h speck.h
	$(CC) $(CFLAGS) -c bf.c

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

text.o: text.c text.h bf.h ht.h metric.h parser.h bv.h node.h speck.h
	$(CC) $(CFLAGS) -c text.c

pq.o: pq.c pq.h
	$(CC) $(CFLAGS) -c pq.c

identify.o: identify.c pq.h text.h bf.h ht.h metric.h parser.h bv.h node.h speck.h
	$(CC) $(CFLAGS) -c identify.c

clean:
	rm -f identify
	rm -f *.o

format:
	clang-format -i -style=file *.h
	clang-format -i -style=file *.c

