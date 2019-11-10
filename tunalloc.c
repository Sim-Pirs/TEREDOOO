#include "tunalloc.h"
#include "extremite.h"
#include <stdlib.h>
#include <unistd.h>

int tun_alloc(char *dev)
{
  struct ifreq ifr;
  int fd, err;

  if( (fd = open("/dev/net/tun", O_RDWR)) < 0 ){
    perror("alloc tun");
    exit(-1);
  }

  memset(&ifr, 0, sizeof(ifr));

  /* Flags: IFF_TUN   - TUN device (no Ethernet headers) 
   *        IFF_TAP   - TAP device  
   *
   *        IFF_NO_PI - Do not provide packet information  
   */ 
  ifr.ifr_flags = IFF_TUN; 
  if( *dev )
    strncpy(ifr.ifr_name, dev, IFNAMSIZ);

  if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
    close(fd);
    return err;
  }
  strcpy(dev, ifr.ifr_name);
  return fd;
}

int reader(int fdtun, char *buf, int n){

  int read_c;

  if((read_c=read(fdtun, buf, n))<0){
    perror("Reading data");
    exit(1);
  }
  
  return read_c;
}

int writer(int fdtun, char *buf, int n){
  
  int read_c;

  if((read_c=write(fdtun, buf, n))<0){
    perror("Reading data");
    exit(1);
  }
  return read_c;
}

void recopie(int src, int dst){
  int size = 256;
  while(1){
    char* buf = malloc(sizeof(char) * size);
    printf("LIT : \n");
    reader(src,buf,size);
    printf("ECRIT : \n");
    writer(dst,buf,size);
     fflush(stdout);
    }
}


int main (int argc, char** argv){

  if(argc == 1){
    printf("Utilisation : ./tunalloc tun0 \n");
    return 0;
  }

  int tunfd;
  printf("Création de %s\n",argv[1]);

  tunfd = tun_alloc(argv[1]);
  printf("tunfd : %d \n", tunfd);


  printf("Faire la configuration de %s...\n",argv[1]);
  printf("Appuyez sur une touche pour continuer\n");
  getchar();
  printf("Interface %s Configurée:\n",argv[1]);
  system("ip addr");

  int dstp = 1; // sortie standard
// set pour détecter paquet entrant //
  fd_set rd_set;
  FD_ZERO(&rd_set);
  FD_SET(tunfd, &rd_set);

  if(FD_ISSET(tunfd, &rd_set)){
    recopie(tunfd,dstp);
  }

  ext_out(tunfd);

  printf("Appuyez sur une touche pour terminer\n");
  getchar();

  return 0;
}
