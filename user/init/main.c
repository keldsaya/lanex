#include <lanex/config.h>
#include <vfs/vfs.h>
#include <procfs/procfs.h>
#include <lanex/tty.h>
#include <lanex/messages.h>
#include <lanex/sh.h>
#include <lanex/main.h>
#include <lanex/pmm.h>
#include <stdio.h>
#include <string.h>

static int get_memory(struct vfs_file *file, char *buf, uint32_t size) {
  uint32_t total = k_get_mem();
  uint32_t free_pages = pmm_free_pages_count();
  uint32_t free_kb = free_pages * 4;
  uint32_t used_kb = total - free_kb;

  char output[256];
  int len = skprintf(output, "Total: %uKB, Free: %uKB, Used: %uKB\n",
                     total, free_kb, used_kb);

  if (file->offset >= (uint32_t)len) return 0;
  uint32_t avail = len - file->offset;
  if (size > avail) size = avail;
  for (uint32_t i = 0; i < size; i++)
    buf[i] = output[file->offset + i];
  file->offset += size;
  return size;
}

static void procfs_setup(void) {
#ifdef CONFIG_FS_PROCFS
  procfs_register_file("memory", get_memory, NULL);
#endif
}

void init_main(void) {
#ifdef CONFIG_FS_VFS
  procfs_setup();
  vfs_inode_t *proc_root = procfs_mount();
  vfs_mount("/proc", proc_root);
#endif

  welcome();
  sh_main();
}
