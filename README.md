# Brainfuck Compiler
An extremely overkill Brainfuck compiler. Currently supports Linux.

## Requirements
To build the compiler executable:
- `make`
- `gcc`

To use the compiler:
- `nasm`
- `ld`

## Quick Start
To start compiling your Brainfuck, first make sure you have access to all the commands listed in `requirements`, then run
```
make
```
to build the `bfc` executable.

Compiling Brainfuck is simple, with `bfc` in the same directory as your source code, run
```
./bfc your-code.bf -o your-executable
```
to compile `your-code.bf` into an executable called `your-executable`. You can then run the executable with `./your-executable`.

## Fun Extras
If you would like to see the intermediate files (e.g. assembly and object files) generated by the program, add the `-s` flag to the `./bfc` command. The program generates a `your-executable.bfc.asm` assembly output file and assembles it into a `your-executable.bfc.o` object file. These are normally deleted once compilation is complete, but the `-s` option will prevent this from happening.

## How It Works
As mentioned above, this compiler is extreme overkill for Brainfuck as the language is supposed to have a very small compiler or interpreter. However, that hasn't stopped me from squeezing every bit of performance out of Brainfuck as I can, just for fun. The compilation process is broken down into the following steps:
1. **Minification**: The source code is stripped of all non-brainfuck characters.
2. **Compression**: The minified code is compressed using run-length encoding. This encodes sequences of repeated Brainfuck instructions into the form `[count][instruction]`. For example, `>>>[+++++]` is encoded into `3>[5+]`. This means that the generated assembly code can be a fraction of the size it would otherwise be, as these repeated sequences of characters can be combined into a single intruction, instead of many repetitions of the same one.
3. **Translation**: The minified and compressed code is translated into NASM assembly.
4. **Assembly**: NASM is called to assemble the assembly into an object file.
5. **Linking**: The last step of compilation: `ld` is called to link the object file into an executable.

In my testing, compiled Brainfuck runs up to 30x faster than code I interpret using my [Brainfuck Interpreter](https://github.com/JPadley18/brainfuck-interpreter) project, which this compiler is loosely based on.