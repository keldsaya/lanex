#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
CONFIG_FILE="$PROJECT_ROOT/.config"
KCONFIG_SOURCE="$PROJECT_ROOT/Kconfig"
CONFEDITOR_BIN="$PROJECT_ROOT/build/config_editor"

generate_default_config() {
    echo "# Generated from Kconfig" > "$CONFIG_FILE"

    awk '
    /^config / {
        conf = $2
    }
    /^[[:space:]]+default / {
        if (conf != "") {
            print "CONFIG_" conf "=" $2
            conf = ""
        }
    }' "$KCONFIG_SOURCE" >> "$CONFIG_FILE"
}

case "$1" in
  menu)
    if [ ! -f "$CONFIG_FILE" ]; then
      generate_default_config
    fi

    if [ ! -f "$CONFEDITOR_BIN" ]; then
      make -C "$PROJECT_ROOT/tools/confeditor" confeditor > /dev/null 2>&1
    fi

    # Run editor from project root so it finds Kconfig
    cd "$PROJECT_ROOT" && "$CONFEDITOR_BIN"
    ;;

  def)
    generate_default_config
    ;;

  *)
    echo "Usage: $0 {menu|def}"
    exit 1
    ;;
esac
