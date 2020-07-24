/*
**
*/

#include "my_tar.h"

/*
**
*/

/* create an empty list */
t_list          *create_list() {
  t_list        *new_list = malloc(sizeof(t_list)); /* malloc - needs free */
  new_list->filename = malloc(sizeof(char *));
  new_list->next = NULL;
  return new_list;
}

t_list *create_file_list(char **filenames, t_list *file_list) {
  t_list                *head = file_list;
  int i = 0;

  while(filenames[i]) {
    if (file_list->filename == NULL) {
      /* empty list: add filename */
      file_list->filename = filenames[i];
      file_list->next = NULL;
    } else {
      /* existing list: append new node with filename and path */
      add_node(head, filenames[i]);
    }
    i++;
  }
  return head;
}

/*
  The posix header uses 257 bytes...
  then is padded with NUL bytes...
  to make it fill a 512 byte record.
*/

/* fill posix header */
struct posix_header *create_header(t_header *header, char *pathname) {
  struct stat           *buf_stat = malloc(sizeof(struct stat));
  snprintf(header->name, 100, "%s", pathname);

  stat(header->name, buf_stat);
  snprintf(header->mode, 8, "%o", buf_stat->st_mode);
  snprintf(header->size, 12, "%lld", buf_stat->st_size);
  snprintf(header->mtime, 12, "%ld", buf_stat->st_mtime);
  free(buf_stat);

  return header;
}

void write_header(FILE *fp_tar, t_list *file_list, t_header *header, char *filename, char *tar_file_name) {
  /* handle error */
  if (fp_tar == NULL) {
    fprintf(stderr, "\nError opening tar file\n");
    EXIT_FAILURE;
  }

  /* write data from posix_header into file. */
  fwrite(header, sizeof(t_header), 1, fp_tar);
}

void write_to_tar(FILE *fp_tar, FILE *fp_file, t_list *file_list,
                    t_header *header, char *filename, char *tar_file_name) {
  unsigned long file_size = strtoul(header->size, NULL, 10);
  char *buffer = malloc(file_size);

  fread(buffer, (file_size + 1), 1, fp_file);
  fwrite(buffer, file_size, 1, fp_tar);

  free(buffer);
}


/* -cf options */
void create_tar_file(char *tar_file_name, char **filenames) {
  t_list          *current;
  t_list          *file_list = create_list();

  file_list = create_file_list(filenames, file_list);
  current = file_list;

  while (current != NULL) {
    if (is_directory(current->filename)) {
      /* Placeholder for the current node of list */
      read_dir(current, file_list);
      /* Advance one node from the directory node that was read */
      current = current->next;
    } else {
      current = current->next;
    }
  }

  while (file_list != NULL) {
    char                  *filename = file_list->filename;
    struct posix_header   *header = malloc(sizeof(struct posix_header));

    header = create_header(header, filename);

    /* open filestream of the current file for reading */
    FILE *fp_file = fopen(filename, "r");
    /* Open (or create) filestream of tar file for writing at end of file */
    FILE *fp_tar = fopen(tar_file_name, "a");

    /* handle error */
    if (fp_tar == NULL) {
      fprintf(stderr, "\nError opening tar file\n");
      EXIT_FAILURE;
    }

    if (fp_file == NULL) {
      fprintf(stderr, "\nError opening %s\n", filename);
      EXIT_FAILURE;
    }

    /* write header into tarfile */
    write_header(fp_tar, file_list, header, filename, tar_file_name);

    /* write contents of file into tar file */
    write_to_tar(fp_tar, fp_file, file_list, header, filename, tar_file_name);

    fclose(fp_file);
    fclose(fp_tar);
    free(header);
    file_list = file_list->next;
  }
  /* delete-list method to free memory allocated for the list */
  free(file_list);
}