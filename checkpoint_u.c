/*
 * In The Name Of God
 * ========================================
 * [] File Name : checkpoint_u.c
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

int checkpoint_u(void)
{
    	// Step 1: Check point process and trapframe
    	struct checkpoint_t *ch;
	int sel;
    	ch = malloc(sizeof(struct checkpoint_t));
	sel = checkpoint_proc(ch);
	if (sel < 0) {
		printf(2, "Process check point failed\n");
		return -1;
    	} else if (sel > 0) {
		free(ch);
		return 0;	
	}

    	// Step 2: Check point process memory
    	ch->pages = malloc(ch->p.sz);
    	ch->flags = malloc(sizeof(uint) * (ch->p.sz / PGSIZE + 1));
    	if (checkpoint_mem(ch) != 0) {
		printf(2, "Memory check point failed\n");
		return -1;
    	}

	char name[32];
	strcpy(name, ch->p.name);
	strcpy(name + strlen(name), ".bin");

    	int fd = open(name, O_WRONLY | O_CREATE);
	if((write(fd, &ch->p, sizeof(struct proc)) < sizeof(struct proc)) || // Serialize Proc
			(write(fd, &ch->tf, sizeof(struct trapframe)) < sizeof(struct trapframe)) || // Serialize TrapFrame
			(write(fd, ch->pages, ch->p.sz) < ch->p.sz) || // Serialize pages
			(write(fd, ch->flags, (ch->p.sz / PGSIZE + 1) * sizeof(uint))
			 	< (ch->p.sz / PGSIZE + 1) * sizeof(uint))) { // Serialize pages flags
    		printf(2, "everything went wrong\n");
		return -1;
    	}
	free(ch->pages);
    	free(ch->flags);
    	free(ch);

	return 0;
}

int restore_u(char *name)
{
	struct checkpoint_t *ch;
	
	ch = malloc(sizeof(struct checkpoint_t));
	
	char nname[32];
	strcpy(nname, name);
	strcpy(nname + strlen(nname), ".bin");

	int fd = open(nname, O_RDONLY);
	if((read(fd, &ch->p, sizeof(struct proc)) < sizeof(struct proc)) || // DeSerialize Proc
			(read(fd, &ch->tf, sizeof(struct trapframe)) < sizeof(struct trapframe))) { // DeSerialize TrapFrame
		printf(2, "everything went wrong\n");	
		return -1;
	}
	
	ch->pages = malloc(ch->p.sz);
	ch->flags = malloc(sizeof(uint) * (ch->p.sz / PGSIZE + 1));

	if ((read(fd, ch->pages, ch->p.sz) < ch->p.sz) || // DeSerialize pages
		(read(fd, ch->flags, (ch->p.sz / PGSIZE + 1) * sizeof(uint)) < (ch->p.sz / PGSIZE + 1) * sizeof(uint))) {
		printf(2, "everything went wrong\n");	
		return -1;
	}
	close(fd);

	restore(ch);
	
	wait();
	
	free(ch->pages);
    	free(ch->flags);
    	free(ch);

	exit();

}
