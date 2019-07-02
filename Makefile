CC ?= /usr/bin/clang
CFLAGS ?= -I/usr/local/opt/readline/include
LDFLAGS ?= -L/usr/local/opt/readline/lib
LIBS = -lreadline

default: psh

psh: src/psh.c src/builtin.c
	$(CC) -Wall $(CFLAGS) $(LDFLAGS) $(LIBS) -o $@ $?

run: clean psh
	./psh
	
clean:
	rm -f ./psh