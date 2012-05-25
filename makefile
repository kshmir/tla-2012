tp1 : tp
	gcc -g -m32 -o  salida target/tp.o target/lex.o \
	 target/cstring.o target/map.o target/list.o target/includes.o \
	 target/tree.o target/grammar.o target/automatha.o
tp : clean
	mkdir target
	flex --outfile=src/lex.gen.c src/parser.l
	gcc -g -m32 -o target/lex.o -c src/lex.gen.c 
	gcc -g -m32 -o target/grammar.o -c src/lib/grammar.c 
	gcc -g -m32 -o target/automatha.o -c src/lib/automatha.c 
	gcc -g -m32 -o target/tp.o -c src/tp1.c 
	gcc -g -m32 -o target/cstring.o -c src/utils/cstring.c 
	gcc -g -m32 -o target/map.o -c src/utils/map.c 
	gcc -g -m32 -o target/queue.o -c src/utils/queue.c 
	gcc -g -m32 -o target/list.o -c src/utils/list.c 
	gcc -g -m32 -o target/includes.o -c src/includes.c 
	gcc -g -m32 -o target/tree.o -c src/utils/tree.c
clean:
	rm -rf target
	rm -f src/lex.gen.c
all : tp
	gcc -g -m32 -o salida target/lex.o target/tp.o target/cstring.o \
	target/map.o target/list.o target/includes.o target/tree.o \
	target/grammar.o target/automatha.o
