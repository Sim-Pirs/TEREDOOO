#include "tunalloc.h"

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

  printf("Appuyez sur une touche pour terminer\n");
  getchar();

  return 0;
}
