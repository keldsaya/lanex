#include "cmd.h"
#include "history.h"
#include <lanex/sh.h>
#include <lanex/config.h>
#include <lanex/main.h>
#include <lanex/tty.h>
#include <lanex/messages.h>
#include <lanex/pit.h>
#include <lanex/pmm.h>
#include <power/power.h>
#include <rtc/rtc.h>
#include <string.h>
#include <stdio.h>
#ifdef CONFIG_FS_VFS
#include <vfs/vfs.h>
#endif
#include "path.h"

void execute(const char *cmd) {
  if (strcmp(cmd, "clear") == 0) {
    tty_clear();
  }
  else if (strcmp(cmd, "history") == 0) {
    for (int i = 0; i < history_get_count(); i++) {
      const char *hist_cmd = history_get_entry(i);
      if (hist_cmd && hist_cmd[0] != '\0')
        kprintf("  %d  %s\n", i + 1, hist_cmd);
    }
  }
#ifdef CONFIG_FS_VFS
  else if (strcmp(cmd, "ls") == 0) {
    const char *cur = get_cwd();
    vfs_inode_t *dir = NULL;

    if (strcmp(cur, "/proc") == 0) {
      dir = vfs_get_proc_inode();
    }
    else {
      kprintf(".\n..\nproc\n");
      return;
    }

    if (!dir) {
      kprintf("ls: cannot open directory\n");
      return;
    }

    char name[256];
    int index = 0;
    while (vfs_readdir(dir, index, name) == 0) {
      kprintf("%s\n", name);
      index++;
    }
  }
  else if (strcmp(cmd, "pwd") == 0) {
    kprintf("%s\n", get_cwd());
  }
  else if (strncmp(cmd, "cd", 2) == 0) {
    char path[256];
    if (strlen(cmd) > 3) {
      strcpy(path, cmd + 3);
    }
    else {
      strcpy(path, "/");
    }

    if (change_directory(path) != 0) {
      kprintf("cd: no such directory: %s\n", path);
    }
  }
#endif /* CONFIG_FS_VFS */
#ifdef CONFIG_DRIVER_POWER
  else if (strcmp(cmd, "shutdown") == 0) {
    power_shutdown();
  }
  else if (strcmp(cmd, "reboot") == 0) {
    power_reboot();
  }
#endif
#ifdef CONFIG_DRIVER_RTC
  else if (strcmp(cmd, "date") == 0) {
    rtc_print_datetime();
  }
#endif
  else if (strcmp(cmd, "free") == 0) {
    uint32_t total = k_get_mem();
    uint32_t free_pages = pmm_free_pages_count();
    uint32_t free_kb = free_pages * 4;
    kprintf("Total: %uKB, Free: %uKB, Used: %uKB\n", total, free_kb, total - free_kb);
  }
  else if (strcmp(cmd, "uptime") == 0) {
    uint32_t ticks = pit_get_ticks();
    kprintf("uptime: %d seconds\n", ticks / 1000);
  }
  else if (strncmp(cmd, "echo", 4) == 0) {
    if (strlen(cmd) > 5)
      kprintf("%s\n", cmd + 5);
    else
      kprintf("\n");
  }
  else if (strcmp(cmd, "panic") == 0) {
    kpanic("Called by user");
  }
  else if (strcmp(cmd, "help") == 0) {
    kprintf("  clear - Clear screen\n");
    kprintf("  history - Get history\n");
#ifdef CONFIG_FS_VFS
    kprintf("  ls - List directory\n");
    kprintf("  pwd - Show current directory\n");
    kprintf("  cd - Change current directory\n");
#endif
#ifdef CONFIG_DRIVER_POWER
    kprintf("  shutdown - Shutdown system\n");
    kprintf("  reboot - Reboot system\n");
#endif
#ifdef CONFIG_DRIVER_RTC
    kprintf("  date - Show time\n");
#endif
    kprintf("  free - Show memory\n");
    kprintf("  uptime - Show uptime\n");
    kprintf("  echo - Print\n");
    kprintf("  panic - Show kernel panic\n");
    kprintf("  help - Show this\n");
  }
  else if (cmd[0] != '\0') {
    kprintf("sh: Unknown command: %s\n", cmd);
  }
}
