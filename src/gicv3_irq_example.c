/*
 * gicv3_irq_example.c
 *
 * Minimal AArch64 (EL1) bare-metal example for GICv3/GICv4 interrupt handling.
 *
 * IMPORTANT: Adjust the GICD_BASE and GICR_BASE platform addresses for your board.
 *
 * Build (example):
 *   aarch64-none-elf-gcc -nostdlib -static -ffreestanding -O2 \
 *       -march=armv8-a -mcpu=cortex-a57 -o gic_example.elf \
 *       gicv3_irq_example.c -T link.ld
 *
 * (link.ld should set entry point to start and place .text/.bss appropriately)
 */

#include <stdint.h>
#include <stddef.h>

/* --- PLATFORM-SPECIFIC: set these to your platform's GIC addresses --- */
/* Example placeholders (NOT GENERIC): replace with your platform values. */
#define GICD_BASE   0x08000000UL   /* GIC Distributor base */
#define GICR_BASE   0x080A0000UL   /* GIC Redistributor base (per-CPU region) */

/* --- GIC Distributor register offsets (a subset) --- */
#define GICD_CTLR           0x000
#define GICD_TYPER          0x004
#define GICD_IIDR           0x008
#define GICD_ISENABLER(n)   (0x100 + ((n) * 4))
#define GICD_ICENABLER(n)   (0x180 + ((n) * 4))
#define GICD_ISPENDR(n)     (0x200 + ((n) * 4))
#define GICD_IPRIORITYR(n)  (0x400 + ((n) * 4))
#define GICD_ITARGETSR(n)   (0x800 + ((n) * 4))

/* --- GIC Redistributor offsets (per-CPU) --- */
#define GICR_WAKER          0x0014
#define GICR_IGROUPR0       0x0080
#define GICR_ISENABLER0     0x0180
#define GICR_ICENABLER0     0x01C0
#define GICR_IPRIORITYR     0x0400

/* WAKER bits */
#define GICR_WAKER_PROC_SLEEP   (1 << 1)
#define GICR_WAKER_CHILDREN_ASLEEP (1 << 2)

/* Memory-mapped IO helpers */
static inline void mmio_write32(uintptr_t addr, uint32_t val) {
    *((volatile uint32_t *)addr) = val;
}
static inline uint32_t mmio_read32(uintptr_t addr) {
    return *((volatile uint32_t *)addr);
}

/* Simple data memory barrier / instruction barrier for AArch64 */
static inline void dsb_sev(void) { __asm__ volatile("dsb sy" ::: "memory"); }
static inline void isb(void)     { __asm__ volatile("isb" ::: "memory"); }

/* --- GIC CPU interface uses system registers (GICv3) --- */

/* Read Interrupt Acknowledge Register (gets active interrupt ID) */
static inline uint32_t read_icc_iar1_el1(void) {
    uint64_t val;
    __asm__ volatile("mrs %0, ICC_IAR1_EL1" : "=r"(val));
    return (uint32_t)val;
}

/* Write End of Interrupt (EOI) */
static inline void write_icc_eoir1_el1(uint32_t val) {
    __asm__ volatile("msr ICC_EOIR1_EL1, %0" :: "r"((uint64_t)val));
}

/* Write Running Priority (optional) */
static inline void write_icc_rpr1_el1(uint32_t val) {
    __asm__ volatile("msr ICC_RPR1_EL1, %0" :: "r"((uint64_t)val));
}

/* Enable group1 interrupts at the CPU interface */
static inline void write_icc_igrpen1_el1(uint64_t v) {
    __asm__ volatile("msr ICC_IGRPEN1_EL1, %0" :: "r"(v));
    isb();
}

/* Enable system register interface for GIC (ICC_SRE_EL1) */
static inline void write_icc_sre_el1(uint64_t v) {
    __asm__ volatile("msr ICC_SRE_EL1, %0" :: "r"(v));
    isb();
}

/* Read processor affinity (affinity value) useful for Redistributor base selection */
static inline uint64_t read_mpidr_el1(void) {
    uint64_t v;
    __asm__ volatile("mrs %0, MPIDR_EL1" : "=r"(v));
    return v;
}

/* A minimal stall: crude busy-wait */
static void udelay(unsigned int loops) {
    for (volatile unsigned int i = 0; i < loops; ++i) { __asm__ volatile("nop"); }
}

/* --- Interrupt handling: assembly vector + C handler --- */

/* Stack and symbols for start */
extern void start(void);
void default_el1_vector(void);

/* Reserve a stack for EL1 */
#define STACK_SIZE 0x4000
static uint8_t el1_stack[STACK_SIZE] __attribute__((aligned(16)));

__attribute__((noreturn)) void start(void) {
    /* Basic stack pointer setup for EL1 */
    __asm__ volatile(
        "mov x0, %0\n\t"
        "mov sp, x0\n\t"
        : : "r"(&el1_stack[STACK_SIZE]) : "x0"
    );

    /* Setup vector base address (VBAR_EL1) to our vector table */
    __asm__ volatile("adrp x0, default_el1_vector\n\t"
                     "add x0, x0, :lo12:default_el1_vector\n\t"
                     "msr VBAR_EL1, x0\n\t"
                     "isb\n\t" ::: "x0");

    /* Initialize console or UART if you have one (omitted) */

    /* Initialize and enable GIC */
    // The main C function
    extern void main_c(void);
    main_c();

    /* Halt if main returns */
    while (1) { __asm__ volatile("wfe\n"); }
}

/* ---- Exception vector table for EL1 (AArch64) ----
   We provide only IRQ (asynchronous) entry that routes to irq_entry.
   The vector table layout must match ARM ARM for AArch64.
*/
__attribute__((section(".vectors"))) __attribute__((aligned(2048)))
const uint8_t default_el1_vector[] = {
    /* We'll generate code via inline asm below using a function label */
};

/* We'll emit the vector code using asm so labels are available. */
__asm__ (
".section .vectors, \"ax\", %progbits\n"
".align 11\n" /* 2KB aligned as recommended */\n
"default_el1_vector:\n"
"    // Synchronous from current EL with SP_EL0\n"
"    b sync_from_current_el\n"
"    // IRQ from current EL with SP_EL0\n"
"    b irq_from_current_el\n"
"    // FIQ from current EL with SP_EL0\n"
"    b fiq_from_current_el\n"
"    // SError from current EL with SP_EL0\n"
"    b serr_from_current_el\n"
"    // Synchronous from lower EL with SP_EL0\n"
"    b sync_from_lower_el\n"
"    // IRQ from lower EL with SP_EL0\n"
"    b irq_from_lower_el\n"
"    // FIQ from lower EL with SP_EL0\n"
"    b fiq_from_lower_el\n"
"    // SError from lower EL with SP_EL0\n"
"    b serr_from_lower_el\n"
"\n"
"// We keep all vectors to branch to a common handler where appropriate\n"
"irq_from_current_el:\n"
"    // Save minimal context (caller-saved) and branch to C handler\n"
"    stp x0, x1, [sp, #-16]!\n"
"    stp x2, x3, [sp, #-16]!\n"
"    // call irq_entry\n"
"    bl irq_entry\n"
"    ldp x2, x3, [sp], #16\n"
"    ldp x0, x1, [sp], #16\n"
"    eret\n"
"\n"
"irq_from_lower_el:\n"
"    stp x0, x1, [sp, #-16]!\n"
"    stp x2, x3, [sp, #-16]!\n"
"    bl irq_entry\n"
"    ldp x2, x3, [sp], #16\n"
"    ldp x0, x1, [sp], #16\n"
"    eret\n"
"\n"
"// Provide dummy labels for unused vectors\n"
"sync_from_current_el: b default_sync_handler\n"
"fiq_from_current_el: b default_fiq_handler\n"
"serr_from_current_el: b default_serr_handler\n"
"sync_from_lower_el: b default_sync_handler\n"
"fiq_from_lower_el: b default_fiq_handler\n"
"serr_from_lower_el: b default_serr_handler\n"
"\n"
"default_sync_handler:\n"
"    // For simplicity spin\n"
"1: wfe; b 1b\n"
"\n"
"default_fiq_handler:\n"
"    b default_sync_handler\n"
"\n"
"default_serr_handler:\n"
"    b default_sync_handler\n"
);

/* Forward declaration of C entry */
void irq_entry(void);

/* C IRQ entry: called from vector assembly */
void irq_entry(void) {
    /* Read interrupt acknowledge to get interrupt ID from ICC */
    uint32_t int_ack = read_icc_iar1_el1();
    uint32_t int_id  = int_ack & 0xFFFFFF; /* lower bits hold ID */

    /* Simple demo: handle a few IDs (platform dependent) */
    // Print or toggle an LED here (platform-specific). Omitted.

    /* Example: acknowledge and end interrupt handling */
    // --- Handle interrupt ID ---
    // (application-specific handling would be here)
    // For demo we'll just mark EOI and return.

    write_icc_eoir1_el1(int_ack);

    /* Optionally de-activate: on GICv3 the EOIR suffices to mark end */
    dsb_sev();
}

/* --- GIC initialization and helper functions --- */

/* Wake up the Redistributor (per-CPU) */
static void gicr_wake(uintptr_t gicr_base) {
    uint32_t val = mmio_read32(gicr_base + GICR_WAKER);
    val &= ~GICR_WAKER_PROC_SLEEP;
    mmio_write32(gicr_base + GICR_WAKER, val);

    /* Wait until ChildrenAsleep bit is clear */
    while (mmio_read32(gicr_base + GICR_WAKER) & GICR_WAKER_CHILDREN_ASLEEP) {
        /* spin */
    }
}

/* Basic Distributor enable */
static void gicd_enable(uintptr_t gicd_base) {
    /* Set GICD_CTLR: enable group1 and group0 as needed.
     * For a basic setup enable by writing 1 (the precise bits depend on spec).
     */
    mmio_write32(gicd_base + GICD_CTLR, 0x1);
    dsb_sev();
}

/* Enable a global interrupt in Distributor (ID) */
static void gicd_enable_interrupt(uintptr_t gicd_base, uint32_t int_id) {
    uint32_t reg = int_id / 32;
    uint32_t bit = int_id % 32;
    mmio_write32(gicd_base + GICD_ISENABLER(reg * 4), (1u << bit));
}

/* Set interrupt priority (optional) */
static void gicd_set_priority(uintptr_t gicd_base, uint32_t int_id, uint8_t prio) {
    uintptr_t addr = gicd_base + GICD_IPRIORITYR + (int_id / 4) * 4;
    uint32_t shift = (int_id % 4) * 8;
    uint32_t val = mmio_read32(addr);
    val &= ~(0xFFu << shift);
    val |= ((uint32_t)prio << shift);
    mmio_write32(addr, val);
}

/* Set CPU targets (for GICv2/GICv3 legacy, in GICv3 it's mostly ignored) */
static void gicd_set_target(uintptr_t gicd_base, uint32_t int_id, uint8_t cpu_mask) {
    uintptr_t addr = gicd_base + GICD_ITARGETSR((int_id / 4));
    uint32_t shift = (int_id % 4) * 8;
    uint32_t val = mmio_read32(addr);
    val &= ~(0xFFu << shift);
    val |= ((uint32_t)cpu_mask << shift);
    mmio_write32(addr, val);
}

/* Per-CPU: Enable CPU interface and enable Group1 interrupts for EL1 */
static void gic_cpuif_enable(void) {
    /* Enable system register access to GIC (ICC_SRE_EL1) */
    write_icc_sre_el1(0x1);

    /* Clear running priority to allow all priorities through (optional) */
    write_icc_rpr1_el1(0);

    /* Enable group1 interrupts at CPU interface (ICC_IGRPEN1_EL1 = 1) */
    write_icc_igrpen1_el1(1);
}

/* Public main in C invoked from start() */
void main_c(void) {
    uintptr_t gicd_base = (uintptr_t)GICD_BASE;
    uintptr_t gicr_base = (uintptr_t)GICR_BASE; /* For real system, compute per-cpu base */

    /* Wake redistributor for this CPU */
    gicr_wake(gicr_base);

    /* Enable Distributor */
    gicd_enable(gicd_base);

    /* Set up CPU interface */
    gic_cpuif_enable();

    /* Example: enable a specific interrupt ID (platform specific)
       Pick some SPIs typically starting at ID 32. We'll enable ID 33 as example. */
    uint32_t example_int = 33; /* platform interrupt to enable */
    gicd_set_priority(gicd_base, example_int, 0x80);
    gicd_set_target(gicd_base, example_int, 0x1); /* target CPU0 (mask bit0 set) */
    gicd_enable_interrupt(gicd_base, example_int);

    /* Enable IRQ exceptions in PSTATE (clear the I bit in DAIF) */
    __asm__ volatile(
        "mrs x0, DAIF\n\t"
        "bic x0, x0, #2\n\t" /* clear I bit (bit 1) to enable IRQ */ 
        "msr DAIF, x0\n\t"
        ::: "x0"
    );

    /* Now main loop: the CPU will jump to irq_entry when interrupts occur */
    while (1) {
        /* Do work, or wait for interrupt */
        __asm__ volatile("wfi");
    }
}

/* Provide minimal required weak symbols or stubs for linking (if needed) */
void _start(void) __attribute__((weak, alias("start")));
