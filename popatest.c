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
#include "proc.c"
#include "ulib.c"

int main()
{
	struct proc *p = malloc(sizeof(struct proc));

	checkpoint(p);
}
