default: build

build:
	clang -Wall -o psh src/psh.c src/builtin.c

run: clean build
	./psh
	
clean:
	rm psh