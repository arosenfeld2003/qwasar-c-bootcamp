#include "my_list.h"

int main(int argc, char **argv) {
  t_opts *opts = create_opts(argc, argv);
  t_list *file_list = make_user_filelist(argc, argv);
  t_list *sorted = sort_with_options(file_list, opts);
  print_list(sorted, opts);
  // recurse to print directories
  recurse(sorted, opts);
  destroy_list(&file_list);
  free(opts);
  return 0;
}