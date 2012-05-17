tp1 : tp
	gcc -m32 -lfl -o salida lex.o tp.o cstring.o map.o list.o includes.o tree.o grammar.o automatha.o
tp : 
	flex --outfile=src/lex.gen.c src/parser.l
	gcc -m32 -lfl -o lex.o -c src/lex.gen.c 
	gcc -m32 -o grammar.o -c src/lib/grammar.c 
	gcc -m32 -o automatha.o -c src/lib/automatha.c 
	gcc -m32 -o tp.o -c src/tp1.c 
	gcc -m32 -o cstring.o -c src/utils/cstring.c 
	gcc -m32 -o map.o -c src/utils/map.c 
	gcc -m32 -o queue.o -c src/utils/queue.c 
	gcc -m32 -o list.o -c src/utils/list.c 
	gcc -m32 -o includes.o -c src/includes.c 
	gcc -m32 -o tree.o -c src/utils/tree.c
clean:
	rm lex.o
	rm list.o 
	rm map.o
	rm queue.o
	rm tree.o 
	rm grammar.o
	rm automatha.o 
	rm src/lex.gen.c 
	rm cstring.o 
	rm tp.o
	rm includes.o
