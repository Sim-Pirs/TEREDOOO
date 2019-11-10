#include "extremite.h"
#include "tunalloc.h"

struct sockaddr_in6 serverAddr;
struct sockaddr_in6 clientAddr;

void recopieSocket(int clientfd, int tunfd){ // src, dest
    int size = 256;
    char tchar[size];
    ssize_t l;
    
    do{
        if((l = recv(clientfd, tchar, size, 0) == 0)){
            perror("client parti et connexion fermée\n");
            break;
        }

        if((l = recv(clientfd, tchar, size, 0) == -1)){
            perror("erreur de transmission\n");
            break;
        }

        writer(tunfd, tchar,size);

    }while(1);

    close(clientfd);

}

void ext_out(int tunfd){

    int socketserv, clientfd;
    if((socketserv = socket(AF_INET6, SOCK_STREAM,0)) == -1){
        perror("problème lors de la création de la socket ... \n");
        exit(1);
    }

    int reuseaddr = 1;
    setsockopt(socketserv,SOL_SOCKET,SO_REUSEADDR,&reuseaddr,sizeof(reuseaddr)); // set option :  buffer de serveur a l'adresse reuseaddr
    
    // initialisation, IP, PORT 
    serverAddr.sin6_family = AF_INET6; 
    serverAddr.sin6_addr = in6addr_any;
    serverAddr.sin6_port = htons(PORT);

    if((bind(socketserv, (SOCKADDR *)&serverAddr, sizeof(serverAddr))) == -1) { 
        perror("bind failed...\n"); 
        exit(1); 
    } 

    if(listen(socketserv, SOMAXCONN) == -1){
      perror("listen()");
         exit(1);
    }

    while(1){

        int taille = sizeof(struct sockaddr_in6);

        printf("Waiting... \n");
        if((clientfd = accept(socketserv,(SOCKADDR *)&clientAddr, (socklen_t *)&taille)) == -1){
            perror("accept()\n");
            exit(1);
        }
        recopieSocket(clientfd, tunfd);
    }
}