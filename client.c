#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"
#include "buffer.h"
#define HOST "34.241.4.235"
#define PORT 8080

int isErrorMessage(char *message)
{
    char *copy = strdup(message);
    char *firstLine = strtok(copy, "\n");

    if( strstr(firstLine, "4") != NULL || strstr(firstLine, "5") != NULL )
        return 1;
    return 0;
}

char* getError(char *message)
{
    char *json = basic_extract_json_response(message);
   
   if( json != NULL )
   {
        JSON_Value *schema = json_parse_string(json);
        return json_object_get_string(json_object(schema), "error");
   }
   return NULL;
}

int main()
{
   
   char *cookie = malloc(sizeof(char)*1555);
   char *token =  malloc(sizeof(char)*1254);
   char comanda[256];

    while(strcmp(comanda, "exit")!=0)
    {
        scanf("%s", comanda);
        if(strcmp(comanda,"register")==0)
        {
            int sockfd = open_connection(HOST, PORT , AF_INET, SOCK_STREAM, 0);
            printf("username=");
            char username[100];
            scanf("%s", username);
            printf("parola=");
            char parola[100];
            scanf("%s", parola);
            char *bodydata[1];
            bodydata[0]= (char*)malloc(sizeof(char)*100);

            JSON_Value *val  = json_value_init_object();
            JSON_Object *obj = json_value_get_object(val);
            json_object_set_string(obj, "username", username);
            json_object_set_string(obj, "password", parola);
            char* str = json_serialize_to_string(val);
            strcpy(bodydata[0], str);
            char *cerere = compute_post_request(HOST, "/api/v1/tema/auth/register", "application/json", bodydata, 1, NULL, 0, NULL);
            
            send_to_server(sockfd, cerere);
            char *raspuns = receive_from_server(sockfd);
            
            if( isErrorMessage(raspuns) )
                printf("%s\n", getError(raspuns));
            else
                printf("Inregistrare cu succes!\n");

            close(sockfd);
        }
        else  if(strcmp(comanda,"login")==0)
        {
            int sockfd = open_connection(HOST, PORT , AF_INET, SOCK_STREAM, 0);
            printf("username=");
            char username[100];
            scanf("%s", username);
            printf("parola=");
            char parola[100];
            scanf("%s", parola);
            char *bodydata[1];
            bodydata[0]= malloc(sizeof(char)*100);

            JSON_Value *val  = json_value_init_object();
            JSON_Object *obj = json_value_get_object(val);
            json_object_set_string(obj, "username", username);
            json_object_set_string(obj, "password", parola);
            char* str = json_serialize_to_string(val);
            strcpy(bodydata[0], str);
            char *cerere = compute_post_request(HOST, "/api/v1/tema/auth/login", "application/json", bodydata, 1, NULL, 0, NULL);
            //printf("%s", cerere);
            //free(bodydata[0]);
            
            send_to_server(sockfd, cerere);
           // free(cerere);
            char *r = receive_from_server(sockfd);
        

            buffer b = buffer_init();
            buffer_add(&b, r, strlen(r)+3);

            int poz = buffer_find(&b, "Set-Cookie", 10);

            buffer b1 = buffer_init();
            buffer_add(&b1, b.data + poz + 12, b.size - poz - 12);

            int poz2 = buffer_find(&b1, ";", 1);
            strncpy(cookie, b1.data, poz2 + 1);
           // printf("%s", cookie);
            
           

            buffer_destroy(&b);
            buffer_destroy(&b1);
            if( isErrorMessage(r) )
                printf("%s\n", getError(r));
            else
                printf("Logare cu succes ca %s!\n", username);
            close(sockfd);
        }
        else  if(strcmp(comanda,"enter_library")==0)
        {
            int sockfd = open_connection(HOST, PORT , AF_INET, SOCK_STREAM, 0);
            char *bodydata[1];
            bodydata[0]= malloc(sizeof(char)*256);
            strcpy(bodydata[0], cookie);
            char* cerere= compute_get_request(HOST, "/api/v1/tema/library/access", NULL, bodydata, 1, NULL);

            send_to_server(sockfd, cerere);
            char *r = receive_from_server(sockfd);
            
            buffer b = buffer_init();
            buffer_add(&b, r, strlen(r));

            int poz = buffer_find(&b, "\"token\":", 8);

            buffer b1 = buffer_init();
            buffer_add(&b1, b.data + poz + 9, b.size - poz - 9);

            int poz2 = buffer_find(&b1, "}", 1);
            strncpy(token, b1.data, poz2 - 1);

            if( isErrorMessage(r) )
                printf("%s\n", getError(r));
            else
                printf("Ai intrat in biblioteca!\n");
            

            close(sockfd);
            
        }
        else  if(strcmp(comanda,"get_books")==0)
        {
            int sockfd = open_connection(HOST, PORT , AF_INET, SOCK_STREAM, 0);
            char* cerere= compute_get_request(HOST, "/api/v1/tema/library/books", NULL, NULL, 0, token);
            send_to_server(sockfd, cerere);
           
            
            char *r = receive_from_server(sockfd);
            printf("%s\n", strstr(r, "["));

            
            if( isErrorMessage(r) )
                printf("%s\n", getError(r));
           
            close(sockfd);
        }
        else  if(strcmp(comanda,"get_book")==0)
        {
            printf("id="); char *id= malloc(sizeof(int)*100);
            scanf("%s", id);
            strtok(id, "\n");

            char aux[100] = "/api/v1/tema/library/books/";
            strcat(aux, id);

            int sockfd=open_connection(HOST, PORT , AF_INET, SOCK_STREAM, 0);
            char* cerere= compute_get_request(HOST, aux, NULL, NULL, 0, token);
            send_to_server(sockfd, cerere);
            char *r = receive_from_server(sockfd);
            
            if( isErrorMessage(r) )
                printf("%s\n", getError(r));
            else
                printf("%s\n", strstr(r, "["));
            close(sockfd);
        
        }
        else  if(strcmp(comanda,"add_book")==0)
        {
            getchar();
            printf("title="); char title[101]; fgets(title, 100, stdin); title[strlen(title)-1] = 0;
            printf("author="); char author[101]; fgets(author, 100, stdin); author[strlen(author)-1] = 0;
            printf("genre="); char genre[101]; fgets(genre, 100, stdin); genre[strlen(genre)-1] = 0;
            printf("publisher="); char publisher[101]; fgets(publisher, 100, stdin); publisher[strlen(publisher)-1] = 0;
            printf("page_count="); int page_count; scanf("%d", &page_count);

            JSON_Value *val  = json_value_init_object();
            JSON_Object *obj = json_value_get_object(val);
            json_object_set_string(obj, "title", title);
            json_object_set_string(obj, "author", author);
            json_object_set_string(obj, "genre", genre);
            json_object_set_string(obj, "publisher", publisher);
            json_object_set_number(obj, "page_count", page_count);
            char* str = json_serialize_to_string(val);

            char *bodydata[1];
            bodydata[0]= malloc(sizeof(char)*100);
            strcpy(bodydata[0], str);

            int sockfd=open_connection(HOST, PORT , AF_INET, SOCK_STREAM, 0);
            char *cerere = compute_post_request(HOST, " /api/v1/tema/library/books", "application/json", bodydata, 1, NULL, 0, token);
            puts(cerere);            
            send_to_server(sockfd, cerere);
            char *r = receive_from_server(sockfd);

            puts(r);
            
            if( isErrorMessage(r) )
                printf("%s\n", getError(r));
           
            
            close(sockfd);
        }
        else  if(strcmp(comanda,"delete_book")==0)
        {
            printf("id="); char *id= malloc(sizeof(int)*100);
            scanf("%s", id);
            strtok(id, "\n");

            char aux[100] = "/api/v1/tema/library/books/";
            strcat(aux, id);

            int sockfd=open_connection(HOST, PORT , AF_INET, SOCK_STREAM, 0);
            char* cerere= compute_delete_request(HOST, aux, NULL, NULL, 0, token);
            send_to_server(sockfd, cerere);

            close(sockfd);
        }
        else  if(strcmp(comanda,"logout")==0)
        {
            int sockfd = open_connection(HOST, PORT , AF_INET, SOCK_STREAM, 0);
            char *bodydata[1];
            bodydata[0]= malloc(sizeof(char)*256);
            strcpy(bodydata[0], cookie);
            char* cerere= compute_get_request(HOST, "/api/v1/tema/auth/logout", NULL, bodydata, 1, NULL);

            send_to_server(sockfd, cerere);
            
            close(sockfd);
        }
    }
    
}