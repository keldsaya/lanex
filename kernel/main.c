#include <lanex/main.h>
#include <lanex/config.h>
#include <lanex/idt.h>
#include <lanex/messages.h>
#include <lanex/pic.h>
#include <lanex/pit.h>
#include <lanex/pmm.h>
#include <lanex/tty.h>
#include <lanex/sh.h>
#include <vfs/vfs.h>
#include <procfs/procfs.h>
#include <stdio.h>

#ifdef CONFIG_DRIVER_RTC
#include <rtc/rtc.h>
#endif

static uint32_t mem_l;
static uint32_t mem_h;

uint32_t k_get_mem() {
  uint32_t mem = 1024;
  if (mem_h > 0) {
    mem += mem_l + (mem_h * 64);
  } else { mem += mem_l; }
  return mem;
}

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
  
  for (uint32_t i = 0; i < size; i++) {
    buf[i] = output[file->offset + i];
  }
  
  file->offset += size;
  return size;
}

void procfs_setup() {
#ifdef CONFIG_FS_PROCFS
  procfs_register_file("memory", get_memory, NULL);
#endif
}

void kmain(uint32_t mem_low, uint32_t mem_high) {
  mem_l = mem_low;
  mem_h = mem_high;
  
  tty_initialize();
  
  uint32_t total_memory_kb = k_get_mem();
  
  idt_install();
  pic_remap();
  pit_init(1000);
  pmm_init(total_memory_kb);
  
#ifdef CONFIG_FS_VFS
  vfs_init();
  procfs_setup();
#endif
  
#ifdef CONFIG_DRIVER_RTC
  rtc_init(1024);
#endif
  
  asm volatile("sti");
 
  welcome();
  sh_main();
  
  for (;;) {
    kpanic("Entered in main().loop");
  }
}
