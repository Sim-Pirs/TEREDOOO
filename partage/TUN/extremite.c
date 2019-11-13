#include "extremite.h"
#include "if_tun.h"

/*
    - Utilisation : ./extremite -in  tunfd IPADDR
                    ./extremite -out tunfd
                    ./extremite -b   tunfd IPADDR

                    fc00:1234:2::36
*/

#define BUF_SIZE 256

/*
 * Recopie le contenu du socket clientfd vers le fichier tunfd
 */
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

/*
 * Met le contenu du fichier tunfd vers le socket sock
 */
void recopieDansSocket(int tunfd, int sock){
    int size = 256;
    char *buffer = malloc(sizeof(char)*size);
    while (1){
        reader(tunfd, buffer, size);
        send(sock, buffer, size, 0);
    }
}

/*
 * Gère le serveur
 */
void ext_out(int tunfd){

    int socketserv, clientfd;
    struct sockaddr_in6 serverAddr, clientAddr;

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

    if(bind(socketserv, (SOCKADDR *)&serverAddr, sizeof(serverAddr)) == -1) { 
        perror("bind failed...\n"); 
        exit(1); 
    } 

    if(listen(socketserv, SOMAXCONN) == -1){
      perror("listen()");
         exit(1);
    }

	int taille = sizeof(struct sockaddr_in6);
    while(1){
        printf("Waiting... \n");
        if((clientfd = accept(socketserv,(SOCKADDR *)&clientAddr, (socklen_t *)&taille)) == -1){
            perror("accept()\n");
            exit(1);
        }
        recopieDepuisSocket(clientfd, tunfd);
    }
    close(socketserv);
}

// Ouvre une connexion TCP avec l’autre extrémité du tunnel, puis lit le trafic provenant de tun0 et le retransmet dans la socket
void ext_in(int tunfd, char* destAddr){
	
	//Ouvre connexion tcp avec l'autre extremité du tunnel
	int sock;
    struct sockaddr_in6 server;
     
    if ((sock = creer_connexion(destAddr)) == -1)
    {
        perror("problème lors de la création de la socket ... \n");
        exit(1);
    }
 
    while(1){
        while(connect(sock , (struct sockaddr *)&server , sizeof(server)) == -1){
             printf("attente de lancement ...");
             sleep(1);
        }
        recopieDansSocket(tunfd, sock);
    }
    close(sock);

    printf("fin de l'entrée\n");
}

int creer_connexion(char *destAddr){
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
	return sock;
}

void bidirection(int tunfd, char* destAddr){
	
	char buf[BUF_SIZE];
	fd_set rdfs;
	int sock;
	
	if ((sock = creer_connexion(destAddr)) == -1)
    {
        perror("problème lors de la création de la socket ... \n");
        exit(1);
    }
    
	while(1)
	{
		FD_ZERO(&rdfs);

		FD_SET(STDIN_FILENO, &rdfs);

		FD_SET(sock, &rdfs);

/*
		if(select(sock + 1, &rdfs, NULL, NULL, NULL) == -1)
		{
			perror("select()");
			exit(errno);
		}
*/

		//Si on a écrit
		if(FD_ISSET(STDIN_FILENO, &rdfs))
		{
			fgets(buf, BUF_SIZE - 1, stdin);
			char *p = NULL;
			p = strstr(buf, "\n");
			if(p != NULL)
				*p = 0;
            else
				buf[BUF_SIZE - 1] = 0;
			writer(sock, buf,BUF_SIZE);
		}
		//Si on a reçu quelque chose
		else if(FD_ISSET(sock, &rdfs))
		{
			int n = reader(sock, buf,BUF_SIZE);
			/* server down */
			if(n == 0)
			{
				printf("Server disconnected !\n");
				break;
			}
			puts(buf);
		}
	}
	close(sock);
}


int main (int argc, char* argv[]){

    if(argc < 3){
        printf("- Utilisation : ./extremite -in  tunfd IPADDR\n                ./extremite -out tunfd \n                ./extremite -b   tunfd IPADDR \n");
        exit(1);
    }

    int tunfd = tun_alloc(argv[2]);


    if(strcmp(argv[1], "-in")==0){
         if(argc != 4){
            printf("Erreur ...\n");
            printf("- Utilisation : ./extremite -in  tunfd IPADDR\n");
            exit(1);
         }
        
         ext_in(tunfd, argv[3]);
    }
    
    else if(strcmp(argv[1], "-out")==0)
    {
        if(argc != 3){
            printf("Erreur ...\n");
            printf("- Utilisation : ./extremite -out tunfd\n");
            exit(1);
        }
        ext_out(tunfd);
    }
    else if(strcmp(argv[1], "-b")==0){
        if(argc != 4){
            printf("Erreur ...\n");
            printf("- Utilisation : ./extremite -b  tunfd IPADDR\n");
            exit(1);
         }
         bidirection(tunfd,argv[3]);
    }
    else{
        printf("Erreur ...\n");
        printf("- Utilisation : ./extremite -in  tunfd IPADDR\n                ./extremite -out tunfd \n                ./extremite -b   tunfd IPADDR \n");
        exit(1);
    }

    return 0;
}