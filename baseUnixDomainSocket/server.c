#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>

#define BUFFER_LENGTH 256
#define SOCKET_NAME "/tmp/baseUnixSocket"

int main(int argc, char *argv[]){

int output;
int connection_socket;
int data_socket;
char buffer[BUFFER_LENGTH];
int client_data;
int result;

struct sockaddr_un unix_name;

   unlink(SOCKET_NAME);
      
      connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);

      if(connection_socket == -1){
              printf("Master Socket File Descriptor creation failed");
              perror("socket");
              exit(EXIT_FAILURE);
      }

printf("Master Socket File Descriptor created sucessfully");

 unix_name.sun_family = AF_UNIX;
 strncpy(unix_name.sun_path, SOCKET_NAME, sizeof(unix_name.sun_path) - 1);

   output = bind(connection_socket ,(const struct sockaddr_un*) &unix_name, sizeof(struct sockaddr_un));

   if(output == -1){
        printf("bind system call failed");
        perror("bind");
        exit(EXIT_FAILURE);
   }

printf("bind() system call sucessful");

   output = listen(connection_socket, 20);

   if(output == -1){
        printf("listen system call failed");
        perror("listen");
        exit(EXIT_FAILURE);
   }

printf("listen() system call sucessful");

   while(1)
   {
           printf("Waiting on accept() system call\n");
           data_socket = accept(connection_socket, NULL, NULL);

           if(data_socket == -1){
                   perror("accept");
                   exit(EXIT_FAILURE);
           }

           printf("Connection Initiation Request received from Client\n");
           printf("Connection sucessfully established with the Client\n");

           while(1){
                   memset(buffer, 0, BUFFER_LENGTH);
                   printf("Waiting for Client to send Data\n");
                   output = read(data_socket, buffer, BUFFER_LENGTH);

           if(output == -1){
                   perror("read");
                   exit(EXIT_FAILURE);
           }
                   memcpy(&client_data, buffer, sizeof(int));

                   if(client_data == 0){
                           printf("Client wants to terminate the connection\n");
                           break;
                   }

                   result += client_data;
           }

           printf("Server sending final Result to Client\n");
           memset(buffer, 0, BUFFER_LENGTH);
           sprintf(buffer, "Result = %d", result);
           
           output = write(data_socket, buffer , BUFFER_LENGTH);

           if(write == -1){
                   perror("write");
                   exit(EXIT_FAILURE);
           }
           
           printf("Server terminating the connection\n");
           close(data_socket);
   }

close(connection_socket);
printf("Connection closed ...\n");
unlink(SOCKET_NAME);
exit(EXIT_SUCCESS);
}
