#include <string.h>

char *strtok(char *str, const char *delim) {
  static char *next_token = NULL;
  char *token_start;
  
  if (str != NULL) {
    next_token = str;
  }
  
  if (next_token == NULL || *next_token == '\0') {
    return NULL;
  }
  
  while (*next_token != '\0' && strchr(delim, *next_token) != NULL) {
    next_token++;
  }
  
  if (*next_token == '\0') {
    return NULL;
  }
  
  token_start = next_token;
  
  while (*next_token != '\0' && strchr(delim, *next_token) == NULL) {
    next_token++;
  }
  
  if (*next_token != '\0') {
    *next_token = '\0';
    next_token++;
  }
  
  return token_start;
}
