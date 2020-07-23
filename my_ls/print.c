#include "my_list.h"

/* changes head of list: skips nodes '.' and '../' */
t_list *skip_hidden_files(t_list *file_list, t_opts *opts) {
  /* handle -a option */
  if (opts->include_hidden_files == 0) {
    while (file_list != NULL) {
      if (file_list->filename[0] == '.') {
        file_list = file_list->next;
      } else {
        break;
      }
    }
  }
  return file_list;
}

void print_list(t_list *head, t_opts *opts) {
  if (head->is_original && head->is_dir) {
    /* don't print user arg if it is a dir */
    return;
  }
  /*
    In linux ls, files print in order DOWN columns.
    In my_ls, files were printed in order ACROSS columns.
    This lead to confusion about the order of the list.
    Instead, we now print one long column of files.
  */
  t_list *current_node = head;
  // int printed_index = 0;
  while (current_node != NULL) {
    // if (printed_index > 2) {
    //   printf("\n");
    //   printed_index = 0;
    // }
    if (current_node->info == NULL) {
      fprintf(stderr, "my_ls: %s: No such file or directory\n", current_node->filename);

    } else {
      /* handle -a option */
      current_node = skip_hidden_files(current_node, opts);
      if (current_node != NULL) {
        /* print filename */
        // printf("%-20s ", current_node->filename);
        printf("%s\n", current_node->filename);
        // printed_index++;
      }
    }
    if (current_node == NULL) {
      continue;
    } else {
      current_node = current_node->next;
    }
  }
  printf("\n");
}

// create, print, and delete file list from head dir node
void print_dir_list(t_list *sorted, t_opts *opts) {
  // handle an empty directory.
  if (strcmp(sorted->filename, "") == 0) {
    return;
  }
  t_list *dir_list = make_dir_list(sorted);
  t_list *sorted_dir = sort_with_options(dir_list, opts);
  // print non-dir filenames.
  print_list(sorted_dir, opts);
  // read/print files from directories if user input or -R option.
  recurse(sorted_dir, opts);
  // recursive lists are deleted after they are printed.
  // after recursion, delete original list.
  destroy_list(&sorted_dir);
}

void recurse(t_list *sorted, t_opts *opts) {
  int len = get_length(sorted);
  while (sorted != NULL) {
    if (sorted->is_original == 0) {
      while (sorted->filename[0] == '.') {
        if (sorted->next != NULL) {
          sorted = sorted->next;
        } else {
          // handle empty directory node
          sorted->next = create_node();
          sorted->next->filename = "";
          sorted = sorted->next;
        }
      }
    }
    if (strcmp(sorted->filename, "") != 0) {
      if (sorted->is_dir) {
        // print dirs passed in by user, or all dirs if -R
        if(sorted->is_original || opts->list_dirs_recursively == 1) {
          // print files from original directory passed in by user.
          if (sorted->is_original && len <= 1) {
            print_dir_list(sorted, opts);
          } else {
            // print path of nested dirs.
            printf("%s:\n", sorted->path);
            print_dir_list(sorted, opts);
            printf("\n");
          }
        }
      }
    }
    sorted = sorted->next;
  }
}