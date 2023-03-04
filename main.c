#include <stdio.h>    
#include <stdlib.h>   
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "cJSON.c";
#include "cJSON.h";
#define HOST "34.241.4.235"
#define PORT 8080

int sockfd = open_connection(HOST.c.str(), PORT , AF_INET, SOCK_STREAM, 0);

int main()
{
   char comanda[256];

    while(strcmp(comanda, "exit")==1)
    {
        if(strcmp(comanda,"register")==0)
        {
            printf("username=", %s);
            char username[100];
            scanf("%s", username);
            printf("parola=", %s);
            char parola[100];
            scanf("%s", parola);
            char *bodydata[2];
            bodydata[0]= malloc(sizeof(char));
            strcpy(bodydata[0], username);
            bodydata[1]= malloc(sizeof(char));
            strcpy(bodydata[1], parola);

            free(bodydata[0]);
            free(bodydata[1]);
            free(bodydata);
            
            char *requestul = compute_post_request(HOST, "/ap1/v1/tema/auth/register", "/application/json");
        }
    }

}