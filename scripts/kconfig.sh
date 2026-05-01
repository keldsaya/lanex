#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
CONFIG_FILE="$PROJECT_ROOT/.config"
DEFCONFIG_FILE="$PROJECT_ROOT/user/defconfig"
CONFEDITOR_BIN="$PROJECT_ROOT/build/config_editor"

case "$1" in
  menu)
    if [ ! -f "$CONFIG_FILE" ]; then
      cp "$DEFCONFIG_FILE" "$CONFIG_FILE"
    fi
        
    if [ ! -f "$CONFEDITOR_BIN" ]; then
      make -C "$PROJECT_ROOT/user/confeditor" confeditor > /dev/null 2>&1
    fi
        
    "$CONFEDITOR_BIN"
    ;;
    
    def)
      cp "$DEFCONFIG_FILE" "$CONFIG_FILE"
      ;;
    
    *)
      echo "Usage: $0 {menu|def}"
      echo "  menu - Run configuration editor"
      echo "  def  - Restore default configuration"
      exit 1
      ;;
esac
