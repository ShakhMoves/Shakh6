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

// Return the address of the PTE in page table pgdir
// that corresponds to virtual address va.  If alloc!=0,
// create any required page table pages.
static pte_t *walkpgdir(pde_t *pgdir, const void *va, int alloc)
{
  pde_t *pde;
  pte_t *pgtab;

  pde = &pgdir[PDX(va)];
  if(*pde & PTE_P){
    pgtab = (pte_t*)p2v(PTE_ADDR(*pde));
  } else {
	  if(!alloc || (pgtab = (pte_t*)kalloc()) == 0)
		  return 0;
    // Make sure all those PTE_P bits are zero.
    memset(pgtab, 0, PGSIZE);
    // The permissions here are overly generous, but they can
    // be further restricted by the permissions in the page table 
    // entries, if necessary.
    *pde = v2p(pgtab) | PTE_P | PTE_W | PTE_U;
  }
  return &pgtab[PTX(va)];
}

static int checkpoint_mem(void *p)
{
	pte_t *pte;
	uint pa, i;
	for(i = 0; i < proc->sz; i += PGSIZE){
		if((pte = walkpgdir(proc->pgdir, (void *) i, 0)) == 0)
			panic("copyuvm: pte should exist");
		if(!(*pte & PTE_P))
			panic("copyuvm: page not present");
		pa = PTE_ADDR(*pte);
		memmove(p + i, (char*)p2v(pa), PGSIZE);
	}
	return 0;
}

static int checkpoint_proc(struct proc *p)
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

int sys_checkpoint_proc(void)
{
	char *p = 0;
	if (argptr(0, &p, sizeof(struct proc)) < 0)
		return -1;
	return checkpoint_proc((struct proc *) p);
}

int sys_checkpoint_mem(void)
{
	char *p = 0;
	if (argptr(0, &p, proc->sz) < 0)
		return -1;
	return checkpoint_mem((void *) p);
}
