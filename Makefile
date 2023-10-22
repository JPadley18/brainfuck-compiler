all: release

debug:
	gcc -Wall -Werror -Wpedantic -g *.c -o bfc

release:
	gcc -Wall -Werror -Wpedantic -O2 -s -DNDEBUG *.c -o bfc
