#ifndef PROC_FS_H
#define PROC_FS_H

#include <stdint.h>
#include <vfs/vfs.h>

int procfs_register_file(const char *name, 
    int (*read)(struct vfs_file*, char*, uint32_t), 
    void *data);

#endif /* PROC_FS_H */
