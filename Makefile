
text.o: text.c
	cc -c text.c `pkg-config --cflags gtk+-2.0`
main.o: main.c
	cc -c main.c `pkg-config --cflags gtk+-2.0`
text: text.o main.o
	cc -o text text.o main.o `pkg-config --libs gtk+-2.0`
clean:
	rm -f main.o text.o text *~
