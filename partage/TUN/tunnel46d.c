#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tunnel46d.h"

#define BUF_SIZE 256

int tunfd = 0;
char* destAddr = NULL;
char* tun = malloc(sizeof(char)*4)

int main (int argc, char* argv[]){
	lit_config();
	
	bidirection();
	
	return 0;
}

void lit_config(){
	FILE* file = fopen("config.conf", "r");
	
	char *ligne = malloc(sizeof(char)*256);
	
	if(file == NULL)
	{
		printf("Erreur lors de l'ouverture du fichier config.conf");
		exit(1);
	}
	
	for(i=0; i<9; i++){
		if(fgets(ligne, 256, file) != NULL){
			printf("Erreur a l lecture du fichier\n");
			exit(1);
		}
		if(ligne[0] != "#")
		{
			for(i = 0; i < 256; i++){
				if(ligne[0] == 't'){
					for(k=0;k<4;k++){
						tun[k] = ligne[k+4]
					}
				}
			}
		}
	}
	fclose(file);
}

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

void bidirection(){
	
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
