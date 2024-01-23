#include "s21_grep.h"

int main(int argc, char **argv) {
  int error_code = NO_MATCHES_FOUND;

  if (argc > 2) {
    Options options = {0};
    char pattern[SIZE] = {0};
    int options_sh = 0;
    char *options_str = "lnvoice:f:sh?";

    while ((options_sh = getopt_long(argc, argv, options_str, 0, NULL)) != -1) {
      error_code = init_struct(&options, options_sh, pattern);
    }
    if ((options.e || options.f) & (argc < 4)) {
      error_code = ERROR;
    }
    if (options.l && options.c) {
      error_code = ERROR;
    }
    if (options.v && options.o) {
      error_code = ERROR;
    }
    if (error_code != ERROR) {
      performer((const char **)argv, pattern, &options);
    }
  }
  return error_code;
}

int init_struct(Options *options, int symbol, char *pattern) {
  int error_code = OK;

  switch (symbol) {
    case 'v':
      options->v = SET;
      break;
    case 'i':
      options->i = SET;
      break;
    case 'o':
      options->o = SET;
      break;
    case 'l':
      options->l = SET;
      break;
    case 'n':
      options->n = SET;
      break;
    case 'c':
      options->c = SET;
      break;
    case 'e':
      options->e = SET;
      init_pattern(pattern, optarg);
      break;
    case 'f':
      options->f = SET;
      error_code = f_handler(pattern);
      break;
    case 's':
      options->s = SET;
      break;
    case 'h':
      options->h = SET;
      break;
    case '?':
      error_code = ERROR;
  }
  return error_code;
}

int performer(const char **argv, const char *pattern, Options const *options) {
  int error_code = NO_MATCHES_FOUND;
  int num_files = 0;
  int flag_no_pattern_opt = CLEAR;

  if (options->e || options->f) {
    flag_no_pattern_opt = CLEAR;
    num_files = file_counter(argv, flag_no_pattern_opt);
  } else {
    flag_no_pattern_opt = SET;
    pattern = argv[optind];
    num_files = file_counter(argv, flag_no_pattern_opt);
  }

  error_code =
      file_handler(argv, pattern, num_files, flag_no_pattern_opt, options);

  return error_code;
}

int file_counter(const char **argv, int flag_no_pattern_opt) {
  int num_files = 0;
  int ind = optind;

  if (flag_no_pattern_opt) {
    ind += 1;
  }
  for (int i = ind; NULL != argv[i]; ++i) {
    if (argv[i][0] != '-') {
      num_files += 1;
    }
  }
  return num_files;
}

int file_handler(const char **argv, const char *pattern, int num_files,
                 int flag_no_pattern_opt, Options const *options) {
  int error_code = NO_MATCHES_FOUND;

  for (int searching_files = 0; searching_files < num_files;
       ++searching_files) {
    FILE *file_ptr;
    int ind_file_arg = optind + searching_files + flag_no_pattern_opt;
    const char *file_name = argv[ind_file_arg];

    if (NULL == (file_ptr = fopen(file_name, "r"))) {
      if (options->s) {
        fprintf(stderr, "s21_grep: %s: %s\n", file_name, strerror(2));
        error_code = ERROR;
      }
    } else {
      char opt_l_handling_is = CLEAR;
      unsigned int num_matching_strings = 0;
      regex_t preg;
      //если флаг i установлен, используем REG_ICASE для игнорирования регистра,
      //иначе REG_EXTENDED для расширенного режима компиляции регулярки
      int regcode = options->i ? regcomp(&preg, pattern, REG_ICASE)
                               : regcomp(&preg, pattern, REG_EXTENDED);
      if (regcode != OK) {
        char reg_errbuf[REG_ERROR_BUF_SIZE] = {0};
        regerror(regcode, &preg, reg_errbuf, REG_ERROR_BUF_SIZE);
        fprintf(stderr, "Regexp compilation failed: '%s'\n", reg_errbuf);
        error_code = ERROR;
      }
      if (error_code != ERROR) {
        char buf_str[SIZE] = {0};
        for (int num_str = 1; NULL != fgets(buf_str, SIZE, file_ptr);
             ++num_str) {
          if ((!options->v && (regexec(&preg, buf_str, 0, NULL, 0) == OK)) ||
              (options->v && (regexec(&preg, buf_str, 0, NULL, 0) != OK))) {
            if (options->c) {
              options->l ? num_matching_strings = 1 : ++num_matching_strings;
            }
            if (options->l) {
              opt_l_handling_is = SET;
            } else {
              error_code = opt_handler(file_name, num_files, num_str, buf_str,
                                       pattern, options);
            }
          }
        }
      }

      if (options->c) {
        error_code =
            c_handler(options, num_files, file_name, num_matching_strings);
      }
      if (opt_l_handling_is == SET) {
        printf("%s\n", file_name);
        error_code = OK;
      }
      regfree(&preg);
      fclose(file_ptr);
    }
  }
  return error_code;
}

int opt_handler(const char *file_name, int num_files, int num_str,
                char *buf_str, const char *pattern, Options const *options) {
  int error_code = NO_MATCHES_FOUND;

  if (!options->c) {
    if (num_files > 1 && !options->h) {
      printf("%s:", file_name);
    }
    if (options->n) {
      printf("%d:", num_str);
    }

    if (options->o && !options->v) {
      error_code = o_handler(options, buf_str, pattern);
    } else {
      fputs(buf_str, stdout);
      error_code = OK;
    }

    if (!options->o) {
      size_t n = strlen(buf_str);
      if (buf_str[n] == '\0' && buf_str[n - 1] != '\n') {
        putchar('\n');
      }
    }
  }
  return error_code;
}

int c_handler(Options const *options, int num_files, const char *file_name,
              unsigned int num_matching_strings) {
  int error_code = NO_MATCHES_FOUND;

  if ((num_files > 1) && !options->h) {
    printf("%s:%u\n", file_name, num_matching_strings);
    error_code = OK;
  } else {
    printf("%u\n", num_matching_strings);
    error_code = OK;
  }

  return error_code;
}

int o_handler(Options const *options, char *buf_str, const char *pattern) {
  int error_code = NO_MATCHES_FOUND;
  regex_t preg;
  int regcode = options->i ? regcomp(&preg, pattern, REG_ICASE)
                           : regcomp(&preg, pattern, REG_EXTENDED);

  if (regcode != OK) {
    char reg_errbuf[REG_ERROR_BUF_SIZE] = {0};

    regerror(regcode, &preg, reg_errbuf, REG_ERROR_BUF_SIZE);
    fprintf(stderr, "Regexp compilation failed: '%s'\n", reg_errbuf);
    error_code = ERROR;
  }

  if (regcode == OK && !options->v) {
    regmatch_t pmatch[SIZE];

    for (char *s = buf_str; *s != '\0';) {
      if (regexec(&preg, s, 1, pmatch, 0) != 0) {
        break;
      }
      printf("%.*s\n", (int)(pmatch->rm_eo - pmatch->rm_so), s + pmatch->rm_so);
      s += pmatch->rm_eo;
      if (pmatch->rm_eo == pmatch->rm_so) {
        ++s;  // Increment s if the match is zero-length to avoid an infinite
              // loop
      }
      error_code = OK;
    }
  }
  regfree(&preg);
  return error_code;
}
//Обрабатывает файлы, содержащие паттерны при запуске программы с опцией '-f' и
//заполняет pattern
int f_handler(char *pattern) {
  int error_code = NO_MATCHES_FOUND;

  FILE *file_pattern_pointer;
  const char *file_name_pattern = optarg;

  if (NULL == (file_pattern_pointer = fopen(file_name_pattern, "r"))) {
    fprintf(stderr, "s21_grep: %s: %s\n", file_name_pattern, strerror(2));
    error_code = ERROR;
  } else {
    char buf_str_pattern[SIZE] = {0};

    while (NULL != fgets(buf_str_pattern, SIZE, file_pattern_pointer)) {
      if ('\n' == *buf_str_pattern) {
        strcpy(pattern,
               ".*\0");  // Если есть пустая строка, то вывод всего
                         // содержимого.
      } else {
        buf_str_pattern[strlen(buf_str_pattern) - 1] =
            '\0';  // Затирание добавленного '\n' от функции fgets
        init_pattern(pattern, buf_str_pattern);
      }
    }
    fclose(file_pattern_pointer);
  }
  return error_code;
}

void init_pattern(char *pattern, const char *string) {
  // Если пустой шаблон, то программа выведет анализируемый текст полностью
  if (*string == '\0') {
    strcpy(pattern, ".*\0");
  }

  // Если паттерн пустой, записываем в него string
  if (0 == pattern[0]) {
    strcpy(pattern, string);

    // Eсли паттерн не "шаблон всего", то дописываем новый string через оператор
    // регулярного выражения "или"
  } else if (strcmp(pattern, ".*") != 0) {
    strcat(pattern, "|");
    strcat(pattern, string);
  }
}