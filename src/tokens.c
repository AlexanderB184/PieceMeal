#include "../include/tokens.h"

#include <string.h>

#define IS_WHITESPACE(c) (c == ' ' || c == '\t' || c == '\r' || c == '\n')

const char *next_token(const char *str) {
  if (str == NULL) {
    return NULL;
  }
  while (*str && !IS_WHITESPACE(*str)) {
    str++;
  }
  while (*str && IS_WHITESPACE(*str)) {
    str++;
  }
  if (*str == '\0') {
    return NULL;
  }
  return str;
}

int token_is_equal(const char *lhs, const char *rhs) {
  if (lhs == NULL) {
    return 0;
  }
  return !strncmp(lhs, rhs, strlen(rhs));
}

int token_is_int(const char *str) { return str && *str >= '0' && *str <= '9'; }

int token_length(const char *str) {
  if (!str) {
    return 0;
  }
  int length = 0;
  while (str[length] && !IS_WHITESPACE(str[length])) {
    length++;
  }
  return length;
}
