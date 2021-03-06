# FERI-TyrOS

Operating System project for seminar paper at Operating Systems (slo. Operacijski Sistemi)

Týr - Germanic god associated with law and heroic glory in Norse mythology, portrayed as **one-handed**.

### Features

- Multiboot (via GRUB2)
- Global Descriptor Table
- Interrupt Descriptor Table
- Interrupt Requests
- Programmable Interval Timer
- Monitor Interface
- Keyboard Interface/Driver
- Panic
- Paging
- Heap
- Multitasking (partially implemented)
- User Mode (partially implemented due to Multitasking)
- System Call Interface

### Setup

```
$ make          # Build
$ make bochs    # Build and run using bochs
$ make qemu     # Build and run using qemu
$ make debug    # Build and run using qemu in gdb
$ make clean    # Clean build files
```

**Dependencies:**
- GCC
- NASM
- GRUB2
- xorriso
- x86 (i386) hardware or emulator:
  - bochs (with `--with-sdl` - `PKGBUILD` edit on Arch Linux, `bochs-sdl` package on Ubuntu)
  - QEMU

### Resources

- [OSDev.org](http://wiki.osdev.org)
- [James Molloy's Kernel Development Tutorials](http://www.jamesmolloy.co.uk/tutorial_html/index.html)
- [James Molloy's Kernel Development Tutorials (Improved Source)](https://github.com/cirosantilli/jamesmolloy-kernel-development-tutorials)
- [James Molloy's Tutorial Known Bugs](http://wiki.osdev.org/James_Molloy%27s_Tutorial_Known_Bugs)
- [Bran's Kernel Development Tutorial](http://www.osdever.net/bkerndev/Docs/gettingstarted.htm)
- [Writing a Simple Operating System - from Scratch](http://www.cs.bham.ac.uk/%7Eexr/lectures/opsys/10_11/lectures/os-dev.pdf)
