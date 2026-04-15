# LANEX
**Lanex** is a monolithic, multiprocessor, minimalistic console-based operating system kernel, built from absolute scratch.
The name "Lanex" originated from a friend's reaction to Linux memes — a simple, memorable twist that stuck.

## Run
```
sudo pacman -S nasm qemu-desktop
yay -S i686-elf-binutils i686-elf-gcc
make
make run
```
## Run on Debian-based systems

> Tested on Linux Mint (Zara 22.2)

1. Edit `Makefile`

```makefile
CC = gcc -m32
AS = as --32
NASM = nasm -f elf32
CFLAGS = -Iinclude -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fno-stack-protector -fno-pic
LDFLAGS = -T linker.ld -m32 -ffreestanding -O2 -nostdlib -Wl,--oformat,binary
```
2. Run
```
sudo apt install build-essential gcc-multilib nasm qemu-system-x86
make
make run
```

## Authors

-   **keldsaya** - Initial development and maintenance.
-   **[oguzokdotdev](https://github.com/oguzokdotdev/)** - Documentation and porting.