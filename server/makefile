
all: ./list.o
	gcc ./main.c ./list.o -o app -g

list.o: ./list/list.c
	gcc ./list/list.c -c -o ./list.o -g

cl:
	rm app *.o