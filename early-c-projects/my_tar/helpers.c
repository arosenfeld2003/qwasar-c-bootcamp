/*
**
*/

#include "my_tar.h"

/*
**
*/

/* TESTING: PRINT THE LIST */
t_list          *print_list(t_list *file_list) {
  t_list *current_node = file_list;
  while (current_node != NULL) {
    current_node = current_node->next;
  }
  return file_list;
}

void     add_node(t_list *file_list, char *filename) {
  /* create new node and append to list */
  t_list        *head = file_list;
  t_list        *new_node = create_list();
  char          *new_filename = malloc(sizeof(char) * strlen(filename) + 1);

  new_filename = strcat(new_filename, filename);
  new_node->filename = malloc(sizeof(char) * strlen(new_filename));
  new_node->filename = new_filename;
  new_node->next = NULL;

  free(new_filename);

  while (file_list->next != NULL) {
    file_list = file_list->next;
  }
  file_list->next = new_node;

  /* reset list pointer to head */
  file_list = head;
}

/*
  Malloc - needs to be free'd.
  Foward slashes separate directories.
*/

char *concat_path_to_filename(char *root, char *filename) {
  /* allocate memory for strcat */
  char *full_filename = malloc(sizeof(char) * ((strlen(root) + strlen(filename))) + 2);
  char *seperator = "/";
  full_filename = strcat(full_filename, root);
  full_filename = strcat(full_filename, seperator);
  full_filename = strcat(full_filename, filename);
  return full_filename;
}

// passed the node containing root filename of a directory.
void      add_dir_files_to_list(t_list *head, char *root) {
  DIR                   *dirp = opendir(root);
  struct dirent         *directory;
  char                  *filename;
  char                  *full_filename;

  /* continually open next directory stream until reaching the end of file list. */
  while ((directory = readdir(dirp)) != NULL) {
    filename = directory->d_name;
    /* If filename is "." or "..", skip to next directory entry */
    if ((strcmp(filename, ".") != 0) && (strcmp(filename, "..") != 0)
      && (strcmp(filename, root) != 0)) {
      /*
        Append to the root for full path.
        malloc: needs free
      */
      full_filename = concat_path_to_filename(root, filename);
      /* add a node with the full filename and path to the list */
      add_node(head, full_filename);
      free(full_filename);
    }
  }
  closedir(dirp);
}


void read_dir(t_list *dir_node, t_list *head) {
  char            *root = dir_node->filename;
  add_dir_files_to_list(head, root);
}

/* check if pathname is a directory */
int is_directory(const char *path) {
    struct stat *path_stat = malloc(sizeof(stat));
    stat(path, path_stat);
    int isDir = S_ISDIR(path_stat->st_mode);
    // free(path_stat);
    return isDir;
}