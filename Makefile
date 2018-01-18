.RECIPEPREFIX +=
.POSIX:

BIN = bin
SRC = src
CC = gcc
CFLAGS = -Wall -Wno-main -fno-builtin -fno-stack-protector -m32 -nostdlib -nostdinc -std=gnu99 -ggdb3
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

$(ELF): \
        $(patsubst %.c, %.o, $(wildcard $(SRC)/*.c)) \
        $(patsubst %.asm, %.o, $(wildcard $(SRC)/*.asm))
    @mkdir -p $(BIN)
    @echo " LINK $@"
    @$(LINK) -o $(ELF) $(LINKFLAGS) $^

$(IMG): $(ELF)
    @echo " GRUB $@"
    @cp '$<' '$(BOOT)'
    @$(GRUB) -o $@ $(GRUBFLAGS)

$(SRC)/%.o: $(SRC)/%.c
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
    qemu-system-i386 -drive file=$<,format=raw -S -s &
    gdb $(ELF) -x gdb.gdb

qemu: $(IMG)
    qemu-system-i386 -drive file=$<,format=raw

.PHONY: all bochs clean debug qemu
