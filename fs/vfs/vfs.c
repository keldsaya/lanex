#include "vfs.h"
#include <stdint.h>
#include <stddef.h>

static vfs_inode_t *root_inode = NULL;

void vfs_init(void) {
  root_inode = NULL;
}

int vfs_mount(const char *path, vfs_inode_t *root) {
  (void)path; (void)root;

  return 0;
}

int vfs_open(const char *path, vfs_file_t **file) {
  (void)path; (void)file;

  return 0;
}

int vfs_read(vfs_file_t *file, char *buf, uint32_t size) {
  (void)file; (void)size; (void)buf;

  return 0;
}

int vfs_close(vfs_file_t *file) {
  (void)file;

  return 0;
}

vfs_inode_t *vfs_lookup(vfs_inode_t *parent, const char *name) {
  (void)parent; (void)name;
  return NULL;
}
