/*
**
*/

#include "my_tar.h"

/*
**
*/

/* -x option: extract */
/*
  variables: archive name, [filenames]
    if no filenames, all files in archive are extracted.

  default behavior: overwrite any file in the directory with same name.
*/
void extract(char *archive_name, char **filenames, t_opts *opts) {
  /*
    BASIC PRINCIPLES:
    - Use the size from each file's header to navigate the tar archive.
    - Create appropriate files in the current directory using filesize and filenames.
      - if no filenames, create a file for every file in tar archive.
      - if filenames, search tar archive and only create files for a match.
  */

  char *current_file;
  FILE *fp_tar;
  FILE *fp_extracted;
  t_header *buf_head = malloc(sizeof(t_header));
  char *buf_file;
  size_t read_head;
  size_t read_file;
  unsigned long file_size;
  unsigned long tar_file_size;

  // if (!filenames) {
  /* extract entire archive */

  /* handle -t, list contents */
  if (opts->list_contents) {
    fp_tar = fopen(archive_name, "r");

    /*
      read_head = fread(buf_head, (sizeof(t_header)), 1, fp_tar);
      file_size = strtoul(buf_head->size, NULL, 10);
      current_file = buf_head->name;
      printf("%s\n", current_file);
    */

    /* get length of tarfile */
    fseek(fp_tar, 0, SEEK_END);
    tar_file_size = ftell(fp_tar);
    rewind(fp_tar);

    while (ftell(fp_tar) < tar_file_size) {
      read_head = fread(buf_head, (sizeof(t_header)), 1, fp_tar);
      file_size = strtoul(buf_head->size, NULL, 10);
      current_file = buf_head->name;
      printf("%s\n", current_file);
      fseek(fp_tar, file_size, SEEK_CUR);
    }

    // free buffers and close file
    free(buf_head);
    fclose (fp_tar);
  }

  int i = 0;
  while (filenames[i]) {

    char *filename = filenames[i];

    /* extract file */
    fp_tar = fopen(archive_name, "r");

    /* get header values for file */
    read_head = fread(buf_head, (sizeof(t_header)), 1, fp_tar);
    file_size = strtoul(buf_head->size, NULL, 10);
    current_file = buf_head->name;

    if (opts->list_contents) {

    }

    /*
      - If filename matches buf_head->name this is the file we want to extract.
      - If it doesn't match, skip over the file to the next header and check again.
      - Repeat until we've found a match or we reach EOF.
    */

    while (strcmp(filename, current_file) != 0) {
      fseek(fp_tar, file_size, SEEK_CUR);

      if (feof(fp_tar)) {
        printf("Error: file not found in archive.");
        EXIT_FAILURE;
      }

      read_head = fread(buf_head, (sizeof(t_header)), 1, fp_tar);
      file_size = strtoul(buf_head->size, NULL, 10);
      current_file = buf_head->name;
    }

    fp_extracted = fopen(filenames[i], "w");

    buf_file = malloc(file_size);
    read_file = fread(buf_file, (file_size + 1), 1, fp_tar);
    fwrite(buf_file, file_size, 1, fp_extracted);

    // free buffers and close file
    free(buf_head);
    free(buf_file);
    fclose (fp_tar);

    i++;
  }
}