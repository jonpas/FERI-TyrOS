#include "descriptor_tables.h"
#include "isr.h"

// Get access to ASM functions from C code
extern void gdt_flush(uint);
extern void idt_flush(uint);
extern void tss_flush();

// Internal function prototypes
static void init_gdt();
static void init_idt();
static void gdt_set_gate(int, uint, uint, uchar, uchar);
static void idt_set_gate(uchar, uint, ushort, uchar);
static void write_tss(int, ushort, uint);

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;
idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;
tss_entry_t tss_entry;

// Extern the ISR handler array so we can nullify them on startup
extern isr_t interrupt_handlers[];

// Initialization routine - zeroes all the interrupt service routines, initializes GDT and IDT
void init_descriptor_tables() {
    // Initialize global descriptor table
    init_gdt();

    // Initialize interrupt descriptor table
    init_idt();

    // Nullify all the interrupt handlers
    memset(&interrupt_handlers, 0, sizeof(isr_t) * 256);
}

static void init_gdt() {
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 6) - 1;
    gdt_ptr.base  = (uint)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment
    write_tss(5, 0x10, 0x0);

    gdt_flush((uint)&gdt_ptr);
    tss_flush();
}

// Sets the value of one GDT entry
static void gdt_set_gate(int num, uint base, uint limit, uchar access, uchar gran) {
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access      = access;
}

static void init_idt() {
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base  = (uint)&idt_entries;

    memset(&idt_entries, 0, sizeof(idt_entry_t) * 256);

    // Remap IRQ table
    outb(PIC_MASTER_CMD,  0x11);
    outb(PIC_SLAVE_CMD,   0x11);
    outb(PIC_MASTER_DATA, 0x20);
    outb(PIC_SLAVE_DATA,  0x28);
    outb(PIC_MASTER_DATA, 0x04);
    outb(PIC_SLAVE_DATA,  0x02);
    outb(PIC_MASTER_DATA, 0x01);
    outb(PIC_SLAVE_DATA,  0x01);
    outb(PIC_MASTER_DATA, 0x00);
    outb(PIC_SLAVE_DATA,  0x00);

    idt_set_gate( 0, (uint)isr0 , 0x08, 0x8E);
    idt_set_gate( 1, (uint)isr1 , 0x08, 0x8E);
    idt_set_gate( 2, (uint)isr2 , 0x08, 0x8E);
    idt_set_gate( 3, (uint)isr3 , 0x08, 0x8E);
    idt_set_gate( 4, (uint)isr4 , 0x08, 0x8E);
    idt_set_gate( 5, (uint)isr5 , 0x08, 0x8E);
    idt_set_gate( 6, (uint)isr6 , 0x08, 0x8E);
    idt_set_gate( 7, (uint)isr7 , 0x08, 0x8E);
    idt_set_gate( 8, (uint)isr8 , 0x08, 0x8E);
    idt_set_gate( 9, (uint)isr9 , 0x08, 0x8E);
    idt_set_gate(10, (uint)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint)isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint)isr31, 0x08, 0x8E);
    idt_set_gate(128, (uint)isr128, 0x08, 0x8E);

    // IRQ
    idt_set_gate(32, (uint)isr32, 0x08, 0x8E);
    idt_set_gate(33, (uint)isr33, 0x08, 0x8E);
    idt_set_gate(34, (uint)isr34, 0x08, 0x8E);
    idt_set_gate(35, (uint)isr35, 0x08, 0x8E);
    idt_set_gate(36, (uint)isr36, 0x08, 0x8E);
    idt_set_gate(37, (uint)isr37, 0x08, 0x8E);
    idt_set_gate(38, (uint)isr38, 0x08, 0x8E);
    idt_set_gate(39, (uint)isr39, 0x08, 0x8E);
    idt_set_gate(40, (uint)isr40, 0x08, 0x8E);
    idt_set_gate(41, (uint)isr41, 0x08, 0x8E);
    idt_set_gate(42, (uint)isr42, 0x08, 0x8E);
    idt_set_gate(43, (uint)isr43, 0x08, 0x8E);
    idt_set_gate(44, (uint)isr44, 0x08, 0x8E);
    idt_set_gate(45, (uint)isr45, 0x08, 0x8E);
    idt_set_gate(46, (uint)isr46, 0x08, 0x8E);
    idt_set_gate(47, (uint)isr47, 0x08, 0x8E);
    idt_set_gate(48, (uint)isr48, 0x08, 0x8E);

    idt_flush((uint)&idt_ptr);
}

static void idt_set_gate(uchar num, uint base, ushort sel, uchar flags) {
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel     = sel;
    idt_entries[num].always0 = 0;

    // OR sets the interrupt gate's privilege level to 3
    idt_entries[num].flags   = flags | 0x60;
}

// Initializes our Task State Segment structure
static void write_tss(int num, ushort ss0, uint esp0) {
    // Compute the base and limit of entry into the GDT
    uint base = (uint)&tss_entry;
    uint limit = base + sizeof(tss_entry);

    // Add TSS descriptor's address to the GDT
    gdt_set_gate(num, base, limit, 0xE9, 0x00);

    // Ensure the descriptor is initially zero
    memset(&tss_entry, 0, sizeof(tss_entry));

    tss_entry.ss0  = ss0;  // Set kernel stack segment
    tss_entry.esp0 = esp0; // Set kernel stack pointer

    // Set the cs, ss, ds, es, fs and gs entries in the TSS
    // These specify what segments should be loaded when the processor switches to kernel mode.
    // Therefore they are just our normal kernel code/data segments - 0x08 and 0x10 respectively,
    // but with the last two bits set, making 0x0B and 0x13.
    // The setting of these bits sets the RPL (requested privilege level) to 3,
    // meaning that this TSS can be used to switch to kernel mode from ring 3
    tss_entry.cs = 0x0B;
    tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x13;
}

void set_kernel_stack(uint stack) {
    tss_entry.esp0 = stack;
}
