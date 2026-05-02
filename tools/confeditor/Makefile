CONFEDITOR_DIR = $(CURDIR)
CONFEDITOR_SRC = config_editor.c
CONFEDITOR_OUT = $(BUILD_DIR)/config_editor

.PHONY: all clean confeditor

all: confeditor

confeditor: $(CONFEDITOR_SRC)
	@echo "  CC       user/confeditor/$<"
	@mkdir -p $(dir $(CONFEDITOR_OUT))
	@gcc -Wall -Wextra -O2 -o $(CONFEDITOR_OUT) $<

clean:
	@rm -f $(CONFEDITOR_OUT)
