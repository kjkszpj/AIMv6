//
// Created by Peijie You on 15/12/10.
//

//  todo, page table domain

#ifndef AIMV6_PCB_H
#define AIMV6_PCB_H

#include <config.h>
#include <kern/mm/pte.h>

//  task descriptor
typedef struct
{
    u32 pid;
    u32 ppid;

    char name[0x8];
} taskd_t;

//  process status
typedef enum
{
    NEW=0, READY, RUN, WAIT, DONE, ABORT
} proc_status_t;

//  page global directory?
//  l1 page table is 16kb, todo don't know if it is the same across arch
typedef struct
{
    u32 pte[0x1000];
} pgd_t;

//  cpu context, arch related, todo move it to arch/
typedef struct
{
//---order here is weird, because we want to use stmia when interrupt
    u32 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12;
    u32 pc;
    u32 cpsr, spsr;
    u32 sp, lr;
} context_cpu_t;

typedef struct
{
    u32 info;
} sched_info_t;

//  process control block
typedef struct pcb_t
{
//    small object first, for locality?
    taskd_t         td;
    proc_status_t   status;
    u32             ret_val;

    pgd_t*          page_table;
    context_cpu_t   cpu;
    sched_info_t    sched_info;
} pcb_t;

#endif //AIMV6_PCB_H
