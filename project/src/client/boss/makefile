All:main.o boss_client.o
	mv $^ ../../../obj/client/boss_client
main.o:main.c
	gcc -c -g -Wall $^ -o $@ -I ../../../include
boss_client.o:boss_client.c
	gcc -c -g -Wall $^ -o $@ -ldl -lpthread -I ../../../include
