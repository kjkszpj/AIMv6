//
// Created by Peijie You on 15/11/14.
//

#include <kern/init/init.h>
#include <kern/mm/pages_manage.h>
#include <kern/mm/slb.h>
#include <interrupt.h>
#include <stdio.h>

u32 *page_table = (u32*)(KERNEL_BASE + PT_OFFSET);

// TODO, for now it is more like arch_init()

void kinit()
{
    uart_spin_puts("GE\r\n\0");
//    turn on hardware
    if (mmu_high_main() == 0) uart_spin_puts("MMU done.\r\n\0");
    if (l1cache_init() == 0) uart_spin_puts("L1 cache done.\r\n\0");
    if (scu_init() == 0) uart_spin_puts("SCU done.\r\n\0");
//    init mm
    if (init_pspace() == 0) uart_spin_puts("pages manage done.\r\n\0");
    if (slb_init() == 0) uart_spin_puts("slb manage done.\r\n\0");
//    init interrupt
    if (interrupt_init() == 0) uart_spin_puts("int done.\r\n\0");
    
//    uart_spin_puts("now trying snprintf\r\n\0");
//    char temp[100];
//    snprintf(temp, 90, "decimal:\t%d %d\r\n\0", 123, 0xFFFD0FA0);
//    uart_spin_puts(temp);

    volatile u32 i;
    volatile u32 a;
    for (i = 0; i < 10; i++) a = i;
    uart_spin_puts("------Live to tell the story------\r\n\0");
    while (1);
}
