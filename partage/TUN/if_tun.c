#include "if_tun.h"

/*
 * Lit n caractères de fdtun dans le buffer buf
 */
int reader(int fdtun, char *buf, int n){

  int read_c;

  if((read_c=read(fdtun, buf, n))<0){
    perror("Reading data");
    exit(1);
  }
  
  return read_c;
}

/*
 * Ecrit n caractère du buffer vers le fichier fdtun
 */
int writer(int fdtun, char *buf, int n){
  
  int read_c;

  if((read_c=write(fdtun, buf, n))<0){
    perror("Reading data");
    exit(1);
  }
  return read_c;
}

/*
 * 
 */
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

/*
 * 
 */
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
