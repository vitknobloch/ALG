CFLAGS+= -pedantic -Wall -std=c99 -O3 -g
HW=hw01
ZIP=zip
ALL = types.h solver.h solver.c loader.h loader.c main.c

$(HW): $(ALL) 
	$(CC) $(CFLAGS) $(ALL) -o main

zip:
	$(ZIP) $(HW).zip $(ALL)

clean:
	$(RM) -f *.o
	$(RM) -f main
	$(RM) -f $(HW).zip
	$(RM) -f datapub/*.myout
	$(RM) -f datapub/*.myerr

.PHONY: clean zip