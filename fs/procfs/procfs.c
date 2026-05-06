#include "procfs.h"
#include <lanex/mm/kmalloc.h>
#include <vfs/vfs.h>
#include <stdint.h>
#include <string.h>

static procfs_entry_t *first_entry = NULL;

static int procfs_file_read(vfs_file_t *file, char *buf, uint32_t size) {
  procfs_entry_t *entry = (procfs_entry_t *)file->inode->private;
  if (!entry || !entry->read)
    return 0;
  return entry->read(file, buf, size);
}

static vfs_file_ops_t procfs_file_ops = {
  .read = procfs_file_read,
  .write = NULL,
  .open = NULL,
  .close = NULL
};

static vfs_inode_ops_t procfs_dir_ops = {
  .lookup = procfs_lookup,
  .readdir = NULL
};

vfs_inode_t *procfs_lookup(vfs_inode_t *parent, const char *name) {
  (void)parent;

  procfs_entry_t *entry = first_entry;
  while (entry) {
    if (strcmp(entry->name, name) == 0) {
      vfs_inode_t *inode = (vfs_inode_t *)kmalloc(sizeof(vfs_inode_t));
      if (!inode)
        return NULL;
      
      memset(inode, 0, sizeof(vfs_inode_t));
      inode->inode_num = 0;
      inode->size = 0;
      inode->private = entry;  
      inode->i_op = NULL;
      inode->f_op = &procfs_file_ops;
      return inode;
    }
    entry = entry->next;
  }
  return NULL;
}

int procfs_register_file(const char *name,
                         int (*read)(struct vfs_file*, char*, uint32_t),
                         void *data) {
  procfs_entry_t *entry = (procfs_entry_t *)kmalloc(sizeof(procfs_entry_t));
  if (!entry)
    return -1;

  memset(entry, 0, sizeof(procfs_entry_t));
  strcpy(entry->name, name);
  entry->read = read;
  entry->data = data;
  entry->next = NULL;

  if (!first_entry) {
    first_entry = entry;
  } else {
    procfs_entry_t *last = first_entry;
    while (last->next)
      last = last->next;
    last->next = entry;
  }

  return 0;
}

vfs_inode_t *procfs_mount(void) {
  vfs_inode_t *root = (vfs_inode_t *)kmalloc(sizeof(vfs_inode_t));
  if (!root)
    return NULL;

  memset(root, 0, sizeof(vfs_inode_t));
  root->inode_num = 0;
  root->size = 0;
  root->private = NULL;
  root->i_op = &procfs_dir_ops;
  root->f_op = NULL;  

  return root;
}

int procfs_readdir(vfs_inode_t *dir, uint32_t index, char *name) {

  (void)dir;
  procfs_entry_t *entry = first_entry;
  uint32_t i = 0;

  while (entry) {
    if (i == index) {
      strcpy(name, entry->name);
      return 0;
    }
    entry = entry->next;
    i++;
  }
  return -1;  
}
