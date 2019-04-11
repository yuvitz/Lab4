all: flame2

flame2: util.o start.o task2c.o
	ld -m elf_i386 start.o task2c.o util.o -o flame2

util.o : util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o

task2c.o : task2c.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task2c.c -o task2c.o

start.o : start.s
	nasm -f elf start.s -o start.o

.PHONY: clean

clean:
	rm -f *.o flame2