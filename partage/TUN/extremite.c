#include "extremite.h"
#include "if_tun.h"

void recopieDepuisSocket(int clientfd, int tunfd){ // src, dest
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

    close(clientfd); ///////////////////////////////////////////////////////////////////////////////////////////
}

void recopieDansSocket(int tunfd, int sock){
    int size = 256;
    while (1){
        char *buffer = malloc(sizeof(char)*size);
        reader(tunfd, buffer, size);
        send(sock, buffer, size, 0);
    }
}

void ext_out(int tunfd){

    int socketserv, clientfd;
    struct sockaddr_in6 serverAddr, clientAddr;
;

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
        recopieDepuisSocket(clientfd, tunfd);
    }
}

// Ouvre une connexion TCP avec l’autre extrémité du tunnel, puis lit le trafic provenant de tun0 et le retransmet dans la socket
void ext_in(int tunfd, char* destAddr){
	
	//Ouvre connexion tcp avec l'autre extremité du tunnel
	int sock;
    struct sockaddr_in6 server;
     
    if ((sock = socket(AF_INET6 , SOCK_STREAM , 0)) == -1)
    {
        perror("problème lors de la création de la socket ... \n");
        exit(1);
    }
     
    inet_pton(AF_INET6, destAddr, &server.sin6_addr);
    server.sin6_family = AF_INET6;
    server.sin6_port = htons( 123 );
 
    while(1){
        while(connect(sock , (struct sockaddr *)&server , sizeof(server)) == -1){
             printf("attente de lancement ...");
             sleep(1);
        }
        recopieDansSocket(tunfd, sock);
    }

    printf("fin de l'entrée\n");
}
