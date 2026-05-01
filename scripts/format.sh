#!/bin/bash
find kernel drivers libc -name "*.c" -o -name "*.h" | xargs clang-format -i
