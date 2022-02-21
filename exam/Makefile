CFLAGS+= -pedantic -Wall -std=c99 -O3 -g
HW=main
ZIP=zip
OBJ=$(patsubst %.c,%.o, $(wildcard *.c))

$(HW): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(HW)

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $< -o $@

main.o: main.c
	$(CC) -c $(CFLAGS) $< -o $@

zip:
	$(ZIP) $(HW).zip *.c *.h

clean:
	$(RM) -f *.o
	$(RM) -f $(HW)
	$(RM) -f $(HW).zip
	$(RM) -f datapub/*.myout
	$(RM) -f datapub/*.myerr
	$(RM) -f vgcore.*

r run:
	./test.sh

.PHONY: clean zip