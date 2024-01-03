 CC=gcc
 CFLAGS=-Wall -Werror -Wextra -std=c11
.PHONY: all clean rebuild

all: s21_cat
s21_cat: s21_cat.o catInit.o
	$(CC) $(CFLAGS) -o $@ $^

s21_cat.o: s21_cat.c catInit.h
	$(CC) $(CFLAGS) -c $<

catInit.o: catInit.c catInit.h
	$(CC) $(CFLAGS) -c $<

test: s21_cat
	bash test.sh
clean:
	rm -rf *.o *.a
	rm -f s21_cat *.o
check: s21_cat
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h ./../common/*.*
	valgrind --tool=memcheck --leak-check=yes ./s21_cat
	clang-format -n *.c *.h