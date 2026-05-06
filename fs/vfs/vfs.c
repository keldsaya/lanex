#include "vfs.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

static vfs_inode_t *root_inode = NULL;
static vfs_inode_t *proc_inode = NULL;  

void vfs_init(void) {
  root_inode = NULL;
  proc_inode = NULL;
}

int vfs_mount(const char *path, vfs_inode_t *root) {
  if (!path || !root)
    return -1;
    
  if (strcmp(path, "/proc") == 0) {
    proc_inode = root;
    return 0;
  }
    
  return -1;
}

vfs_inode_t *vfs_lookup(vfs_inode_t *parent, const char *name) {
  if (!parent || !name)
    return NULL;
    
  if (parent->i_op && parent->i_op->lookup)
    return parent->i_op->lookup(parent, name);
    
  return NULL;
}

int vfs_readdir(vfs_inode_t *dir, uint32_t index, char *name) {
  if (!dir || !dir->i_op || !dir->i_op->readdir)
    return -1;
  return dir->i_op->readdir(dir, index, name);
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

vfs_inode_t *vfs_get_proc_inode(void) {
  return proc_inode;
}
