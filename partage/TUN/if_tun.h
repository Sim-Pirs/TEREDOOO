#ifndef IF_TUN_H
#define IF_TUN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h> 
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

int reader_char(int fdtun, char *buf, int n);
int reader(int fdtun, char *buf, int n);
int writer(int fdtun, char *buf, int n);
void recopie(int src, int dst);
int tun_alloc(char *dev);

#endif