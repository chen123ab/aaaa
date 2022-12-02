All:main.o
	make -C network
	make -C db
main.o:main.c
	gcc -c -g -Wall $^ -o $@ -I../../include
	mv $@ ../../obj/server
