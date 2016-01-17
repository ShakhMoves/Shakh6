# Shakh6 OS : A fork of xv6 operating system

```

--------------------------------------------------------------------
     /------| __    __       ___       __  ___  __    ___    __
    /       ||  |  |  |     /   \     |  |/  / |  |  |  |   / /
   |   (----'|  |__|  |    /  ^  \    |  '  /  |  |__|  |  / /_
    \	\    |  |__|  |   /  ( )  \   |  . \   |  |__|  | (    \
.----)   |   |  |  |  |  /  _____  \  |  |  \  |  |  |  | | (_) |
|_______/    |__|  |__| /__/     \__\ |__|\__\ |__|  |__|  \___/
--------------------------------------------------------------------

```

## What's new

### Process CheckPoint/Migration/Restore

Ability to save process current state to filesystem and restore later. following
program show how to use our checkpoint and restore feature.
```c
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
```
```c
#include "user.h"
#include "checkpoint_u.h"

int main()
{
	restore_u("checkpoint_test");
	exit();
}
```

### Basic netwroking support for N2K drivers

Basic networking support (bochs only)

### Halt/Shutdown

Halt function added (it may or may not work!)
   
   
*** for a list of changelog see: [project-changes.log](project-changes.log) ***

## Contributers

- Parham Alvani <parham.alvani@gmail.com> ([Github](https://github.com/1995parham))
- Pooya Parsa <pooya@pi0.ir> ([Github](https://github.com/pi0))

## Original ReadMe
### Introduntion
xv6 is a re-implementation of Dennis Ritchie's and Ken Thompson's Unix
Version 6 (v6).  xv6 loosely follows the structure and style of v6,
but is implemented for a modern x86-based multiprocessor using ANSI C.

### Acknowledgments

xv6 is inspired by John Lions's Commentary on UNIX 6th Edition (Peer
to Peer Communications; ISBN: 1-57398-013-7; 1st edition (June 14,
2000)). See also [this](http://pdos.csail.mit.edu/6.828/2014/xv6.html), which
provides pointers to on-line resources for v6.

#### xv6 borrows code from the following sources:
* JOS (asm.h, elf.h, mmu.h, bootasm.S, ide.c, console.c, and others)
* Plan 9 (entryother.S, mp.h, mp.c, lapic.c)
* FreeBSD (ioapic.c)
* NetBSD (console.c)

#### The following people have made contributions:
* Russ Cox (context switching, locking)
* Cliff Frey (MP)
* Xiao Yu (MP)
* Nickolai Zeldovich
* Austin Clements

In addition, we are grateful for the bug reports and patches contributed by
Silas Boyd-Wickizer, Peter Froehlich, Shivam Handa, Anders Kaseorg, Eddie
Kohler, Yandong Mao, Hitoshi Mitake, Carmi Merimovich, Joel Nider, Greg Price,
Eldar Sehayek, Yongming Shen, Stephen Tu, and Zouchangwei.

The code in the files that constitute xv6 is
Copyright 2006-2014 Frans Kaashoek, Robert Morris, and Russ Cox.

### Error Reports

If you spot errors or have suggestions for improvement, please send
email to Frans Kaashoek and Robert Morris (kaashoek,rtm@csail.mit.edu). 

### Building and Running xv6

* To build xv6 on an x86 ELF machine (like Linux or FreeBSD), run `make`.
On non-x86 or non-ELF machines (like OS X, even on x86), you will
need to install a cross-compiler gcc suite capable of producing x86 ELF
binaries.  See [here](http://pdos.csail.mit.edu/6.828/2014/tools.html)
Then run `make TOOLPREFIX=i386-jos-elf-`.

* To run xv6, install the QEMU PC simulators.  To run in QEMU, run `make qemu`.

* To create a typeset version of the code, run `make xv6.pdf`.  This
requires the "mpage" utility.  See [here](http://www.mesa.nl/pub/mpage/).
