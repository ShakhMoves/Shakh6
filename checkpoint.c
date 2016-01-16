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
#include "checkpoint.h"

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
static int checkpoint_mem(struct checkpoint_t *ch)
{
	pte_t *pte;
	uint pa, i, flags;
	for(i = 0; i < ch->p.sz; i += PGSIZE){
		cprintf("start write at %p with index %d\n", (ch->pages) + i, i);
		if((pte = walkpgdir(ch->p.pgdir, (void *) i, 0)) == 0)
			panic("checkpoint_mem: pte should exist");
		if(!(*pte & PTE_P))
			panic("checkpoint_mem: page not present");
		pa = PTE_ADDR(*pte);
    		flags = PTE_FLAGS(*pte);
		memmove((ch->pages) + i, (char*)p2v(pa), PGSIZE);
		(ch->flags)[i / PGSIZE] = flags;
		cprintf("write successfuly at %p\n", ch->pages + i);
	}

	return 0;
}

static int checkpoint_proc(struct checkpoint_t *ch)
{
	if ((ch->p.pgdir = copyuvm(proc->pgdir, proc->sz)) == 0) {
		return -1;
	}
  	ch->p.sz = proc->sz;
	ch->tf = *proc->tf;

  	safestrcpy(ch->p.name, proc->name, sizeof(proc->name));
	cprintf("Your process name was: %s\n", ch->p.name);

	return 0;
}

int sys_checkpoint_proc(void)
{
	char *p = 0;
	if (argptr(0, &p, sizeof(struct checkpoint_t)) < 0)
		return -1;
	return checkpoint_proc((struct checkpoint_t *) p);
}

int sys_checkpoint_mem(void)
{
	char *p = 0;
	if (argptr(0, &p, sizeof(struct checkpoint_t)) < 0)
		return -1;
	return checkpoint_mem((void *) p);
}
