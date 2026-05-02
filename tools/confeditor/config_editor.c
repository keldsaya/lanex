#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>

#define KCONFIG_FILE "Kconfig"
#define CONFIG_FILE ".config"
#define MAX_LINE 512
#define CMD_BUF_SIZE 16
#define SCROLL_HEIGHT 18

typedef struct {
  char name[64];
  char value[8];
  char desc[256];
  char depends[64];
} config_option_t;

static config_option_t* options = NULL;
static int num_options = 0;
static int cursor = 0;
static int scroll_offset = 0;
static int cmd_mode = 0;
static char cmd_buf[CMD_BUF_SIZE];
static int cmd_pos = 0;

static struct termios orig_termios;

void disable_raw_mode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disable_raw_mode);

  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

char getch() {
  char c;
  read(STDIN_FILENO, &c, 1);
  return c;
}

void clear_screen() {
  printf("\033[2J\033[H");
}

void trim(char* str) {
  char* start = str;
  while (isspace((unsigned char)*start)) start++;
  
  char* end = start + strlen(start) - 1;
  while (end > start && isspace((unsigned char)*end)) end--;
  
  *(end + 1) = '\0';
  if (start != str) {
    memmove(str, start, end - start + 2);
  }
}

void parse_kconfig() {
  FILE* fp = fopen(KCONFIG_FILE, "r");
  if (!fp) {
    printf("Cannot open %s\n", KCONFIG_FILE);
    exit(1);
  }

  // First count options
  char line[MAX_LINE];
  int count = 0;
  while (fgets(line, sizeof(line), fp)) {
    if (strncmp(line, "config ", 7) == 0) {
      count++;
    }
  }

  if (count == 0) {
    fclose(fp);
    printf("No config options found in %s\n", KCONFIG_FILE);
    exit(1);
  }

  options = malloc(count * sizeof(config_option_t));
  if (!options) {
    fclose(fp);
    printf("Memory allocation failed\n");
    exit(1);
  }

  rewind(fp);
  num_options = 0;

  while (fgets(line, sizeof(line), fp)) {
    if (strncmp(line, "config ", 7) == 0) {
      // Get config name
      char* name = line + 7;
      trim(name);
      char* nl = strchr(name, '\n');
      if (nl) *nl = '\0';
      
      strcpy(options[num_options].name, name);
      strcpy(options[num_options].value, "y");
      strcpy(options[num_options].desc, "");
      strcpy(options[num_options].depends, "");

      // Read until next config or end
      int in_help = 0;
      while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "config ", 7) == 0) {
          fseek(fp, -strlen(line), SEEK_CUR);
          break;
        }
        
        if (strncmp(line, "endmenu", 7) == 0) {
          break;
        }
        
        char* trimmed = line;
        trim(trimmed);
        
        if (strncmp(trimmed, "help", 4) == 0) {
          in_help = 1;
          continue;
        }
        
        if (in_help) {
          if (trimmed[0] == '\0') {
            in_help = 0;
          }
          continue;
        }
        
        if (strncmp(trimmed, "bool", 4) == 0) {
          char* desc = trimmed + 4;
          trim(desc);
          if (desc[0] == '"') {
            desc++;
            int len = strlen(desc);
            if (len > 0 && desc[len-1] == '"') {
              desc[len-1] = '\0';
            }
          }
          strcpy(options[num_options].desc, desc);
        }
        else if (strncmp(trimmed, "default", 7) == 0) {
          char* val = trimmed + 7;
          trim(val);
          if (strcmp(val, "y") == 0) {
            strcpy(options[num_options].value, "y");
          } else if (strcmp(val, "n") == 0) {
            strcpy(options[num_options].value, "n");
          }
        }
        else if (strncmp(trimmed, "depends", 7) == 0) {
          char* dep = trimmed + 7;
          if (strncmp(dep, "on", 2) == 0) {
            dep += 2;
            trim(dep);
            strcpy(options[num_options].depends, dep);
          }
        }
      }
      num_options++;
    }
  }
  fclose(fp);
}

void load_config_values() {
  FILE* fp = fopen(CONFIG_FILE, "r");
  if (!fp) return;
  
  char line[MAX_LINE];
  while (fgets(line, sizeof(line), fp)) {
    line[strcspn(line, "\n")] = 0;
    if (line[0] == '#' || line[0] == '\0') continue;
    
    char* eq = strchr(line, '=');
    if (eq) {
      *eq = '\0';
      char* name = line;
      char* val = eq + 1;
      trim(name);
      trim(val);
      
      // Remove CONFIG_ prefix if present
      if (strncmp(name, "CONFIG_", 7) == 0) {
        name += 7;
      }
      
      for (int i = 0; i < num_options; i++) {
        if (strcmp(options[i].name, name) == 0) {
          strcpy(options[i].value, val);
          break;
        }
      }
    }
  }
  fclose(fp);
}

void save_config() {
  FILE* fp = fopen(CONFIG_FILE, "w");
  if (!fp) {
    printf("Error: Cannot write to .config\n");
    return;
  }
  
  fprintf(fp, "# Lanex Kernel Configuration\n");
  fprintf(fp, "# Auto-generated\n\n");
  
  for (int i = 0; i < num_options; i++) {
    fprintf(fp, "CONFIG_%s=%s\n", options[i].name, options[i].value);
  }
  fclose(fp);
}

void toggle_option(int idx) {
  if (options[idx].value[0] == 'y')
    strcpy(options[idx].value, "n");
  else
    strcpy(options[idx].value, "y");
}

void execute_command() {
  if (strcmp(cmd_buf, "q") == 0) {
    exit(0);
  } else if (strcmp(cmd_buf, "wq") == 0) {
    save_config();
    exit(0);
  } else if (strcmp(cmd_buf, "w") == 0) {
    save_config();
  } else if (strcmp(cmd_buf, "r") == 0) {
    free(options);
    options = NULL;
    parse_kconfig();
    load_config_values();
    cursor = 0;
    scroll_offset = 0;
  }
}

void draw() {
  clear_screen();
  
  if (cmd_mode) {
    printf(":%s\n", cmd_buf);
  } else {
    printf("\n");
  }
  
  printf("----------------------------------------------------------------------\n");
  
  int end = scroll_offset + SCROLL_HEIGHT;
  if (end > num_options) end = num_options;
  
  for (int i = scroll_offset; i < end; i++) {
    if (i == cursor && !cmd_mode)
      printf("# ");
    else
      printf("  ");
    
    printf("%-32s = %s\n", options[i].name, options[i].value);
  }
  
  if (num_options > SCROLL_HEIGHT) {
    printf("\n[%d-%d of %d]\n", scroll_offset + 1, end, num_options);
  }
  
  printf("----------------------------------------------------------------------\n");
  
  if (cursor < num_options && options[cursor].desc[0] != '\0') {
    char desc_buf[80];
    strncpy(desc_buf, options[cursor].desc, 77);
    desc_buf[77] = '\0';
    printf("%s\n", desc_buf);
  } else {
    printf("No description available\n");
  }
  
  printf("----------------------------------------------------------------------\n");
  printf(":q  :wq  :w  :r  |  Space=toggle  |  j/k up/down  |  Esc\n");
}

int main() {
  parse_kconfig();
  load_config_values();
  enable_raw_mode();
  
  while (1) {
    draw();
    
    char c = getch();
    
    if (c == 27) {
      if (cmd_mode) {
        cmd_mode = 0;
        cmd_pos = 0;
        cmd_buf[0] = '\0';
      }
    } else if (cmd_mode) {
      if (c == '\n') {
        cmd_buf[cmd_pos] = '\0';
        execute_command();
        cmd_mode = 0;
        cmd_pos = 0;
        cmd_buf[0] = '\0';
      } else if (c == 127 || c == '\b') {
        if (cmd_pos > 0) {
          cmd_pos--;
          cmd_buf[cmd_pos] = '\0';
        }
      } else if (c >= 32 && c <= 126 && cmd_pos < CMD_BUF_SIZE - 1) {
        cmd_buf[cmd_pos++] = c;
        cmd_buf[cmd_pos] = '\0';
      }
    } else {
      if (c == ':') {
        cmd_mode = 1;
        cmd_pos = 0;
        cmd_buf[0] = '\0';
      } else if (c == ' ') {
        toggle_option(cursor);
      } else if (c == 'j' || c == 's' || c == 66) {
        if (cursor < num_options - 1) {
          cursor++;
          if (cursor >= scroll_offset + SCROLL_HEIGHT) {
            scroll_offset = cursor - SCROLL_HEIGHT + 1;
          }
        }
      } else if (c == 'k' || c == 'w' || c == 65) {
        if (cursor > 0) {
          cursor--;
          if (cursor < scroll_offset) {
            scroll_offset = cursor;
          }
        }
      }
    }
  }
  
  free(options);
  disable_raw_mode();
  return 0;
}
