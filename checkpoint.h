/*
 * In The Name Of God
 * ========================================
 * [] File Name : checkpoint_u.h
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
#include "proc.h"
#include "x86.h"

#ifdef CHECKPOINT_H
#define CHECKPOINT_H

struct checkpoint_t {
	struct proc p;
	struct trapframe tf;
	char *pages;
	uint *flags;
};

#endif
