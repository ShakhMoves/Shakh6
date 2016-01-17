/*
 * In The Name Of God
 * ========================================
 * [] File Name : checkpoint_test.c
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
#include "checkpoint_u.h"
#include "user.h"

int main() {
	int i;
	for (i = 0; i < 10; i++)
		printf(1, "%d\n", i);

	if (checkpoint_u() < 0) {
		printf(2, "everything went wrong ...\n");
		exit();
	}

    	for (; i < 20; i++)
        	printf(1, "%d\n", i);

       	exit();
}
