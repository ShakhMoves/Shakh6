/*
 * In The Name Of God
 * ========================================
 * [] File Name : restore.c
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
#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"

static int restore(char *name, struct proc *p)
{
	return procload(p, name);
}

int sys_restore(void)
{
	char *name = 0;
	char *p = 0;
	if (argstr(0, &name) < 0)
		return -1;
	if (argptr(1, &p, sizeof(struct proc)) < 0)
		return -1;
	return restore(name, (struct proc *)p);
}
