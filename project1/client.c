/*///////////////////////////////////////////////////////////
*
* FILE:		client.c
* AUTHOR:	Simon Moreno
* PROJECT:	CS 3251 Project 1 - Professor Jun Xu 
* DESCRIPTION:	Network Client Code
* CREDIT:	Adapted from Professor Traynor
*
*////////////////////////////////////////////////////////////

/* Included libraries */

#include <stdio.h>		    /* for printf() and fprintf() */
#include <sys/socket.h>		    /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>		    /* for sockaddr_in and inet_addr() */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Constants */
#define RCVBUFSIZE 512		    /* The receive buffer size */
#define SNDBUFSIZE 512		    /* The send buffer size */
#define REPLYLEN 32

/* The main function */
int main(int argc, char *argv[])
{

    int clientSock;		    /* socket descriptor */
    struct sockaddr_in serv_addr;   /* server address structure */

    char *accountName;		    /* Account Name  */
    char *servIP;		    /* Server IP address  */
    unsigned short servPort;	    /* Server Port number */
    

    char sndBuf[SNDBUFSIZE];	    /* Send Buffer */
    char rcvBuf[RCVBUFSIZE];	    /* Receive Buffer */
    
    int balance;		    /* Account balance */

    char *toAccount;     /* Account to transfer money to */
    char *moveMoney;          /* Amount of $ to move */

    enum operations{BAL, WITHDRAW, TRANSFER};
    enum accountNames{myChecking, mySavings, myCD, my401k, my529};

    enum operations operation;
    enum accountNames account;

    /* Get the Account Name from the command line */
    /* Get the addditional parameters from the command line */
    /*	    FILL IN	*/
    //BAL
    if (argc == 5) {
        operation = BAL;
        accountName = argv[2];
        servIP = argv[3];
        servPort = atoi(argv[4]);
        memset(&sndBuf, 0, SNDBUFSIZE);
        memset(&rcvBuf, 0, RCVBUFSIZE);
    }
    //WITHDRAW
    else if (argc == 6) {
        operation = WITHDRAW;
        accountName = argv[2];
        moveMoney = argv[3];
        servIP = argv[4];
        servPort = atoi(argv[5]);
        memset(&sndBuf, 0, SNDBUFSIZE);
        memset(&rcvBuf, 0, RCVBUFSIZE);
    }
    //TRANSFER
    else if (argc == 7) {
        operation = TRANSFER;
        accountName = argv[2];
        toAccount = argv[3];
        moveMoney = argv[4];
        servIP = argv[5];
        servPort = atoi(argv[6]);
        memset(&sndBuf, 0, SNDBUFSIZE);
        memset(&rcvBuf, 0, RCVBUFSIZE);
    }
    else {
        printf("Incorrect number of arguments. Try again.");
        exit(1);
    }

    /* Create a new TCP socket*/
    /*	    FILL IN	*/
    clientSock = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSock == -1){
        printf("Socket creation failed...\n");
        exit(0);
    }
    else { printf("Socket successfully created..\n"); }

    /* Construct the server address structure */
    /*	    FILL IN	 */
    serv_addr.sin_family = AF_INET;          // host byte order
    serv_addr.sin_port = htons(servPort);   // short, network byte order
    serv_addr.sin_addr.s_addr = inet_addr(servIP);
    memset(&(serv_addr.sin_zero), '\0', 8);

    /* Establish connecction to the server */
    /*	    FILL IN	 */
    if (connect(clientSock, (struct sockaddr_in *)&serv_addr, sizeof(struct sockaddr_in)) == -1) {
        printf("Connection failed...\n");
        exit(0);
    } else { printf("Connection successfully established..\n"); }
    
    /* Send the string to the server */
    /*	    FILL IN	 */
    /*
    snprintf(sndBuf, SNDBUFSIZE, "%s %s", balCount, accountName);

    ssize_t numBytes = send(sock, sndBuf, SNDBUFSIZE, 0);
    if(numBytes < 0){
        // DieWithSystemMessage("send() failed");
        perror("send() failed... TRY AGAIN");
        close(sock);
        exit(EXIT_FAILURE);
    }
    */
    /* Receive and print response from the server */
    /*	    FILL IN	 */

    int recvSize = recv(clientSock, rcvBuf, RCVBUFSIZE, 0);
    if(recvSize == -1){
        printf("Receiving response failed...\n");
        exit(1);
    }

    printf("%s\n", rcvBuf);

    close(clientSock);

    return 0;
}

