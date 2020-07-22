#include "my_list.h"

void free_node(t_list *node) {
  /*
    In an empty dir, node->filename == "".
    We did not allocate filename or path.
  */
  if (strcmp(node->filename, "") != 0) {
    /* in the original nodes, *path == *filename */
    if (node->filename != node->path) {
      free(node->filename);
    }
    free(node->path);
  }
  free(node->info);
  free(node);
  node = NULL;
}

void destroy_list(t_list **head) {
  t_list *current = *head;
  while (current != NULL) {
    if (strcmp(current->filename, "") == 0) {
      // handle end of empty dir: filename->""
      free_node(current);
      current = NULL;
    } else {
      t_list *temp = current;
      current = current->next;
      free_node(temp);
    }
  }
  head = NULL;
}