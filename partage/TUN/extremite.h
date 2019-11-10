#ifndef EXTREMITE_H
#define EXTREMIE_H

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
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <netdb.h>
#include <errno.h>

#include "tunalloc.h"

#define PORT 123 
typedef struct sockaddr SOCKADDR;

void ext_out(int tunfd);
void recopieSocket(int clientfd, int tunfd);
void ext_in(int tunfd);

#endif
