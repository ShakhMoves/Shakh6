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

// Create PTEs for virtual addresses starting at va that refer to
// physical addresses starting at pa. va and size might not
// be page-aligned.
static int mappages(pde_t *pgdir, void *va, uint size, uint pa, int perm)
{
	char *a, *last;
	pte_t *pte;

	a = (char*)PGROUNDDOWN((uint)va);
  	last = (char*)PGROUNDDOWN(((uint)va) + size - 1);
  	for(;;){
		if((pte = walkpgdir(pgdir, a, 1)) == 0)
			return -1;
		if(*pte & PTE_P)
			panic("remap");
		*pte = pa | perm | PTE_P;
		if(a == last)
			break;
		a += PGSIZE;
		pa += PGSIZE;
	}
	return 0;
}

static int restore(struct checkpoint_t *ch)
{
	pde_t *d;
	uint i;
	char *mem;

	if((d = setupkvm()) == 0)
		return -1;
	
	for(i = 0; i < ch->p.sz; i += PGSIZE){
		if((mem = kalloc()) == 0)
			goto bad;
		memmove(mem, (ch->pages) + i, PGSIZE);
		if(mappages(d, (char*)i, PGSIZE, v2p(mem), (ch->flags)[i / PGSIZE]) < 0)
			goto bad;
	}

	struct proc np;
	struct trapframe tf;

	np.pgdir = d;
	tf = ch->tf;
	*np.tf = tf;
	return procload(&np);
bad:
	freevm(d);
	return -1;
}

int sys_restore(void)
{
	char *p = 0;
	if (argptr(0, &p, sizeof(struct checkpoint_t)) < 0)
		return -1;
	return restore((struct checkpoint_t *)p);
}
