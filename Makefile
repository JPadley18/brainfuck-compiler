all: release

debug:
	gcc -Wall -Werror -Wpedantic -g *.c -o bfc

release:
	gcc -Wall -Werror -Wpedantic -O2 -s -DNDEBUG *.c -o bfc

assemble:
	nasm -f elf32 -o test.o test.asm
	ld -m elf_i386 -o test test.o

assemble-debug:
	nasm -f elf32 -gdwarf -o test.o test.asm
	ld -m elf_i386 -o test test.o

run:
	./test
