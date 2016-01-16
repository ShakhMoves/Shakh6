/*
 * In The Name Of God
 * ========================================
 * [] File Name : popatest.c
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


int main()
{
	int i;
	struct proc *p;
	void *pg;
	
	p = malloc(sizeof(struct proc));
	
	for (i = 0; i < 10; i++)
		printf(1, "%d\n", i);

	if (checkpoint_proc(p) == 0) {
		pg = malloc(p->sz + sizeof(struct trapframe));
	
		if (checkpoint_mem(pg, p->sz) == 0) {
			printf(1, "checkpoint_mem successfuly..\n");
			int fd = open("checkpoint.bin", O_WRONLY | O_CREATE);
			if (write(fd, p, sizeof(struct proc)) < sizeof(struct proc))
				printf(2, "everything went wrong\n");
			if (write(fd, pg + p->sz, sizeof(struct trapframe)) < sizeof(struct trapframe))
				printf(2, "everything went wrong\n");
			if (write(fd, pg, p->sz) < p->sz)
				printf(2, "everything went wrong\n");
			printf(1, "write was Done on %d\n", fd);
		
		}
	}

	for (; i < 20; i++)
		printf(1, "%d\n", i);

	free(pg);
	free(p);

	exit();
}
