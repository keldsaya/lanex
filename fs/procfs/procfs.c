#include "procfs.h"
#include <vfs/vfs.h>
#include <stdint.h>

#ifndef CONFIG_FS_VFS
struct vfs_file;
#endif

int procfs_register_file(const char *name,
                         int (*read)(struct vfs_file*, char*, uint32_t),
                         void *data) {
  (void)name;
  (void)read;
  (void)data;
  
  return 0;
}
