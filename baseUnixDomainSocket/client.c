#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>

#define BUFFER_LENGTH 256
#define SOCKET_NAME "/tmp/baseUnixSocket"

int main(int argc, char *argv[]){
        
        int data_socket;
        int output;
        int server_data;
        int result;
        char buffer[BUFFER_LENGTH];

        struct sockaddr_un unix_name;

        data_socket = socket(AF_UNIX, SOCK_STREAM, 0);

        if(data_socket == -1){
                perror("socket");
                exit(EXIT_FAILURE);
        }

        printf("Connection file Descriptor created sucessfully");

        unix_name.sun_family = AF_UNIX;
        strncpy(unix_name.sun_path, SOCKET_NAME, sizeof(unix_name.sun_path) - 1);
        printf("Trying to establish connection with the Server");
        
        output = connect(data_socket, (const struct sockaddr_un*) unix_name, sizeof(struct sockaddr_un));

        if(output == -1){
               fprintf(stderr, "The server is down\n");
               exit(EXIT_FAILURE);
        }

        printf("Sending data to Server\n");
        
        do{
                printf("Enter a Number : ");
                scanf("%d", &i);
                memset(buffer, 0, BUFFER_LENGTH);

                write(data_socket, &i, BUFFER_LENGTH);

                if(output == -1){
                        perror("write");
                        break;
                }
                
                printf("Number of Bytes sent = %d and data sent = %d", output, i); 
               
               }while(i);

memset(buffer, 0, BUFFER_LENGTH);

output = read(data_socket, buffer, BUFFER_LENGTH);

                if(output == -1){
                        perror("read");
                        break;
                }
printf("Received from Server %s\n", buffer);
printf("Closing connection on Client");
close(data_socket);
exit(EXIT_SUCCESS);

}
