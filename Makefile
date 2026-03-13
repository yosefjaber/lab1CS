#
# Makefile for Lab1
# Note: requires a 64-bit x86-64 system 
#
CC = gcc
CFLAGS = -g -Wall -Werror -std=c99 -m64

all: csim
	# Generate a handin tar file each time you compile
	-tar -cvf lab1-handin.tar  csim.c 

csim: csim.c lab1.c lab1.h
	$(CC) $(CFLAGS) -o csim csim.c lab1.c -lm 

#
# Clean the src dirctory
#
clean:
	rm -rf *.o
	rm -f *.tar
	rm -f csim
	rm -f trace.all trace.f*
	rm -f .csim_results .marker
