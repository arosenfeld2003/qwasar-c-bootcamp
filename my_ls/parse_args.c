#include "my_list.h"

/* user must free filenames array */
char **allocate_filenames(char *file, int i, char **filenames) {
  filenames[i] = malloc(sizeof(char) * strlen(file) + 1);
  strcpy(filenames[i], file);
  return filenames;
}

int count_files(int argc, char **argv) {
  int filecount = 0;
  int i = 1;
  while (i < argc) {
    if (argv[i][0] != '-') {
      filecount++;
    }
    i++;
  }
  return filecount;
}

char **set_filenames(int argc, char **argv) {
  char **filenames = NULL;
  char *file = NULL;
  int filecount = count_files(argc, argv);

  if (filecount == 0) {
    filecount = 1;
    filenames = malloc(sizeof(char *) * (filecount));
    file = ".";
    filenames = allocate_filenames(file, 0, filenames);
  }

  if (filenames != NULL) {
    int i = 1;
    // skip the flags.
    while (argv[i]) {
      if (argv[i][0] == '-') {
        i++;
      }
    }
    filenames = allocate_filenames(file, 0, filenames);
  } else {
    int i = 0;
    int j;
    switch(argv[1][0]) {
      case '-':
        j = 2;
        break;
      default:
        j = 1;
    }
    if (argv[j] == NULL) {
      return filenames;
    }
    filenames = malloc(sizeof(char *) * (filecount));
    while (j < argc) {
      filenames = allocate_filenames(argv[j], i, filenames);
      i++;
      j++;
    }
  }
  return filenames;
}

char *create_flags(int argc, char **argv) {
  char *flags = NULL;
  int flag_count = 0;

  // count number of flags and allocate space.
  int i = 1;
  while (i < argc) {
    if (argv[i][0] ==  '-') {
      flag_count += (strlen(argv[i]) - 1);
    }
    i++;
  }
  // flags free'd in get_opts() after being read.
  flags = malloc(sizeof(char) * flag_count + 1);
  char temp_head[flag_count + 1];
  char *temp_ptr = temp_head;
  int len, j;
  i = 1;
  while (i < argc) {
    j = 0;
    if (argv[i][j] == '-') {
      j++;
      len = strlen(argv[i]);
      while (j < len) {
        *temp_ptr = argv[i][j];
        temp_ptr++;
        j++;
      }
    }
    i++;
  }
  *temp_ptr = '\0';
  strcpy(flags, temp_head);
  return flags;
}
