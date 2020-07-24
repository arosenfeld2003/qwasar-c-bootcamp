/*
**
*/

#include "my_tar.h"

/*
**
*/


int         main(int argc, char **argv) {
  t_opts     *opts = create_opts(); /* malloc - needs free */
  char       **filenames = set_filenames(argc, argv); /* malloc - needs free */
  char       *tarfile;

  set_opts(opts, argc, argv);

  if (opts->filename) {
    /* f option will always immediately precede the name of tar archive file */
    int tarfile_arg = get_tarfile_arg(argv);
    if (tarfile_arg > 0) {
      tarfile = argv[tarfile_arg];
    }
  }

  error_check(opts);

  if (opts->create && opts->filename) {
    create_tar_file(tarfile, filenames);
  }

  if (opts->extract || opts->list_contents) {
    extract(tarfile, filenames, opts);
  }

  // free(filenames);
  free(opts);
  return 0;
}