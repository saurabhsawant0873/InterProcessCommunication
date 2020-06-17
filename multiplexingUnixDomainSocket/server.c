#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>

/*Socket Name*/
#define SOCKET_NAME = "/tmp/baseUnixSocket"
#define MAX_CLIENT 32
#define BUFFER_MAX_SIZE 128

/*The fd_set  can store or server can serve ONLY 31 clients at a time + 1 Master File Descriptor*/
int multiplex_fd_set[MAX_CLIENT];

/*Each Client result at any point of time is maintained in this array*/
int result[MAX_CLIENT] = {0};


/*Initialize multiplex_fd_set with all -1's and also remove all FDs if any*/
static void initializeMultiplexFdSet()
{
        int i;
        for(i = 0; i < MAX_CLIENT; i++ )
        {
                multiplex_fd_set[i] = -1;
        }
}


/*Add a File Descriptor to multiplex_fd_set*/
static void addToMultiplexFdSet(int communication_fd)
{
        int i;
        for(i = 0; i < MAX_CLIENT; i++)
        {
                if(multiplex_fd_set[i] != -1)
                {
                        continue;
                }
                multiplex_fd_set[i] = communication_fd;
                break;
        }
}

/*Remove a File Descriptor from multiplex_fd_set*/
static void removeFromMultiplexFdSet(int communication_fd)
{
        int i;
        for(i = 0; i < MAX_CLIENT; i++)
        {
                if(multiplex_fd_set[i] != communication_fd)
                {
                        continue;
                }
                multiplex_fd_set[i] = -1;
                break;
        }
}


/*Get max FD value which server is monitoring in multiplex_fd_set*/
static int getMaxFd()
{
        int i;
        int max = -1;
        for(i = 0; i < MAX_CLIENT; i++ )
        {
                if(multiplex_fd_set[i] > max)
                {
                        max = multiplex_fd_set;
                }
        }
        return max;
}

/*Clearing the previous fd_data set values and cloning multiplex_fd_set into fd_set Data Strcuture provided by Linux OS Kernel*/

static void refreshFdSet(fd_set *fdDataSetPtr)
{
        FD_ZERO(fdDataSetPtr);      //Setting all values in fd_set to 0
        int i;

        for(i = 0; i < MAX_CLIENT; i++)
        {
                if(multiplex_fd_set[i] != -1)
                {
                        FD_SET(multiplex_fd_set, fdDataSetPtr);
                }
        }
}




int main(int argc, char *argv[])
{

        int connection_socket;
        int data_socket;
        int output;
        int buffer[BUFFER_MAX_SIZE];

        initializeMultiplexFdSet();
        unlink(SOCKET_NAME);
        connection_socket = socket();

        if(connection_socket == -1)
        {
                perror(socket);
                exit(EXIT_FAILURE);
        }

        printf("Master File descriptor sucessfully created");


        bind

                listen

                addToMultiplexFdSet(connection_socket);

        while(1)
        {
                refreshFdSet(&readfds);

                printf("Waiting on select() system call");

                select(getMaxFd() + 1, &readfds, NULL, NULL, NULL);

                if(FD_ISSET(connection_socket, &readfds))
                {
                        data_socket = accept(connection_socket, NULL, NULL);

                        if(data_socket == -1)
                        {
                                perror(accept);
                                exit(EXIT_FAILURE);
                        }
                        printf("accept() system call is sucessful, connection is established between client and server");

                        addToMultiplexFdSet(data_socket);
                }
                else
                {
                        int i, communication_socket_fd = -1;
                        for(i = 0; i < MAX_CLIENT; i++)
                        {
                                if(FD_ISSET(multiplex_fd_set[i], &readfds))
                                {
                                        communication_socket_fd = multiplex_fd_set[i];
                                }


                                memset(buffer, 0, BUFFER_MAX_SIZE);
                                printf("Waiting for SRM from client");
                                output = read(communication_socket_fd, buffer, BUFFER_MAX_SIZE);

                                if(output = -1)
                                {
                                        perror(read);
                                        exit(EXIT_FAILURE);
                                }

                                memcpy(&data, buffer, sizeof(int));

                                if(data == 0)
                                {
                                        sprintf(buffer, "Result = %d", result[i]);
                                        printf("sending final result to client");
                                        output = write(communication_socket_fd, buffer, BUFFER_MAX_SIZE);

                                        if(output == -1)
                                        {
                                                perror(write);
                                                exit(EXIT_FAILURE);
                                        }

                                        close(communication_socket_fd);
                                        result[i] = 0;
                                        removeFromMultiplexFdSet(communication_socket_fd);
                                        continue;
                                }
                                result[i] =+ data;

                        }
                }
        }
}
