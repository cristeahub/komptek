BINDIR := bin/

all: compile

compile:
	test -d $(BINDIR) || mkdir $(BINDIR)
	gcc -std=c99 -o bin/a.out ps1.c

debug:
	gcc -std=c99 -g ps1.c

.PHONY:run
run:
	./bin/a.out

.PHONY:clean
clean:
	rm *.o
