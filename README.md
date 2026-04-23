# LANEX

**Lanex** is a monolithic, -nix-like, multiprocessor, minimalistic, console-based operating system kernel, built from absolute scratch.

**Lanex** doesn't use other -nix code — everything is from scratch. But it looks like a -nix operating system kernel.

The name "**Lanex**" originated from a friend's reaction to Linux memes — a simple, memorable twist that stuck.

## Run (Arch-based systems)

> Tested on Arch Linux

```
sudo pacman -S nasm qemu-desktop
yay -S i686-elf-binutils i686-elf-gcc
make
make run
```
## Run (Debian-based systems)

> Tested on Linux Mint (Zara 22.2)

```
sudo apt install build-essential gcc-multilib nasm qemu-system-x86
make deb=1
make run
```

## Authors

-   **keldsaya** - Initial development and maintenance.
-   **oguzokdotdev** - Documentation and porting (first contributor)
