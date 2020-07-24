#include "my_tar.h"

/*
  Parse all of the arguments.
  args[0] ==> always the program call ./my_tar.
  any args starting with '-' ==> option arg.
  any option arg ending in 'f' ==> next arg is the tarfile name.
  any other args ==> files (if they exist) to be added or extracted from tar archive.
*/

t_opts          *pass_in_options(int argc, char **args, t_opts *opts) {
  opts = create_opts(args, 1);
  return opts;
}

bool             is_filename(char *arg, char *preceding_arg) {
  if (!is_option(arg) && !is_last_opt_filename(preceding_arg)) {
    return true;
  }
  return false;
}


int            get_tarfile_arg(char **argv) {
  /* tarfile name will always come after call to tar and options */
  int i = 2;
  while (argv[i]) {
    if (!is_option(argv[i]) && is_last_opt_filename(argv[i - 1])) {
      return i;
    }
    i++;
  }
  return 0;
}


/* pass in array of all args after the tar filename */
char            **set_filenames(int argc, char **args) {
  char **filenames = malloc(sizeof(char *) * argc);
  int i = 1;
  int j = 0;
  while (args[i]) {
    if (is_filename(args[i], args[i - 1])) {
      filenames[j] = args[i];
      j++;
    }
    i++;
  }
  return filenames;
}



