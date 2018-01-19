; Sets up global descriptor table and interrupt descriptor table

[BITS 32]                   ; All instructions 32-bit

[GLOBAL gdt_flush]          ; Make 'gdt_flush()' accessible from C

gdt_flush:
    mov     eax, [esp+4]    ; Get pointer to GDT, passed as a parameter
    lgdt    [eax]           ; Load new GDT pointer

    mov     ax, 0x10        ; 0x10 is offset in GDT to our data segment
    mov     ds, ax          ; Load all data segment selectors
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax
    jmp     0x08:.flush     ; 0x08 is offset to our code segment
.flush:
    ret


[GLOBAL idt_flush]          ; Make 'idt_flush()' accessible from C

idt_flush:
    mov     eax, [esp+4]    ; Get pointer to the IDT, passed as a parameter
    lidt    [eax]           ; Load IDT pointer
    ret


[GLOBAL tss_flush]          ; Make 'tss_flush()' accessible from C

tss_flush:
   mov      ax, 0x2B             ; Load the index of TSS structure (0x28), as it is the 5th selector and each is 8 bytes long,
                            ; but we set the bottom two bits (making 0x2B) so that it has an RPL of 3, not 0
   ltr      ax                   ; Load 0x2B into the task state register
   ret
