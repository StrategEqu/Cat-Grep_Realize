CC			:= gcc
NAME		:= s21_cat
SRC			:= $(NAME).c
HEADER		:= $(NAME).h

#LFLAGS - содержит флаги оптимизации для компоновщика (линкера)
#CFLAGS - содержит флаги оптимизации для компилятора
MODE?=1

ifeq ($(MODE), 2)
	LFLAGS=-g -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector -fanalyzer
	CFLAGS=-Wall -Werror -Wextra -g -O2 -pedantic -std=c11 -Wshadow -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-align -Wfloat-equal -Wconversion -Wcast-qual -Wformat=2 -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector -fanalyzer
else
	LFLAGS= 
	CFLAGS=-Wall -Werror -Wextra -g -std=c11 -pedantic
endif

all: $(NAME)

info:
		@echo "Available make targets:"
		@echo "  $(NAME):  create the executable file $(NAME)"
		@echo "  leak:     memory leak check"
		@echo "  check:    error analysis"
		@echo "  style:    style analysis"
		@echo "  clean:    remove all generated files"
		@echo "  test:     run the functional test of the programm"

s21_cat: $(SRC) $(HEADER)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)		

check: $(NAME)
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h ./../common/*.*
leak: $(NAME)
	valgrind --tool=memcheck --leak-check=yes ./$(NAME)
style: $(NAME)
	clang-format -n *.c *.h
test: $(NAME)
	bash test.sh
clean:
	rm -rf *.o *.a
	rm -f $(NAME) *.o
	
rebuild: clean $(NAME)	