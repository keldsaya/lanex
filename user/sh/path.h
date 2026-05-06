#ifndef SH_PATH_H
#define SH_PATH_H

#define PATH_MAX 256

void  path_init(void);
char *get_cwd(void);
int   set_cwd(const char *path);
int   change_directory(const char *path);
char *normalize_path(const char *path, char *resolved);

#endif
