#!/bin/bash

CONFIG_FILE=$1
OUTPUT_HEADER=$2

if [ ! -f "$CONFIG_FILE" ]; then
    echo "Error: .config file not found at $CONFIG_FILE"
    exit 1
fi

mkdir -p "$(dirname "$OUTPUT_HEADER")"

{
    echo "/* Auto-generated config header for Lanex OS */"
    echo "#ifndef _LANEX_CONFIG_H_"
    echo "#define _LANEX_CONFIG_H_"
    echo ""
    echo "/* Enabled configs (y): */"
    grep "^CONFIG_.*=y" "$CONFIG_FILE" | sed 's/=y$/ 1/' | sed 's/^/#define /'
    echo ""
    echo "/* Disabled configs (n): */"
    grep "^CONFIG_.*=n" "$CONFIG_FILE" | sed 's/=n$//' | sed 's/^/#undef /'
    echo ""
    echo "#endif"
} > "$OUTPUT_HEADER"

echo "  GEN     ${OUTPUT_HEADER#$(pwd)/}"
