/*
 * In The Name Of God
 * ========================================
 * [] File Name : checkpoint_test.c
 *
 * [] Creation Date : 13-01-2016
 *
 * [] Created By : Pooya Parsa (pooya@pi0.ir)
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2016 Parham Alvani and Pooya Parsa.
*/
#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "checkpoint.h"

int main() {
    int i;
    for (i = 0; i < 10; i++)
        printf(1, "%d\n", i);


    // Step 1: Check point process and trapframe
    struct checkpoint_t *ch;
    ch = malloc(sizeof(struct checkpoint_t));
    if (checkpoint_proc(ch) != 0) {
        printf(2, "Process check point failed");
        exit();
    }

    // Step 2: Check point process memory
    ch->pages = malloc(ch->p.sz);
    ch->flags = malloc(sizeof(uint) * (ch->p.sz / PGSIZE + 1));
    if (checkpoint_mem(ch->pages, ch->p.sz) != 0) {
        printf(1, "checkpoint_mem failed..\n");
        exit();
    }

    int fd = open("checkpoint.bin", O_WRONLY | O_CREATE);
    if ((write(fd, &ch->p, sizeof(struct proc)) < sizeof(struct proc)) || // Serialize Proc
        (write(fd, &ch->tf, sizeof(struct trapframe)) < sizeof(struct trapframe)) || // Serialize TrapFrame
        (write(fd, ch->pages, ch->p.sz) < ch->p.sz)) { // Serialize pages
    		printf(2, "everything went wrong\n");
    }
    printf(1, "write was Done on %d\n", fd);


    for (; i < 20; i++)
        printf(1, "%d\n", i);

    free(ch->pages);
    free(ch->flags);
    free(ch);

    exit();

}
