/*
 * In The Name Of God
 * ========================================
 * [] File Name : checkpoint.c
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

// Create a new process copying p as the parent.
// Sets up stack to return as if from system call.
static int checkpoint(struct proc *p)
{
	// Allocate process.
  	//if((checkpoint_proc = allocproc()) == 0)
	//	return -1;

  	// Copy process state from p.
  	if((p->pgdir = copyuvm(proc->pgdir, proc->sz)) == 0){
    		kfree(p->kstack);
    		p->kstack = 0;
    		p->state = UNUSED;
    		return -1;
  	}
  	p->sz = proc->sz;
  	p->parent = proc;
  	// *p->tf = *proc->tf;

  	// Clear %eax so that fork returns 0 in the child.
	// p->tf->eax = 0;

	/*
  	for(i = 0; i < NOFILE; i++)
		if(proc->ofile[i])
			p->ofile[i] = filedup(proc->ofile[i]);
  	p->cwd = idup(proc->cwd);
	*/

  	safestrcpy(p->name, proc->name, sizeof(proc->name));
	cprintf("Your process name was: %s\n", p->name);

	return 0;
}

int sys_checkpoint(void)
{
	char *p = 0;
	if (argptr(0, &p, sizeof(struct proc)) < 0)
		return -1;
	return checkpoint((struct proc *) p);
}

