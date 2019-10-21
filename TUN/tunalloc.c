#include "tunalloc.h"

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

void recopie(int src, int dst){
  
  while(1){
    char* buf = malloc(sizeof(char) * 100);
    if(read_n(src,buf,100))

  }


}

int reader_char(int fdtun, char *buf, int n){
  
  int read;

  if((read=read(fdtun, buf, n))<0){
    perror("Reading data");
    exit(1);
  }
  return read;
}

int reader(int fdtun, char *buf, int n) {

  int read, left = n;

  while(left > 0) {
    if ((read = reader_char(fdtun, buf, left))==0){
      return 0 ;      
    }else {
      left -= read;
      buf += read;
    }
  }
  return n;  
}

int main (int argc, char** argv){

  int tunfd;
  char ipaddr;
  printf("Création de %s\n",argv[1]);
  tunfd = tun_alloc(argv[1]);
  printf("tunfd : %d \n", tunfd);

  ipaddr = argv[2];
  printf("Faire la configuration de %s...\n",argv[1]);
  printf("Appuyez sur une touche pour continuer\n");
  getchar();
  printf("Interface %s Configurée:\n",argv[1]);
  system("ip addr");



  printf("Appuyez sur une touche pour terminer\n");
  getchar();

  return 0;
}
