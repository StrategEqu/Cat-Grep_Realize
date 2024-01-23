#include "s21_cat.h"

int main(int argc, char* argv[]) {
  initFlags(argc, argv);
  open_file(argv);
  return 0;
}

void initFlags(int argc, char** argv) {
  int options_sh;
  options_sh = getopt_long(argc, argv, "bnseEtTv", long_options, 0);
  for (int i = 0; i < argc; i++) {
    for (; options_sh != -1;
         options_sh = getopt_long(argc, argv, "bnseEtTv", long_options, 0)) {
      switch (options_sh) {
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
        case 'v':
          Flags.v = 1;
          break;
        case '?':
          break;
      }
    }
  }
}

void open_file(char* argv[]) {
  int count_line = 1;
  int last = '\n';
  for (int i = 1; argv[i] != NULL; i++) {
    if (argv[i][0] != '-' && argv[i][1] != '-') {
      FILE* file = fopen(argv[i], "r");
      if (file == NULL) {
        printf("%s: No such file or directory\n", argv[i]);
      } else {
        print_file(file, &count_line, &last);
      }
    }
  }
}

void print_file(FILE* file, int* count_line, int* last) {
  int counter_for_b = 0;
  int empty_lines = 0;
  int ch;
  while ((ch = fgetc(file)) != EOF) {
    if (Flags.s == 1 && ch == '\n') {
      if (empty_lines >= 1) {
        continue;
      }
      empty_lines++;
    } else {
      empty_lines = -1;
    }
    if ((Flags.b != 1 && Flags.n == 1 && *last == '\n') ||
        (Flags.b == 1 && ch != '\n' && *last == '\n')) {
      if (counter_for_b != 1) {
        printf("%6d\t", (*count_line)++);
        counter_for_b++;
      }
    }
    if (Flags.v == 1 && ch != '\t' && ch != '\n') {
      print_visible_character(ch);
    } else {
      if (Flags.e == 1 && ch == '\n') {
        printf("%c", '$');
      }
      if (Flags.t == 1 && ch == '\t') {
        printf("^I");
      } else {
        printf("%c", ch);
        *last = ch;
        counter_for_b = 0;
      }
    }
  }
  fclose(file);
}

void print_visible_character(int ch) {
  if (ch >= 32 && ch < 127) {
    printf("%c", ch);
  } else if (ch == 127) {
    printf("^?");
  } else if (ch >= 128 + 32 && ch < 128 + 127) {
    printf("M-%c", ch - 128);
  } else if (ch >= 128) {
    printf("M-^%c", ch - 128 + 64);
  } else {
    printf("^%c", ch + 64);
  }
}