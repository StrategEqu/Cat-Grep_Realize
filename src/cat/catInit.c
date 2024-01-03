#include "catInit.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 100

void initFlag(int argc, char *argv[]) {
  char *option = NULL;
  char *filenames[MAX_FILES] = {0};
  int filenameCount = 0;
  if (argc >= 2 && argv[1][0] == '-') {
    option = argv[1];
  }

  for (int i = 1; i < argc; i++) {
    if (argc >= 2 && argv[i][0] != '-') {
      filenames[filenameCount] = argv[i];
      filenameCount++;
    }
  }
  printFile(filenames, option, filenameCount);
}

void printFile(char **filenames, char *option, int filenameCount) {
  FILE *file = NULL;
  int ch = 0;
  int lineNum = 1;
  if (option == NULL) {
    for (int i = 0; i < filenameCount; i++) {
      file = fopen(filenames[i], "r");
      if (file == NULL) {
        return;
      }
      while ((ch = fgetc(file)) != EOF) {
        printf("%c", ch);
      }
      fclose(file);
    }
  } else {
    for (int i = 0; i < filenameCount; i++) {
      file = fopen(filenames[i], "r");
      if (file == NULL) {
        return;
      }
      flagSelection(file, option, &lineNum, ch);
      fclose(file);
    }
  }
}

void flagSelection(FILE *file, char *option, int *lineNum, int ch) {
  int check_v = 0;
  int check_t_or_e = 0;
  char line[1024];
  if (strcmp(option, "-n") == 0 || strcmp(option, "--number") == 0) {
    flag_n(file, line, lineNum);
  } else if (strcmp(option, "-b") == 0 ||
             strcmp(option, "--number-nonblank") == 0) {
    flag_b(file, line, lineNum);
  } else if (strcmp(option, "-s") == 0 ||
             strcmp(option, "--squeeze-blank") == 0) {
    flag_s(file, line);
  } else if (strcmp(option, "-E") == 0 || strcmp(option, "--show-ends") == 0) {
    flag_E(file, ch);
  } else if (strcmp(option, "-T") == 0 || strcmp(option, "--show-tabs") == 0) {
    flag_T(file, ch);
  } else if (strcmp(option, "-v") == 0 ||
             strcmp(option, "--show-nonprinting") == 0) {
    check_v = 1;
    flag_v(file, check_t_or_e, check_v, ch);
  } else if (strcmp(option, "-t") == 0) {
    check_t_or_e = 1;
    flag_v(file, check_t_or_e, check_v, ch);
  } else if (strcmp(option, "-e") == 0) {
    check_t_or_e = 0;
    flag_v(file, check_t_or_e, check_v, ch);
  } else {
    printf("Failed flags");
  }
}

void flag_n(FILE *file, char *line, int *lineNum) {
  while (fgets(line, 1024, file) != NULL) {
    printf("%6d\t%s", *lineNum, line);
    (*lineNum)++;
  }
}

void flag_b(FILE *file, char *line, int *lineNum) {
  while (fgets(line, 1024, file) != NULL) {
    if (line[0] != '\n') {
      printf("%6d\t%s", *lineNum, line);
      (*lineNum)++;
    } else {
      printf("%s", line);
    }
  }
}

void flag_s(FILE *file, char *line) {
  bool isFirstEmptyLine = true;
  while (fgets(line, 1024, file) != NULL) {
    if (line[0] == '\n') {
      if (isFirstEmptyLine) {
        printf("%s", line);
        isFirstEmptyLine = false;
      }
    } else {
      printf("%s", line);
      isFirstEmptyLine = true;
    }
  }
}

void flag_E(FILE *file, int ch) {
  while ((ch = fgetc(file)) != EOF) {
    if (ch == '\n') {
      putchar('$');
    }
    putchar(ch);
  }
}

void flag_T(FILE *file, int ch) {
  while ((ch = fgetc(file)) != EOF) {
    if (ch == '\t') {
      printf("^I");
    } else {
      putchar(ch);
    }
  }
}

void flag_v(FILE *file, int check_t_or_e, int check_v, int ch) {
  while ((ch = fgetc(file)) != EOF) {
    if (ch >= 32) {
      if (ch < 127)
        printf("%c", ch);
      else if (ch == 127) {
        printf("^?");
      } else {
        printf("M-");
        if (ch >= 128 + 32) {
          if (ch < 128 + 127)
            printf("%c", ch - 128);
          else {
            printf("^?");
          }
        } else {
          printf("^");
          printf("%c", ch - 128 + 64);
        }
      }
    } else if (ch == '\t' && !check_t_or_e)
      printf("\t");
    else if (ch == '\n') {
      if (!check_t_or_e && !check_v) {
        printf("$");
      }
      printf("\n");
    } else {
      printf("^%c", ch + 64);
    }
  }
}