.RECIPEPREFIX +=
.POSIX:

BIN = bin
SRC = src
CC = gcc
CFLAGS = -Wextra -fno-builtin -fno-stack-protector -m32 -nostdlib -nostdinc -std=gnu99 -ggdb3
LINK = ld
LINKFLAGS = -melf_i386 -Tlink.ld
CASM = nasm
ASMFLAGS = -f elf
GRUB = grub-mkrescue
GRUBFLAGS = iso

BOOT = iso/boot
ELF = $(BIN)/main.elf
IMG = $(BIN)/main.img

all: $(IMG)

$(IMG): $(ELF)
    @echo " GRUB $<"
    @cp '$<' '$(BOOT)'
    @$(GRUB) -o '$@' $(GRUBFLAGS)

$(ELF): \
        $(patsubst %.c, %.o, $(wildcard $(SRC)/*.c)) \
        $(patsubst %.asm, %.o, $(wildcard $(SRC)/*.asm))
    @echo " LINK $<"
    @$(LINK) -o '$(ELF)' $(LINKFLAGS) $^

$(SRC)/%.o: $(SRC)/%.c
    @mkdir -p $(BIN)
    @echo "  CC  $<"
    @$(CC) $(CFLAGS) -o '$@' -c '$<' $(CLIBS)

$(SRC)/%.o: $(SRC)/%.asm
    @echo " CASM $<"
    @$(CASM) $(ASMFLAGS) $< -o $@

bochs: $(IMG)
    CYLINDERS="$$(($$(stat -c '%s' '$<') / 512))" && \
    bochs -qf /dev/null \
        'ata0-master: type=disk, path="$<", mode=flat, cylinders='"$$CYLINDERS"', heads=1, spt=1' \
        'boot: disk' \
        'display_library: sdl' \
        'megs: 128'

clean:
    rm -rf $(BIN) $(SRC)/*.o $(BOOT)/*.elf

debug: $(IMG)
    qemu-system-i386 -hda '$<' -S -s &
    @gdb $(ELF) -x gdb.gdb

qemu: $(IMG)
    qemu-system-i386 -drive file=$<,format=raw

.PHONY: all bochs clean debug qemu
