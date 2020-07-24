#include "readline.h"

int main(int ac, char **av) {
  char *str = NULL;
  int fd = open("./file.txt", O_RDONLY);
  while ((str = my_readline(fd)) != NULL) {
    printf("%s\n", str);
    free(str);
  }
  close(fd);
  //
  //  Yes it's also working with stdin :-)
  //  printf("%s", my_readline(0));
  //
  return 0;
}