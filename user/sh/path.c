#include "path.h"
#include <string.h>
#include <stddef.h>

static char cwd[PATH_MAX] = "/";

void path_init(void) {
  cwd[0] = '/';
  cwd[1] = '\0';
}

char *get_cwd(void){
  return cwd;
}

int set_cwd(const char *path) {
  if (!path || path[0] != '/')
    return -1;
  size_t len = strlen(path);
  if (len >= PATH_MAX)
    return -1;
  strcpy(cwd, path);
  return 0;
}

static void append_component(char *dest, const char *comp) {
  size_t dest_len = strlen(dest);
  if (dest_len == 0 || dest[dest_len - 1] != '/') {
    dest[dest_len] = '/';
    dest[dest_len + 1] = '\0';
  }
  strcat(dest, comp);
}

char *normalize_path(const char *path, char *resolved) {
  if (!path || !resolved)
    return NULL;

  char temp[PATH_MAX];
  if (path[0] == '/')
    temp[0] = '\0';
  else
    strcpy(temp, cwd);

  char buf[PATH_MAX];
  strcpy(buf, path);
  char *tok = strtok(buf, "/");
  while (tok) {
    if (strcmp(tok, "..") == 0) {
      char *last_slash = strrchr(temp, '/');
      if (last_slash && last_slash != temp)
        *last_slash = '\0';
      else if (last_slash == temp)
        temp[1] = '\0';
      else
        temp[0] = '\0';
    }
    else if (strcmp(tok, ".") != 0 && tok[0] != '\0') {
      append_component(temp, tok);
    }
    tok = strtok(NULL, "/");
  }

  if (temp[0] == '\0')
    strcpy(resolved, "/");
  else
    strcpy(resolved, temp);
  return resolved;
}

int change_directory(const char *path) {
  char resolved[PATH_MAX];
  if (!normalize_path(path, resolved))
    return -1;
  if (strcmp(resolved, "/") != 0 && strcmp(resolved, "/proc") != 0)
    return -1;
  return set_cwd(resolved);
}
