/*
**
*/
#include "my_tar.h"

/*
**
*/


void            destroy_list(t_list **head) {
  t_list        *current;
  t_list        *next;

  current = *head;
  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
  head = NULL;
}


/* delete any nodes whose filenames start with char value */
t_list *delete_nodes(t_list *curr, char value) {
  t_list          *next;
  next = curr->next;

  /* See if we are at end of list. */
  if (curr->filename == NULL) {
    return NULL;
  }

  /* see if the next node is empty */
  if (next == NULL) {
    return curr;
  }

  /* Check to see if current node is one to be deleted. */;
  char *filename = curr->filename;
  if (filename[0] == value) {

    /* Deallocate the node. */
    free(curr->filename);

    /* recursive call: continue checking list after deleted element */
    return delete_nodes(next, value);
  }

  /* recursive call: continue checking list after retained element */
  curr->next = delete_nodes(curr->next, value);

  /*
    Return the pointer to where we were called from.
    Since we did not remove this node it will be the same.
  */
  return curr;
}