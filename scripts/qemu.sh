#!/bin/bash
MEM=${1:-4M}
IMG=${2:-build/lanex.img}

qemu-system-i386 -m "$MEM" -drive file="$IMG",format=raw,index=0,media=disk
