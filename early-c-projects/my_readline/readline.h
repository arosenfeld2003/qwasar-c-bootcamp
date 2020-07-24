#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>


/*
  **
*/

char               *my_readline(int fd);
char               *create_temp(char *temp, char *str, char *overflow,
                                size_t MAX_SIZE, bool should_return);