#ifndef TUNALLOC_H
#define TUNALLOC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h> 
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

int tun_alloc(char *dev);
int reader_char(int fdtun, char *buf, int n);
int reader(int fdtun, char *buf, int n);
int writer(int fdtun, char *buf, int n);
void recopie(int src, int dst);


#endif