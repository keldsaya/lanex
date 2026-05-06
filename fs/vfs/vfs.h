#ifndef VFS_H
#define VFS_H

#include <stdint.h>

struct vfs_inode;
struct vfs_file;

typedef struct vfs_file_ops {
    int (*read)(struct vfs_file *file, char *buf, uint32_t size);
    int (*write)(struct vfs_file *file, const char *buf, uint32_t size);
    int (*open)(struct vfs_inode *inode, struct vfs_file *file);
    int (*close)(struct vfs_file *file);
} vfs_file_ops_t;

typedef struct vfs_inode_ops {
    struct vfs_inode *(*lookup)(struct vfs_inode *parent, const char *name);
    int (*readdir)(struct vfs_inode *dir, uint32_t index, char *name);
} vfs_inode_ops_t;

typedef struct vfs_inode {
    uint32_t inode_num;
    uint32_t size;
    void *private;  
    vfs_inode_ops_t *i_op;
    vfs_file_ops_t *f_op;
} vfs_inode_t;

typedef struct vfs_file {
    vfs_inode_t *inode;
    uint32_t offset;
    uint32_t flags;
} vfs_file_t;

void vfs_init(void);
int vfs_mount(const char *path, vfs_inode_t *root);
int vfs_readdir(vfs_inode_t *dir, uint32_t index, char *name);
int vfs_open(const char *path, vfs_file_t **file);
int vfs_read(vfs_file_t *file, char *buf, uint32_t size);
int vfs_close(vfs_file_t *file);
vfs_inode_t *vfs_lookup(vfs_inode_t *parent, const char *name);
vfs_inode_t *vfs_get_proc_inode(void);

#endif
