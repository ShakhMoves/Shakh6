/*
 * In The Name Of God
 * ========================================
 * [] File Name : popatest.c
 *
 * [] Creation Date : 13-01-2016
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2016 Parham Alvani.
*/
#include "proc.c"
#include "ulib.c"

int main()
{
	struct proc *p = malloc(sizeof(struct proc));

	checkpoint(p);
}
