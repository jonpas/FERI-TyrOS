; Sets up multiboot configuration for GRUB

MBOOT_PAGE_ALIGN    equ 1<<0        ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1        ; Provide kernel with memory info
MBOOT_HEADER_MAGIC  equ 0x1BADB002  ; Multiboot magic value (identify kernel as multiboot-compatible)
; We do not use MBOOT_AOUT_KLUDGE, meaning GRUB does not pass us a symbol table
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS) ; Error checking (must be 0)


[BITS 32]                   ; All instructions 32-bit

[GLOBAL mboot]              ; Make 'mboot' accessible from C
[EXTERN code]               ; Start of the '.text' section
[EXTERN bss]                ; Start of the '.bss' section
[EXTERN end]                ; End of last loadable section

section .mbheader

; Embed constants into kernel
mboot:
    dd  MBOOT_HEADER_MAGIC
    dd  MBOOT_HEADER_FLAGS
    dd  MBOOT_CHECKSUM
    dd  mboot               ; Address of structure we are currently writing (for relocations)
    ; Linker defined, defines sections where kernel can be located
    dd  code
    dd  bss
    dd  end
    dd  start

section .text

[GLOBAL start]              ; Kernel entry point
[EXTERN main]               ; C code entry point

start:
    ; Load multiboot information
    push    ebx             ; Save mboot information structure
    cli                     ; Disable interrupts
    call    main            ; Call 'main()' C function
    jmp     $               ; Continue loop
