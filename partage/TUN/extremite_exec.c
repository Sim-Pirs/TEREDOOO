#include "extremite_exec.h"

/*
    - Utilisation : ./extremite_exec -in  tunfd IPADDR
                    ./extremite_exec -out tunfd
                    ./extremite_exec -b   tunfd IPADDR
*/

int main (int argc, char* argv[]){

    if(argc < 3){
        printf("- Utilisation : ./extremite_exec -in  tunfd IPADDR\n                ./extremite_exec -out tunfd \n                ./extremite_exec -b   tunfd IPADDR \n");
        exit(1);
    }

    if(strcmp(argv[1], "-in")==0){
         if(argc != 4){
            printf("Erreur ...\n");
            printf("- Utilisation : ./extremite_exec -in  tunfd IPADDR\n");
            exit(1);
         }
         ext_in(argv[2], argv[3]);
    }
    
    else if(strcmp(argv[1], "-out")==0)
    {
        if(argc != 3){
            printf("Erreur ...\n");
            printf("- Utilisation : ./extremite_exec -out tunfd\n");
            exit(1);
        }
        ext_out(argv[2]);
    }
    else if(strcmp(argv[1], "-b")==0){
        if(argc != 4){
            printf("Erreur ...\n");
            printf("- Utilisation : ./extremite_exec -b  tunfd IPADDR\n");
            exit(1);
         }
         /*bidirection(argv[2],argv[3]);*/
    }
    else{
        printf("Erreur ...\n");
        printf("- Utilisation : ./extremite_exec -in  tunfd IPADDR\n                ./extremite_exec -out tunfd \n                ./extremite_exec -b   tunfd IPADDR \n");
        exit(1);
    }

}