#include <stdio.h>

#ifndef CATINIT_H
#define CATINIT_H

void initFlag(int argc, char *argv[]);
void flagSelection(FILE *file, char *option, int *lineNum, int ch);
void printFile(char **filenames, char *option, int filenameCount);
void flag_n(FILE *file, char *line, int *lineNum);
void flag_b(FILE *file, char *line, int *lineNum);
void flag_s(FILE *file, char *line);
void flag_E(FILE *file, int ch);
void flag_T(FILE *file, int ch);
void flag_v(FILE *file, int check_t_or_e, int check_v, int ch);

#endif