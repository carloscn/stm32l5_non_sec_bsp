/**
 * @file fault_ns.c
 * @brief NS fault handlers that report before halting (bring-up only).
 *
 * Only in the TF-M NS build. Prints CFSR + the stacked exception frame,
 * then spins (does NOT reset) so a serial capture shows why a fault fired.
 */
#if defined(TFM_NS)

#include "osal_log.h"
#include <stdint.h>

/* Called from the naked handlers with r0 = tag id, r1 = stacked frame ptr. */
void fault_ns_report(uint32_t tag, uint32_t *frame)
{
    static const char *const k_names[] = {
        "HardFault", "MemManage", "BusFault", "UsageFault"
    };
    volatile uint32_t *scb = (volatile uint32_t *)0xE000ED00;
    const char *name = (tag < 4U) ? k_names[tag] : "?";

    osal_log_printf("FAULT %s  CFSR=%08lX HFSR=%08lX MMFAR=%08lX BFAR=%08lX",
                    name,
                    (unsigned long)scb[0x28 / 4],
                    (unsigned long)scb[0x2C / 4],
                    (unsigned long)scb[0x34 / 4],
                    (unsigned long)scb[0x38 / 4]);
    osal_log_printf("  R0=%08lX R1=%08lX R2=%08lX R3=%08lX R12=%08lX",
                    (unsigned long)frame[0], (unsigned long)frame[1],
                    (unsigned long)frame[2], (unsigned long)frame[3],
                    (unsigned long)frame[4]);
    osal_log_printf("  LR=%08lX PC=%08lX xPSR=%08lX",
                    (unsigned long)frame[5], (unsigned long)frame[6],
                    (unsigned long)frame[7]);
    for (;;) {
    }
}

#define FAULT_ENTRY(fn, id)                             \
    __attribute__((naked)) void fn(void)               \
    {                                                  \
        __asm volatile (                               \
            "movs  r0, #" #id "     \n"                \
            "tst   lr, #4           \n"                \
            "ite   eq              \n"                 \
            "mrseq r1, msp         \n"                 \
            "mrsne r1, psp         \n"                 \
            "b     fault_ns_report \n");               \
    }

FAULT_ENTRY(HardFault_Handler,  0)
FAULT_ENTRY(MemManage_Handler,  1)
FAULT_ENTRY(BusFault_Handler,   2)
FAULT_ENTRY(UsageFault_Handler, 3)

#endif /* TFM_NS */
