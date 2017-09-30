CC = cc

.PHONY: clean

spellchk1: spellchk
	${CC} -o$@ spellchecker.o -std=gnu99 -g

spellchk: spellchecker.c
	${CC} -c spellchecker.c -std=gnu99 -g

clean:
	rm *.o spellchk1

