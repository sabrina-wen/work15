all: client.c control.c
	gcc -o client client.c
	gcc -o control control.c

clean: all
	rm control
	rm client
