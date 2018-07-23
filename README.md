## MIPS Assembler
Simple tool which takes MIPS assemly as input and prints output similar to objdump in format:

| address | compiled hex | original instruction |
| ---------- | ----------------- | ------------------------ |
|.text|--|--|
|main:|
|00400000|3c01000a|	lui		$1 10|
|00400004|200c000c|	addi		$t4, $zero, 12
|00400008|27ad000a|	addiu		$t5, $sp, 10
|0040000c|2810d8f0|	slti		$s0, $zero, -1000
|...|...|...|
|.data|
|10010000|48454c4c4f20574f524c44215c6e00|.asciiz "HELLO WORLD!\n"


It's written in c++11 because I am too dumb to program in LISP (there are some mistakes in parser)

I have included some tests in `tests` directory
You can ran it `./mipsasm tests/test1.in`

I did it because I wanted to learn MIPS(also because my uni forced mi to do it :wq)
