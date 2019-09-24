/*///////////////////////////////////////////////////////////
*
* FILE:		server.c
* AUTHOR:	Simon Moreno
* PROJECT:	CS 3251 Project 1 - Professor Jun Xu
* DESCRIPTION:	Network Server Code
* CREDIT:	Adapted from Professor Traynor
*
*////////////////////////////////////////////////////////////

/*Included libraries*/

#include <stdio.h>	  /* for printf() and fprintf() */
#include <sys/socket.h>	  /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>	  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>	  /* supports all sorts of functionality */
#include <unistd.h>	  /* for close() */
#include <string.h>	  /* support any string ops */

#define RCVBUFSIZE 512		/* The receive buffer size */
#define SNDBUFSIZE 512		/* The send buffer size */
#define BUFSIZE 40		/* Your name can be as many as 40 chars*/

static int maxRequests = 5;

/* The main function */
int main(int argc, char *argv[])
{

    int serverSock;				/* Server Socket */
    int clientSock;				/* Client Socket */
    struct sockaddr_in changeServAddr;		/* Local address */
    struct sockaddr_in changeClntAddr;		/* Client address */
    unsigned short changeServPort;		/* Server port */
    unsigned int clntLen;			/* Length of address data struct */

    char nameBuf[BUFSIZE];			/* Buff to store account name from client */
    int  balance;				/* Place to record account balance result */


    char sndBuf[SNDBUFSIZE];        /* Send Buffer */
    char rcvBuf[RCVBUFSIZE];        /* Receive Buffer */

    static const int maxRequests = 5;

    int countSaving = 0;
    int countChecking = 0;
    int countRetirement = 0;
    int countCollege = 0;

    changeServPort = (unsigned short)(atoi(argv[1]));

    if (argc != 2)
    {
        printf("Incorrect number of arguments. The correct format is:\n\tserverPort");
        exit(1);
    }

    /* Create new TCP Socket for incoming requests*/
    /*	    FILL IN	*/
    serverSock = socket(PF_INET, SOCK_STREAM, 0);
    if (serverSock == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    /* Construct local address structure*/
    /*	    FILL IN	*/
    memset(&changeServAddr, 0, sizeof(changeServAddr));
    changeServAddr.sin_family = AF_INET;
    changeServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    changeServAddr.sin_port = htons(changeServPort);
    
    /* Bind to local address structure */
    /*	    FILL IN	*/
    //(const struct sockaddr_in*)
    if (bind(serverSock, (struct sockaddr_in*)&changeServAddr, sizeof(struct sockaddr_in)) == -1) {
        printf("Binding failed...\n");
        close(serverSock);
        exit(0);
    }
    else { printf("Binding successful..\n"); }

    /* Listen for incoming connections */
    /*	    FILL IN	*/
    if (listen(serverSock, maxRequests) == -1) {
        printf("Listening failed...\n");
        close(serverSock);
        exit(0);
    } else { printf("Listening successful..\n"); }

    /* Loop server forever*/
    while(1)
    {

	/* Accept incoming connection */
	/*	FILL IN	    */
        clientSock = accept(serverSock, (struct sockaddr *) &changeClntAddr, &clntLen);
        if(clientSock == -1){
            printf("Accept connection failed...\n");
            close(serverSock);
            exit(0);
        }
        else { printf("Connection accepted..\n"); }

	/* Extract the account name from the packet, store in nameBuf */
	/* Look up account balance, store in balance */
	//myChecking|mySavings|myCD|my401k|my529
	/*	FILL IN	    */

	    //BAL commands for each account
        if(strcmp("BAL myChecking", rcvBuf)==0){
            countSaving++;
            balance = 3000;
            snprintf(sndBuf, SNDBUFSIZE, "myChecking BALANCE  %d", balance);
        }
        else if(strcmp("BAL mySavings", rcvBuf)==0){
            countChecking++;
            balance = 10000;
            snprintf(sndBuf, SNDBUFSIZE, "mySavings BALANCE  %d", balance);
        }
        else if(strcmp("BAL myCD", rcvBuf)==0){
            countRetirement++;
            balance = 2000;
            snprintf(sndBuf, SNDBUFSIZE, "myRetirement BALANCE  %d", balance);
        }
        else if(strcmp("BAL my401k", rcvBuf)==0){
            countCollege++;
            balance = 50000;
            snprintf(sndBuf, SNDBUFSIZE, "myCollege BALANCE  %d", balance);
        }
        else if(strcmp("BAL my529", rcvBuf)==0){
            countCollege++;
            balance = 5000;
            snprintf(sndBuf, SNDBUFSIZE, "myCollege BALANCE  %d", balance);
        }

        //WITHDRAW commands for each account
        else if(strcmp("WITHDRAW myChecking", rcvBuf)==0){
            snprintf(sndBuf, SNDBUFSIZE, "mySavings COUNT  %d", countSaving);
        }
        else if(strcmp("WITHDRAW mySavings", rcvBuf)==0){
            snprintf(sndBuf, SNDBUFSIZE, "myChecking COUNT  %d", countChecking);
        }
        else if(strcmp("WITHDRAW myCD", rcvBuf)==0){
            snprintf(sndBuf, SNDBUFSIZE, "myRetirement COUNT  %d", countRetirement);
        }
        else if(strcmp("WITHDRAW my401k", rcvBuf)==0){
            snprintf(sndBuf, SNDBUFSIZE, "myCollege COUNT  %d", countCollege);
        }
        else if(strcmp("WITHDRAW my529", rcvBuf)==0){
            snprintf(sndBuf, SNDBUFSIZE, "myCollege COUNT  %d", countCollege);
        }

        //TRANSFER commands for each account
        else if(strcmp("TRANSFER myChecking", rcvBuf)==0){
            snprintf(sndBuf, SNDBUFSIZE, "mySavings COUNT  %d", countSaving);
        }
        else if(strcmp("TRANSFER mySavings", rcvBuf)==0){
            snprintf(sndBuf, SNDBUFSIZE, "myChecking COUNT  %d", countChecking);
        }
        else if(strcmp("TRANSFER myCD", rcvBuf)==0){
            snprintf(sndBuf, SNDBUFSIZE, "myRetirement COUNT  %d", countRetirement);
        }
        else if(strcmp("TRANSFER my401k", rcvBuf)==0){
            snprintf(sndBuf, SNDBUFSIZE, "myCollege COUNT  %d", countCollege);
        }
        else if(strcmp("TRANSFER my529", rcvBuf)==0){
            snprintf(sndBuf, SNDBUFSIZE, "myCollege COUNT  %d", countCollege);
        }
        else{
            // perror() -> posted straight to command prompt; not buffered
            snprintf(sndBuf, SNDBUFSIZE, "YOU FAILED TO GIVE CORRECT PROMPT. Please try again.");
        }

	/* Return account balance to client */
	/*	FILL IN	    */

        int sentSize = send(clientSock, sndBuf, SNDBUFSIZE, 0);
        close(clientSock);
    }

}

