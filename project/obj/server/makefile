serverapp:db.o main.o network.o sqlite3.o
	gcc $^ -o $@ -ldl -lpthread
	mv $@ ../../bin
