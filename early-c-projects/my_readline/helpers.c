#include "readline.h"

  /* copy str data to temp */

  char *create_temp(char *temp, char *str, char *overflow, size_t MAX_SIZE, bool should_return) {
    if (str == NULL || should_return == true) {
      temp = malloc((sizeof(char) * strlen(overflow)) + MAX_SIZE);
      temp = strncpy(temp, overflow, strlen(overflow));
    }

    if (overflow == NULL) {
      temp = malloc((sizeof(char) * strlen(str)) + MAX_SIZE);
      temp = strncpy(temp, str, strlen(str));
    }

    if ((str && overflow) && should_return == false) {
      temp = malloc((sizeof(char) * strlen(str)) +
                    (sizeof(char) * strlen(overflow)) + MAX_SIZE);
      temp = strncpy(temp, str, strlen(str));
      temp = strncat(temp, overflow, strlen(overflow));
    }

    return temp;
  }
