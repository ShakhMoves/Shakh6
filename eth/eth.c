#include "../types.h"
#include "../defs.h"
#include "../fs.h"
#include "../file.h"
#include "../traps.h"
#include "ne.h"
#include "eth.h"

static ne_t ne;

void
ethintr() {
    ne_interrupt(&ne);
    return;
}

// not used because ioctl() isn't still implemented.
int
ethioctl(struct inode *ip, int request, void *p) {
    switch (request) {
        case ETH_IPC_SETUP:
//    cprintf("%s: ETH_IPC_SETUP isn't still implemented because of no IPC\n", ne.name);
            break;
    }
    return 0;
}

int
ethread(struct inode *ip, char *p, int n) {
    return ne_pio_read(&ne, (uchar *) p, n);
}

int
ethwrite(struct inode *ip, char *p, int n) {
    return ne_pio_write(&ne, (uchar *) p, n);
}

void
ethinit() {
    int i;
    int f = 0;
    char name[] = "eth#";
    //Ethernet Auto-Probing <http://www.tldp.org/LDP/nag/node50.html>
    int ports[] = {0xC100, 0x240, 0x280, 0x320, 0x340, 0x360};

    devsw[ETHERNET].write = ethwrite;
    devsw[ETHERNET].read = ethread;
    devsw[ETHERNET].ioctl = ethioctl;

    for (i = 0; i < NELEM(ports); ++i) {
        memset(&ne, 0, sizeof(ne));
        name[3] = '0' + i;
        strncpy(ne.name, name, strlen(name) + 1);
        ne.irq = IRQ_ETH;
        ne.base = ports[i];
        if (ne_probe(&ne)) {
            cprintf("Ethernet: Initialize port %d [0x%x].\n", i, ports[i]);
            ne_init(&ne);
            picenable(ne.irq);
            ioapicenable(ne.irq, 0);
            f = 1;
            break;
        }
    }

    if (!f)
        cprintf("Ethernet: Initialize failed!\n");
    return;
}
