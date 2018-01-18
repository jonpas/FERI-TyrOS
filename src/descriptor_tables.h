// Defines the interface and structures for initialising the GDT and IDT

#pragma once

#include "common.h"

// Value of one GDT entry
// Aattribute 'packed' for GCC not to change any of the alignment in the structure
struct gdt_entry_struct {
    u16int limit_low;       // Lower 16 bits of the limit
    u16int base_low;        // Lower 16 bits of the base
    u8int  base_middle;     // Next 8 bits of the base
    u8int  access;          // Access flags, determine what ring this segment can be used in
    u8int  granularity;
    u8int  base_high;       // Last 8 bits of the base
} __attribute__((packed));

typedef struct gdt_entry_struct gdt_entry_t;

// GDT pointer pointing to the start of array of GDT entries
// In format required by 'lgdt' instruction
struct gdt_ptr_struct {
    u16int limit;           // Upper 16 bits of all selector limits
    u32int base;            // Address of the first gdt_entry_t struct
} __attribute__((packed));

typedef struct gdt_ptr_struct gdt_ptr_t;

// Interrupt gate
struct idt_entry_struct {
    u16int base_lo;         // Lower 16 bits of the address to jump to when this interrupt fires
    u16int sel;             // Kernel segment selector
    u8int  always0;         // Must always be zero
    u8int  flags;
    u16int base_hi;         // Upper 16 bits of the address to jump to
} __attribute__((packed));

typedef struct idt_entry_struct idt_entry_t;

// Pointer to an array of interrupt handlers
// In format suitable for 'lidt'
struct idt_ptr_struct {
    u16int limit;
    u32int base;            // Address of the first element in 'idt_entry_t' array
} __attribute__((packed));

typedef struct idt_ptr_struct idt_ptr_t;

// Extern directives for accessing the addresses of ASM ISR handlers
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

// IRQ
extern void isr32();
extern void isr33();
extern void isr34();
extern void isr35();
extern void isr36();
extern void isr37();
extern void isr38();
extern void isr39();
extern void isr40();
extern void isr41();
extern void isr42();
extern void isr43();
extern void isr44();
extern void isr45();
extern void isr46();
extern void isr47();
extern void isr48();

// Initialization function
void init_descriptor_tables();
