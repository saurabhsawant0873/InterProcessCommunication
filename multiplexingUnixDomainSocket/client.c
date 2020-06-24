#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>


#define BUFFER_LENGTH 128
#define SOCKET_NAME "/tmp/baseUnixSocket"



int main(int argc, char *argv[])
{

int data_socket;
int result;
int output;
char buffer[BUFFER_LENGTH];

struct sockaddr_un unix_name;

/*Start the client, create a socket using socket() system call which will do stream communication*/
data_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if(data_socket == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

printf("Communication file descriptor created sucessfully")

/*Filling up the unix_name structure*/
unix_name.sun_family = AF_UNIX;
strncpy(unix_name.sun_path, SOCKET_NAME, sizeof(struct sockaddr_un));

/*Trying to establish connection with server using connect() system call*/
output = connect(data_socket, (const struct sockaddr_un*) &unix_name, sizeof(struct sockaddr_un));

    if(output == -1)
    {
        perror(connect);
        exit(EXIT_FAILURE);
    }
printf("Connection sucessfully established with the server\n");

    do
    {
    int i;
    printf("Sending Data to server\n");
    printf("Enter a value = ");
    scanf(%d, &i);
/*Send data to Server*/   
    output = write(data_socket, &i, sizeof(int));

        if(output == -1)
        {
            perror(write);
            break;
        }
        printf("Number of Bytes sent = %d and data sent = %d", output, i);
    
    }while(i);

/*Planning to Terminate the connection to the server*/
memset(buffer, 0, BUFFER_LENGTH);
/*Waiting for the messsage from Server*/
printf("Waiting for a message from Server...");   
   output = read(data_socket, buffer, BUFFER_LENGTH);

   if(output == -1)
   {
        perror(read);
        exit(EXIT_FAILURE);
   }

   printf("Received from server\n%s", buffer);
   printf("Closing connection on Client side...");
   close(data_socket);
   exit(EXIT_SUCCESS);
}
