#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef CATINIT_H
#define CATINIT_H

struct flag {
  short int b;
  short int e;
  short int n;
  short int s;
  short int t;
  short int v;
} Flags = {0};

const struct option long_options[] = {
    {"number-nonblank", no_argument, NULL, 'b'},
    {"number", no_argument, NULL, 'n'},
    {"squeeze-blank", no_argument, NULL, 's'},
    {"show-ends", no_argument, NULL, 'E'},
    {"show-tabs", no_argument, NULL, 'T'},
    {NULL, 0, NULL, 0}};

void initFlags(int argc, char** argv);
void open_file(char* argv[]);
void print_file(FILE* file, int* count_line, int* last);
void print_visible_character(int ch);

#endif