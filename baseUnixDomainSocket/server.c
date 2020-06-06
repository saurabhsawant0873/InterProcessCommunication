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

printf("bind system call sucessful");

   output = listen(connection_socket, 20);

   if(output == -1){
        printf("listen system call failed");
        perror("listen");
        exit(EXIT_FAILURE);
   }

printf("listen system call sucessful");

   while(1)
   {
           printf("Waiting on accept() system call\n");
           data_socket = accept(connection_socket, NULL, NULL);

           if(data_socket == -1){
                   perror("accept");
                   exit(EXIT_FAILURE);
           }

           printf("Connection Initiation Request received from client\n");
   }


}
