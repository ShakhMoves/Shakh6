/*
 * In The Name Of God
 * ========================================
 * [] File Name : resotre_test.c
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
#include "mmu.h"
#include "memlayout.h"
#include "checkpoint.h"

int main()
{
	struct checkpoint_t *ch;
	
	ch = malloc(sizeof(struct checkpoint_t));
	
	int fd = open("checkpoint.bin", O_RDONLY);
	if((read(fd, &ch->p, sizeof(struct proc)) < sizeof(struct proc)) || // DeSerialize Proc
			(read(fd, &ch->tf, sizeof(struct trapframe)) < sizeof(struct trapframe))) { // DeSerialize TrapFrame
		printf(2, "everything went wrong\n");	
	}
	
	ch->pages = malloc(ch->p.sz);
	ch->flags = malloc(sizeof(uint) * (ch->p.sz / PGSIZE + 1));

	if ((read(fd, ch->pages, ch->p.sz) < ch->p.sz) || // DeSerialize pages
		(read(fd, ch->flags, (ch->p.sz / PGSIZE + 1) * sizeof(uint)) < (ch->p.sz / PGSIZE + 1) * sizeof(uint))) {
		printf(2, "everything went wrong\n");	
	}
 	printf(1, "we restore from addr: %p\n", ch->tf.eip);
	printf(1, "ready for restore on %d\n", fd);
	close(fd);
	
	restore(ch);

	wait();
	exit();
}
