#include "s21_cat.h"

int main(int argc, char* argv[]) {
  int init = 0;
  int flags_num = 0;
  for (int i = 1; i < argc; i++) {
    init = getFlags(argc, argv);
    if (init == -1) {
    } else if (init == '?') {
      exit(0);
    } else {
      flags_num++;
    }
  }
  int j = 0;
  for (j = 1 + flags_num; j < argc; j++) {
    open_file(&argv[j]);
  }
  return 0;
}

int getFlags(int argc, char* argv[]) {
  int option;
  option = getopt_long(argc, argv, "bnseEtTv", long_options, NULL);
  if (option != -1) {
    switch (option) {
      case 'b':
        Flags.b = 1;
        break;
      case 'e':
        Flags.e = 1;
        Flags.v = 1;
        break;
      case 'E':
        Flags.e = 1;
        break;
      case 'n':
        Flags.n = 1;
        break;
      case 's':
        Flags.s = 1;
        break;
      case 't':
        Flags.t = 1;
        Flags.v = 1;
        break;
      case 'T':
        Flags.t = 1;
        break;
      case '?':
        break;
    }
  }
  return option;
}

void open_file(char* argv[]) {
  FILE* file = NULL;
  file = fopen(*argv, "r");
  if (file == NULL) {
    printf("%s: No such file or directory\n", *argv);
  } else {
    print_file(file);
  }
}

void print_file(FILE* file) {
  int count_line = 1;
  int empty_lines = 0;
  int ch = '\0';
  int last = '\n';
  int counter_for_b = 0;
  while ((ch = fgetc(file)) != EOF) {
    if (Flags.s == 1 && ch == '\n') {
      if (empty_lines >= 1) {
        continue;
      }
      empty_lines++;
    } else {
      empty_lines = -1;
    }
    if ((Flags.b != 1 && Flags.n == 1 && last == '\n') ||
        (Flags.b == 1 && ch != '\n' && last == '\n')) {
      if (counter_for_b == 1) {
      } else {
        printf("%6d\t", count_line++);
        counter_for_b++;
      }
    }
    if (Flags.v == 1 && ch != '\t' && ch != '\n') {
      print_visible_character(ch);
      continue;
    }
    if (Flags.e == 1 && ch == '\n') {
      printf("%c", '$');
    }
    if (Flags.t == 1 && ch == '\t') {
      printf("^I");
      continue;
    }
    printf("%c", ch);
    last = ch;
    counter_for_b = 0;
  }
  fclose(file);
}

void print_visible_character(int ch) {
  if (ch >= 32) {
    if (ch < 127) {
      printf("%c", ch);
    } else if (ch == 127) {
      printf("^?");
    } else {
      printf("M-");
      if (ch >= 128 + 32) {
        if (ch < 128 + 127) {
          printf("%c", ch - 128);
        } else {
          printf("^?");
        }
      } else {
        printf("^");
        printf("%c", ch - 128 + 64);
      }
    }
  } else {
    printf("^%c", ch + 64);
  }
}