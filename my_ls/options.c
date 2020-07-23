#include "my_list.h"

t_opts *get_opts(char *flags, t_opts *opts) {
  int i = 0;
  while (flags[i]) {
    switch(flags[i]) {
      case 'a':
        opts->include_hidden_files = 1;
        break;
      case 'R':
        opts->list_dirs_recursively = 1;
        break;
      case 't':
        opts->sort_by_time_modified = 1;
        break;
      default:
        printf("option %c invalid\n", flags[i]);
    }
    i++;
  }
  free(flags);
  return opts;
}

// initialize without any options.
t_opts *init_opts() {
  t_opts *opts = malloc(sizeof(t_opts));
  opts->include_hidden_files = 0; // -a
  opts->list_dirs_recursively = 0; // -R
  opts->sort_by_time_modified = 0; // -t
  return opts;
}

t_opts *create_opts(int argc, char **argv) {
  t_opts *opts = init_opts();
  char *flags = create_flags(argc, argv);
  return get_opts(flags, opts);
}
