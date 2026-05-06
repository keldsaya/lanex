# LANEX

![Lines](https://img.shields.io/badge/lines-~3800-blueviolet?style=flat-square)
![Editor](https://img.shields.io/badge/editor-vim-green?style=flat-square&logo=vim)
![OS](https://img.shields.io/badge/sys-arch-blue?style=flat-square&logo=arch-linux&logoColor=white)
![Gender](https://img.shields.io/badge/dev--gender-female-ff69b4?style=flat-square)

**Lanex** is a monolithic, -nix-like, console-based operating system kernel built entirely from scratch.  
It does not borrow code from other -nix-like systems — everything is original.  
The name "Lanex" originated from a friend's reaction to Linux memes — a simple, memorable twist that stuck.

## Features

- **Protected mode** with basic memory management (PMM, kmalloc)
- **Interrupt handling** (IDT, PIC, PIT, keyboard, RTC)
- **VGA text mode** console with hardware cursor support
- **PS/2 keyboard driver** with arrow keys and modifier support
- **Power management** (shutdown / reboot via QEMU or real hardware)
- **Real Time Clock** driver (date/time readout)
- **Virtual File System (VFS)** with `/proc` pseudo-filesystem
- **Basic interactive shell** with command history and navigation
- **Kernel configuration** system (Kconfig / menuconfig)

### Shell commands

| Command     | Description                          |
|-------------|--------------------------------------|
| `clear`     | Clear screen                         |
| `history`   | Show command history                 |
| `ls`        | List directory (`/proc` supported)   |
| `pwd`       | Show current directory               |
| `cd <path>` | Change directory (`/`, `/proc`)      |
| `shutdown`  | Power off                            |
| `reboot`    | Reboot system                        |
| `date`      | Show RTC date and time               |
| `free`      | Display memory usage                 |
| `uptime`    | Show system uptime                   |
| `echo`      | Print arguments                      |
| `panic`     | Trigger a kernel panic               |
| `help`      | Show this help                       |

## Build & Run

### Arch Linux

```bash
sudo pacman -S nasm qemu-desktop
yay -S i686-elf-binutils i686-elf-gcc
make
make run
```

### Debian-based (Linux Mint, Ubuntu, etc.)

```bash
sudo apt install build-essential gcc-multilib nasm qemu-system-x86
make
make run
```

### Configuration

The kernel can be customized using the Kconfig system:

```bash
make defconfig      # generate default .config
make menuconfig     # interactive configuration menu
```

After changing the configuration, rebuild with `make clean && make`.

## Project Structure

| Directory       | Description                         |
|----------------|-------------------------------------|
| `boot/`         | 16-bit bootloader (real mode)       |
| `kernel/`       | Core kernel (C + asm)               |
| `drivers/`      | VGA, keyboard, RTC, power, ATA      |
| `fs/`           | VFS, procfs, FAT32 (skeleton)       |
| `user/`         | Shell, init, userspace utilities    |
| `libc/`         | Minimal standard library            |
| `include/`      | Public kernel headers               |
| `scripts/`      | Build scripts, QEMU launcher        |
| `tools/`        | `confeditor` – config editor        |

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md).  
**Keep history linear** – no merge commits.

## License

MIT – see [LICENSE](LICENSE).

## Authors

- **keldsaya** – Initial development & maintenance  
- **oguzokdotdev** – Documentation and first contributor
