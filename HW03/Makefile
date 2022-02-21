CFLAGS+= -pedantic -Wall -std=c99 -O3 -g
HW=hw03
ZIP=zip
ALL = loader.h loader.c main.c

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