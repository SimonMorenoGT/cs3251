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


    int sndBuf[SNDBUFSIZE];	    /* Send Buffer */
    char rcvBuf[RCVBUFSIZE];	    /* Receive Buffer */

    int balance;		    /* Account balance */

    char *to_account; //Transfers account

    enum operations{BAL, WITHDRAW, TRANSFER};
    enum accountNames{myChecking, mySavings, myCD, my401k, my529};

    int operation;
    int account1;
    int account2;

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
        //moveMoney = argv[3];
        servIP = argv[4];
        servPort = atoi(argv[5]);
        memset(&sndBuf, 0, SNDBUFSIZE);
        memset(&rcvBuf, 0, RCVBUFSIZE);
    }
    //TRANSFER
    else if (argc == 7) {
        operation = TRANSFER;
        accountName = argv[2];
        to_account = argv[3];
        //moveMoney = argv[4];
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
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(servPort);
    serv_addr.sin_addr.s_addr = inet_addr(servIP);
    //memset(&(serv_addr.sin_zero), '\0', 8);

    /* Establish connection to the server */
    /*	    FILL IN	 */
    if (connect(clientSock, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) == -1) {
        printf("Connection failed...\n");
        exit(0);
    } else { printf("Connection successfully established..\n"); }

    /* Send the string to the server */
    /*	    FILL IN	 */

    //Create message to send//

    sndBuf[0] = operation;

    if (strcmp(accountName, "myChecking") == 0) {
        account1 = myChecking;
        sndBuf[1] = account1;
    }
    else if (strcmp(accountName, "mySavings") == 0) {
        account1 = mySavings;
        sndBuf[1] = account1;
    }
    else if (strcmp(accountName, "myCD") == 0) {
        account1 = myCD;
        sndBuf[1] = account1;
    }
    else if (strcmp(accountName, "my401k") == 0) {
        account1 = my401k;
        sndBuf[1] = account1;
    }
    else if (strcmp(accountName, "my529") == 0) {
        account1 = my529;
        sndBuf[1] = account1;
    } else {
        printf("Invalid account...\n");
        exit(0);
    }

    if (operation == BAL) {
        send(clientSock, sndBuf, SNDBUFSIZE, 0);
    } else if (operation == WITHDRAW) {
        sndBuf[2] = atoi(argv[3]);
        send(clientSock, sndBuf, SNDBUFSIZE, 0);
    } else if (operation == TRANSFER) {
        if (strcmp(to_account, "myChecking") == 0) {
            account2 = myChecking;
            sndBuf[2] = account2;
        }
        else if (strcmp(to_account, "mySavings") == 0) {
            account2 = mySavings;
            sndBuf[2] = account2;
        }
        else if (strcmp(to_account, "myCD") == 0) {
            account2 = myCD;
            sndBuf[2] = account2;
        }
        else if (strcmp(to_account, "my401k") == 0) {
            account2 = my401k;
            sndBuf[2] = account2;
        }
        else if (strcmp(to_account, "my529") == 0) {
            account2 = my529;
            sndBuf[2] = account2;
        } else {
            printf("Invalid account to transfer to...\n");
            exit(0);
        }
        printf("%s", argv[4]);
        sndBuf[3] = atoi(argv[4]);
    } else {
        printf("The message type entered was invalid.\n");
        exit(1);
    }

    send(clientSock, sndBuf, SNDBUFSIZE, 0);

    /* Receive and print response from the server */
    /*	    FILL IN	 */
    if (operation == BAL) {
        recv(clientSock, &balance, REPLYLEN, 0);
        close(clientSock);
        balance = ntohl(balance);
        printf("Balance is: %i\n", balance);
    } else if (operation == WITHDRAW) {
        recv(clientSock, rcvBuf, RCVBUFSIZE, 0);
        close(clientSock);
        printf("%s\n", rcvBuf);
    } else if (operation == TRANSFER){
        recv(clientSock, rcvBuf, RCVBUFSIZE, 0);
        close(clientSock);
        printf("%s\n", rcvBuf);
    }

    return 0;
}
