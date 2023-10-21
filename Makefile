all: release

debug:
	gcc -Wall -Werror -Wpedantic -g *.c -o bf

release:
	gcc -Wall -Werror -Wpedantic -O2 -s -DNDEBUG *.c -o bf
