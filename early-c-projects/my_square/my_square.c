#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// first line & last line
void print_first_or_last_line(int x, int y) {

  int dashes = (x == 1 || x == 2) ? 0 : x - 2;
  char corner = 'o';
  char dash = '-';
  char line = '|';
  int i = 0;

  if (x > 0 || y > 0) {
    printf("%c", corner);
    for (i = 0; i < dashes; i++) {
      printf("%c", dash);
    }
    if (x > 1) {
      printf("%c", corner);
    }
  }
}

void print_middle(int x, int y, int newLines) {
  int spaces = (x == 1 || x == 2) ? 0 : x - 2;
  char line = '|';
  char space = ' ';
  int i;

  while (newLines > 0) {
    printf("%c", line);
    for (i = 0; i < spaces; i++) {
      printf("%c", space);
    }
    if (x > 1) {
      printf("%c", line);
    }

    printf("\n");
    newLines--;
  }
}

int main(int ac, char **av) {
  int x = atoi(av[1]);
  int y = atoi(av[2]);

  if (x == 0 || y == 0) {
    return 0;
  }

  if (x == 1 && y == 1) {
    printf("o\n");
    return 0;
  }

  int newLines = y - 1;

  print_first_or_last_line(x, y);
  if (newLines > 0) {
    printf("\n");
    newLines--;
  }

  print_middle(x, y, newLines);

  if (y > 1) {
    print_first_or_last_line(x, y);
  }

  printf("\n");
}
