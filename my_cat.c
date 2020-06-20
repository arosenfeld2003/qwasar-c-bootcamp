#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

/*
  Basic implementation of Linux cat method.
  Uses open, read, write, close.
  Does not handle options.
*/

int main (int argc, char **argv) {
  int file, ch;

  // handle multiple filenames passed in.
  if (argc >= 2) {
    int i = 1;
    while (argv[i]) {
      file = open(argv[i], O_RDONLY);
      if (file < 0) {
        errno;
        return -1;
      }
      while (read(file, &ch, 1)) {
        write(STDOUT_FILENO, &ch, 1);
      }
      close(file);
      i++;
    }
  } else {
    // handle pipe input.
    while (read(STDIN_FILENO, &ch, 1) > 0) {
      write(STDOUT_FILENO, &ch, 1);
    }
  }
  return 0;
}