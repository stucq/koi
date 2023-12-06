CC=clang
CFLAGS=-g

.PHONY: clean

bin/koi: bin obj/main.o obj/die.o obj/lexer.o obj/parser.o obj/value.o
	$(CC) $(CFLAGS) obj/main.o obj/die.o obj/lexer.o obj/parser.o obj/value.o -o bin/koi

bin:
	mkdir -p bin

obj/main.o: obj src/main.c
	$(CC) $(CFLAGS) src/main.c -c -o obj/main.o

obj/die.o: obj src/die.c
	$(CC) $(CFLAGS) src/die.c -c -o obj/die.o

obj/lexer.o: obj src/lexer.c
	$(CC) $(CFLAGS) src/lexer.c -c -o obj/lexer.o

obj/parser.o: obj src/parser.c
	$(CC) $(CFLAGS) src/parser.c -c -o obj/parser.o

obj/value.o: obj src/value.c
	$(CC) $(CFLAGS) src/value.c -c -o obj/value.o

obj:
	mkdir -p obj

clean:
	rm -rf bin
	rm -rf obj
