project: text.o main.o
	cc -o project text.o main.o `pkg-config --libs gtk+-2.0`
text.o: text.c text.h
	cc -c text.c `pkg-config --cflags gtk+-2.0`
main.o: main.c text.h
	cc -c main.c `pkg-config --cflags gtk+-2.0`
clean:
	rm -f main.o text.o text *~
