#include "read.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


int read_int(void)
{
  int n;
  int r = scanf("%d", &n);
  if (r == 1)
  {
    return n;
  }
  return -1;
}

char *read_str(void)
{
  char c = 0;
  if (scanf(" %c", &c) != 1) { // ignore initial WS
    return NULL;
  }

  int maxlen = 1;
  int len = 1;
  char *str = malloc(maxlen * sizeof(char));
  str[0] = c;

  while (1) {
    if (scanf("%c", &c) != 1 ||  c == '\n') {
      break;
    } //c == ' ' ||
    
    if (len == maxlen) { // DOUBLE the allocated array size
      maxlen *= 2;
      str = realloc(str, maxlen * sizeof(char));
    }
    ++len;
    str[len - 1] = c;
  }
  // shrink the array back down to the correct size
  if (str[len - 1] == '\n') {
    str = realloc(str, (len) * sizeof(char));
    str[len - 1] = '\0';
  } else {
    str = realloc(str, (len + 1) * sizeof(char));
    str[len] = '\0';
  }
  return str;
}
