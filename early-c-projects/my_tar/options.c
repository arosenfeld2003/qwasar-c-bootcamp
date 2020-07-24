#include "my_tar.h"

/*
  For simplicity, just accept letter options after '-' character.
  This differs from the linux implementation:
  http://man7.org/linux/man-pages/man1/tar.1.html
*/

t_opts              *create_opts() {
  t_opts            *opts = malloc(sizeof(t_opts));

  opts->append = false; /* -r */
  opts->append_new = false; /* -u */
  opts->create = false; /* -c */
  opts->extract = false; /* -x */
  opts->list_contents = false; /* -t */

  return opts;
}

bool             is_option(char *argument) {
  if (argument[0] == '-') {
    return true;
  }
  return false;
}

bool              is_last_opt_filename(char *arg) {
  int len = strlen(arg);
  if (arg[0] == '-' && arg[len - 1] == 'f') {
    return true;
  }
  return false;
}

void              set_opts(t_opts *opts, int argc, char **argv) {
  char            option;
  /*
    argv[0] is always './my_tar'.
    options arg[0] is always '-'.
  */
  int             i = 1;
  int             j = 1;

  while (i < argc) {
    if (is_option(argv[i])) {

      while (argv[i][j]) {
        option = argv[i][j];
        switch(option) {
          case 'c':
            opts->create = true;
            break;
          case 'r':
            opts->append = true;
            break;
          case 't':
            opts->list_contents = true;
            break;
          case 'u':
            opts->append_new = true;
            break;
          case 'x':
            opts->extract = true;
            break;
          case 'f':
            opts->filename = true;
            break;
        }
        j++;
      }
    }
    i++;
  }
}

/*
  check for errors if incorrect options are used.
    1. -t and -c cannot both be used at once.
    2. -c and -x cannot both be used at once.
    3. -c and -r cannot both be used at once.
    4. -f must always be used (except with -t)
      ==> this is a slight difference from original.
*/

int error_check(t_opts *opts) {
  if (!opts->filename &&
      (opts->append || opts->append_new || opts->extract || opts->list_contents)) {
    printf("Error: always use -f for a filename.\n");
    EXIT_FAILURE;
  }
  if (opts->create && opts->list_contents) {
    printf("Error: cannot use -t and -c.\n");
    EXIT_FAILURE;
  }
  if (opts->create && opts->list_contents) {
    printf("Error: cannot use options -c and -t.\n");
    EXIT_FAILURE;
  }
  if (opts->create && opts->extract) {
    printf("Error: cannot use options -c and -x.\n");
    EXIT_FAILURE;
  }
  if (opts->create && opts->append) {
    printf("Error: cannot use options -c and -r.\n");
    EXIT_FAILURE;
  }
  if (!opts->create && !opts->append && !opts->append_new
      && !opts->list_contents && !opts->extract) {
    printf("Error: Must specify one of -c, -r, -t, -u, -x.\n");
  }
  return 0;
}

