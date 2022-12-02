All:main.o custom_client.o
	mv $^ ../../../obj/client/custom_client
main.o:main.c
	gcc -c -g -Wall $^ -o $@ -I ../../../include
custom_client.o:custom_client.c
	gcc -c -g -Wall $^ -o $@ -I ../../../include
