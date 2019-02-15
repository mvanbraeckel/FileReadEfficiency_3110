# Mitchell Van Braeckel (mvanbrae@uoguelph.ca) 1002297
# 15/02/2019
# CIS*3110: Operating Systems A2 - System Call vs Function Call: File Reading Efficiency
# --> Compares C function calls and Unix system calls in regards to efficiency of reading a file
 
CC = gcc
CFLAGS = -g -Wall -std=c99

all: unixio

unixio: unixio.c
	$(CC) $(CFLAGS) unixio.c -o unixio

git: *.c *.sh Makefile 
	git add Makefile
	git add *.pdf
	git add *.txt
	git add *.c
	git add *.sh
	git commit -m "automatic backup via makefile"
	git remote rm origin
	git config credential.helper store
	git config --global credential.helper 'cache --timeout 3600'
	git remote add origin https://github.com/mvanbraeckel/FileReadEfficiency_3110.git
	git push -u origin master

clean:
	rm -f *.o unixio
