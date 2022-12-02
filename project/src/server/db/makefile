All:db.o sqlite3.o
	mv $^ ../../../obj/server
db.o:db.c
	gcc -c -g -Wall $^ -o $@ -I ../../../include
sqlite3.o:sqlite3.c
	gcc -c -g -Wall $^ -o $@
