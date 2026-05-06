#ifndef PROC_FS_H
#define PROC_FS_H

#include <stdint.h>
#include <vfs/vfs.h>

struct procfs_entry {
  char name[64];
  int (*read)(struct vfs_file*, char*, uint32_t);
  void *data;
  struct procfs_entry *next;
};
typedef struct procfs_entry procfs_entry_t;

int procfs_register_file(const char *name,
  int (*read)(struct vfs_file*, char*, uint32_t),
  void *data);

vfs_inode_t *procfs_mount(void);
vfs_inode_t *procfs_lookup(vfs_inode_t *parent, const char *name);

#endif
