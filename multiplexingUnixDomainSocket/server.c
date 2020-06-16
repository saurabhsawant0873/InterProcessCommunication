#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>

/*Socket Name*/
#define SOCKET_NAME = "/tmp/baseUnixSocket"
#define MAX_CLIENT 32

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

static void refreshFdDataSet(fd_set *fdDataSetPtr)
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
