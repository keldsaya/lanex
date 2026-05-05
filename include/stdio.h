#ifndef H_STDIO
#define H_STDIO

void kprintf(const char *fmt, ...);
int skprintf(char *buf, const char *fmt, ...);

#define printf kprintf

#endif /* H_STDIO */
