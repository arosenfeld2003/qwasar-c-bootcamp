#include "readline.h"

/* Pick a random size  */
#define MAX_SIZE 30

char            *my_readline(int fd) {
  static char     *overflow = NULL;
  static char     *str = NULL;
  char            *buffer = NULL;
  char            *temp = NULL;
  static int      bytes_read = -1; // start with a non-zero value.
  int             str_size = 1; // terminating null byte: '/0'
  int             overflow_size = 0;
  char            *buf_p;
  int             i = 0, j = 0;
  static bool     should_return = false;

  /* if bytes_read is 0, we are done */
  if (bytes_read == 0) {
    return NULL;
  }

  /* If previous string was returned, clear string */
  if (should_return == true)  {
    str = memset(str, '\0', strlen(str));
    str = NULL;
  }

  /* assign previous data to temp, and empty overflow and str variables */
  if (overflow || str) {
    temp = create_temp(temp, str, overflow, MAX_SIZE, should_return);
    str = NULL;
    overflow = NULL;
    should_return = false;
  }

  /* read into buffer and set buffer pointer */
  buffer = malloc(sizeof(char) * MAX_SIZE);
  bytes_read = read(fd, buffer, MAX_SIZE);
  buf_p = buffer;

  /* count chars read until '\n' or until we've read the entire buffer */
  i = 0;
  while (i < MAX_SIZE) {
    if (*buf_p == '\n') {
      break;
    } else {
      buf_p++;
      str_size++;
      i++;
    }
  }

  /* replace '\n' with null terminator; skip '\n' */
  if (*buf_p == '\n') {
    should_return = true;
    *buf_p = '\0';
    buf_p++;
  }

  /* get size of overflow for this read call */
  overflow_size = (MAX_SIZE - str_size);

  /* copy appropriate amount of data to overflow from buffer */
  if (overflow_size > 0) {
    overflow = malloc(sizeof(char) * overflow_size);
    overflow = strncpy(overflow, buf_p, overflow_size);
  }

  /* copy appropriate amount of data from buffer */
  if (temp != NULL) {
    /* we have data from previous call and need to combine */
    str = malloc((sizeof(char) * str_size) + (sizeof(char) * strlen(temp)) + 1);
    str_size += strlen(temp);
    /* read any data in temp (from previous calls) */
    i = 0;
    while (i < strlen(temp)) {
      str[i] = temp[i];
      i++;
    }
    i = strlen(temp);
  } else {
    /* no data from previous call */
    str = malloc(sizeof(char) * str_size);
    i = 0;
  }

  if (bytes_read != 0) {
    /* read data from buffer (current read() call) */
    while (i < str_size) {
      str[i] = buffer[j];
      j++;
      i++;
    }
  }

  if (should_return == false && bytes_read != 0) {
    /* call again  */
    my_readline(fd);
  }
  return str;
}
