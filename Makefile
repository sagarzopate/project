try: func.o te.o
	cc func.o te.o -lgraph -o try
func.o: func.c
	cc -Wall -c func.c
te.o: te.c
	cc -Wall -c te.c
