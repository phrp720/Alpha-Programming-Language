# Alpha-Programming-Language
Alpha Programming Language based on Delta (https://projects.ics.forth.gr/hci/files/plang/Delta/Delta.html) implemented in C++. It was created as the main project of the undergraduate course CS-340 (Languages and Compilers, https://www.csd.uoc.gr/~hy340/).

The languge resembles a subset of Javascript, and runs on a virtual machine (the coding of which was the last part of the project).

The team consisted of three members (Phillip Papadakis https://github.com/phrp720 and  Yfantis Evaggelos https://github.com/octoper) and the work was more or less equally distributed. This repo is a public read-only copy of the original, created for showcasing examples of our work for CV purposes.


## How to compile and how to run it.

First you need to have all the appropriate tools installed

**gcc - GNU C++ Compiler** (version 9.3.0, C++11)

**flex - Fast Lexical Analyzer** (version 2.6.4)

**bison - The Yacc-compatible Parser Generator** (version 3.8.2)

### Compile the Compiler
You can compile the compiler by running

```bash
make compiler
```

## Run
You can run the compiler by running
```bash
./alpha_lexer <your_alpha_file.al> <output-file.txt>
```


### Compile the AVM
You can compile the avm by running

```bash
make avm
```

## Run
You can run the AVM by running
```bash
./avm <any_alpha_binary_file.abc>
```

The `alpha_lexer` can accept 2 parameters the first parameter is the input file for the lexer and the second is the output file.
You can skip the second parameter and everything will be passed to the standard input.
