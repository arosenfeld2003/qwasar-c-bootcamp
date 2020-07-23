#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/*
  My implementation of the C library (stdio.h) method 'printf'.
  Includes conversions for %d, %o, %d, %x, %c, %s, and %p.
  Includes my implementation of a method to convert a number of any base to another base (e.g. decimal to hexadecimal).
*/

// print %d or %i.
int printnumber(int n) {
  int len = 0;
  if (n < 0) {
    putchar('-');
    len++;
    n = -n;
  }
  if (n == 0) {
    putchar('0');
    len++;
  }
  if (n / 10) {
    printnumber(n / 10);
    putchar(n % 10 + '0');
    len++;
  } else {
    putchar(n + '0');
    len++;
  }
  return len;
}

// print %u.
int print_unsigned_number(int n) {
  int len = 0;
  if (n == 0) {
    putchar('0');
    len++;
  }
  if (n / 10) {
    printnumber(n / 10);
    putchar(n % 10 + '0');
    len++;
  } else {
    putchar(n + '0');
    len++;
  }
  return len;
}

int print_char_array(char *s) {
  int i = 0;
  int len = 0;
  while (s[i] != '\0') {
    putchar(s[i]);
    i++;
    len++;
  }
  return len;
}

// convert integer number to a string, representing a number of another base.
char *convert(long long num, char *digits, int baseLen) {
  int len = 0;
  long long numCopy = num;

  do {
    len++;
    numCopy /= baseLen;
  } while (numCopy != 0);

  char *buffer = malloc(sizeof(int) * len);

  // add null terminator
  buffer[len] = '\0';
  len--;

  while (num != 0) {
    buffer[len] = digits[num % baseLen];
    num /= baseLen;
    len--;
  }
  
  return buffer;
}

int my_printf(char * restrict format, ...) {
  int len = 0; // for returning number of chars printed.
  char *s; // string and pointer.
  int n;
  void *p; // for %p and %x
  char *traverse; // for iterating argument list.

  va_list arg;
  va_start(arg, format);

  for (traverse = format; *traverse != '\0'; traverse++) {
    // print chars if not a conversion spec.
    if (*traverse != '%') {
	  putchar(*traverse);
      len++;
	}
    // handle conversion spec.
    if (*traverse == '%') {
      // skip to next char
      traverse++;
      // d, o, u, c, s, p, i.
      if (*traverse == 'd' || *traverse == 'i') {
        n = va_arg(arg, int);
        len += printnumber(n);
      }
      if (*traverse == 'o') {
        n = va_arg(arg, long long);
        char *base = "01234567";
        char *oct = convert(n, base, 8);
        len += print_char_array(oct);
      }
      if (*traverse == 'u') {
        n = va_arg(arg, int);
        len += print_unsigned_number(n);
      }
      if (*traverse == 'c') {
        n = va_arg(arg, int);
        putchar(n);
        len++;
      }
      if (*traverse == 's') {
        s = va_arg(arg, char *);
        if (s == NULL) {
          s = "(null)";
        }
        len += print_char_array(s);
      }
      if (*traverse == 'x') {
        p = va_arg(arg, void *);
        char *base = "0123456789abcdef";
        char *hex = convert((long long) p, base, 16);
        len += print_char_array(hex);
        free(hex);
      }
      if (*traverse == 'p') {
        p = va_arg(arg, void *);
        char *base = "0123456789abcdef";
        char *hex = convert((long long) p, base, 16);
        char *hex_str = calloc((strlen(hex) + 2), sizeof(int));
        strcpy(hex_str, "0x");
        strcat(hex_str, hex);
        len += print_char_array(hex_str);
        free(hex_str);
        free(hex);
      }
    }
  }
  va_end(arg);
  return len;
}
