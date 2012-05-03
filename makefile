tp1 : dependencies tp
	gcc -m32 -o salida tp.o cstring.o map.o list.o includes.o tree.o
tp : 
	gcc -m32 -o tp.o -c src/tp1.c 
dependencies : 
	gcc -m32 -o cstring.o -c src/utils/cstring.c 
	gcc -m32 -o map.o -c src/utils/map.c 
	gcc -m32 -o list.o -c src/utils/list.c 
	gcc -m32 -o includes.o -c src/includes.c 
	gcc -m32 -o tree.o -c src/utils/tree.c 