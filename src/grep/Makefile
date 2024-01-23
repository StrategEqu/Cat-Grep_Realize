CC			:= gcc
NAME		:= s21_grep
SH_NAME		:= test.sh
SRC			:= $(NAME).c
HEADER		:= $(NAME).h

COLOR_BOLD := \033[1m
COLOR_RESET := \033[0m
COLOR_YELLOW := \033[33m
COLOR_MAGENTA := \033[35m
COLOR_CYAN := \033[36m

MODE?=1
#LFLAGS - содержит флаги оптимизации для компоновщика
#CFLAGS - содержит флаги оптимизации для компилятора
ifeq ($(MODE), 2)
	LFLAGS=-g -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector -fanalyzer
	CFLAGS=-Wall -Werror -Wextra -g -O2 -pedantic -std=c11 -Wshadow -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-align -Wfloat-equal -Wconversion -Wcast-qual -Wformat=2 -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=undefined -fno-sanitize-recover -fstack-protector -fanalyzer
else
	LFLAGS= 
	CFLAGS=-Wall -Werror -Wextra -g -std=c11 -pedantic
endif

all: $(NAME) test check check_script style leak

info:
	@echo "$(COLOR_BOLD)$(COLOR_YELLOW)Available make targets:$(COLOR_RESET)"
	@echo "$(COLOR_MAGENTA)$(NAME):\t\t$(COLOR_CYAN)create the executable file $(NAME)$(COLOR_RESET)"
	@echo "$(COLOR_MAGENTA)MODE=2:$(COLOR_CYAN)\t\t\tactivate full scan mode$(COLOR_RESET)"
	@echo "$(COLOR_MAGENTA)leak:$(COLOR_CYAN)\t\t\tmemory leak check$(COLOR_RESET)"
	@echo "$(COLOR_MAGENTA)style:$(COLOR_CYAN)\t\t\tstyle analysis$(COLOR_RESET)"
	@echo "$(COLOR_MAGENTA)check:$(COLOR_CYAN)\t\t\tterror analysis$(COLOR_RESET)"
	@echo "$(COLOR_MAGENTA)check_script:$(COLOR_CYAN)\t\tformatting and improving the script$(COLOR_RESET)"
	@echo "$(COLOR_MAGENTA)test:$(COLOR_CYAN)\t\t\trun the functional test of the program$(COLOR_RESET)"
	@echo "$(COLOR_MAGENTA)clean:$(COLOR_CYAN)\t\t\tremove all generated files$(COLOR_RESET)"

s21_grep: $(SRC) $(HEADER)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)		

check: $(NAME)
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h ./../common/*.*
leak: $(NAME)
	valgrind --tool=memcheck --leak-check=yes ./$(NAME)
style: $(NAME)
	clang-format -style=Google -n *.c *.h
test: $(NAME)
	bash test.sh
check_script:
	shfmt -w $(SH_NAME)
	shellcheck $(SH_NAME)	
clean:
	rm -rf *.o *.a
	rm -f $(NAME) *.o

rebuild: clean $(NAME)	