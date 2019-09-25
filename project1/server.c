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
#include <time.h>

#define RCVBUFSIZE 512		/* The receive buffer size */
#define SNDBUFSIZE 512		/* The send buffer size */
#define BUFSIZE 40		/* Your name can be as many as 40 chars*/

enum operations{BAL, WITHDRAW, TRANSFER};
enum accountNames{myChecking, mySavings, myCD, my401k, my529};

int operation;
int account_name;
int account_name2;

struct accounts {
    int myChecking;
    int mySavings;
    int myCD;
    int my401K;
    int my529;
};

struct counters {
    int count_checking;
    int count_savings;
    int count_cd;
    int count_401k;
    int count_529;
};

int updateCounters(int account_name, struct counters *count);

/* The main function */
int main(int argc, char *argv[])
{

    int serverSock;				/* Server Socket */
    int clientSock;				/* Client Socket */
    struct sockaddr_in changeServAddr;		/* Local address */
    struct sockaddr_in changeClntAddr;		/* Client address */
    unsigned short changeServPort;		/* Server port */
    unsigned int clntLen;			/* Length of address data struct */

    int nameBuf[BUFSIZE];			/* Buff to store account name from client */
    int balance;				/* Place to record account balance result */
    int balance2;

    char *ip_address;

            /* Send Buffer */
    char rcvBuf[RCVBUFSIZE];        /* Receive Buffer */

    int move_money;

    struct accounts account;
    //initialize values for each account
    account.myChecking = 300;
    account.mySavings = 1000;
    account.myCD = 200;
    account.my401K = 5000;
    account.my529 = 1500;

    struct counters counter;
    //counters for each of the accounts to check for withdrawals per minute
    counter.count_checking = 3;
    counter.count_savings = 3;
    counter.count_cd = 3;
    counter.count_401k = 3;
    counter.count_529 = 3;

    ip_address = argv[1];
    changeServPort = (unsigned short) atoi(argv[2]);

    if (argc != 3)
    {
        printf("Incorrect number of arguments.");
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
    memset(&nameBuf, 0 ,BUFSIZE);
    changeServAddr.sin_family = AF_INET;
    inet_aton(ip_address, &changeServAddr.sin_addr);
    changeServAddr.sin_port = htons(changeServPort);

    /* Bind to local address structure */
    /*	    FILL IN	*/
    //(const struct sockaddr_in*)
    bind(serverSock, (struct sockaddr*)&changeServAddr, sizeof(changeServAddr));

    /* Listen for incoming connections */
    /*	    FILL IN	*/
    if (listen(serverSock, 10) == -1) {
        printf("Listening failed...\n");
        close(serverSock);
        exit(0);
    } else { printf("Listening successful..\n"); }


    time_t begin_time;
    time_t time_now;
    time(&begin_time);
    /* Loop server forever*/
    while(1)
    {

	/* Accept incoming connection */
	/*	FILL IN	    */
        clientSock = accept(serverSock, (struct sockaddr *) &changeClntAddr, &clntLen);
        /*if(clientSock == -1){
            printf("Accept connection failed...\n");
            close(serverSock);
            exit(0);
        }
        else { printf("Connection accepted..\n"); }
    */
        recv(clientSock, &nameBuf, BUFSIZE, 0);

        operation = nameBuf[0];
        account_name = nameBuf[1];

        struct accounts* account_p;
        account_p = &account;

        //BAL operation for all accounts
        if (operation == BAL){
            if(account_name == myChecking) {
                balance = htonl(account.myChecking);
            }
            else if (account_name == mySavings) {
                balance = htonl(account.mySavings);
            }
            else if (account_name == myCD) {
                balance = htonl(account.myCD);
            }
            else if (account_name == my401k) {
                balance = htonl(account.my401K);
            }
            else if (account_name == my529) {
                balance = htonl(account.my529);
            }
            send(clientSock, &balance, sizeof(balance), 0);
        }
        //WITHDRAW operation for all accounts
        else if (operation == WITHDRAW) {
            move_money = nameBuf[2];
            if(account_name == myChecking) {
                if (updateCounters(account_name, &counter) == 0) {
                    char sndBuf[SNDBUFSIZE] = "Too many withdraws in a minute. Try again later.";
                    send(clientSock, sndBuf, sizeof(sndBuf), 0);
                } else {
                    updateCounters(account_name, &counter);
                    balance = account.myChecking;
                    if (balance >= move_money) {
                        account.myChecking = balance - move_money;
                        //account.myChecking = (int) balance - (int) move_money;
                        char sndBuf[SNDBUFSIZE] = "Withdrawal successful! Balance updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                }
            }
            else if (account_name == mySavings) {
                if (updateCounters(account_name, &counter) == 0) {
                    char sndBuf[SNDBUFSIZE] = "Too many withdraws in a minute. Try again later.";
                    send(clientSock, sndBuf, sizeof(sndBuf), 0);
                } else {
                    balance = account.mySavings;
                    if (balance >= move_money) {
                        balance -= move_money;
                        account.mySavings -= move_money;
                        char sndBuf[SNDBUFSIZE] = "Withdrawal successful! Balance updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                }
            }
            else if (account_name == myCD) {
                if (updateCounters(account_name, &counter) == 0) {
                    char sndBuf[SNDBUFSIZE] = "Too many withdraws in a minute. Try again later.";
                    send(clientSock, sndBuf, sizeof(sndBuf), 0);
                } else {
                    balance = account.myCD;
                    if (balance >= move_money) {
                        balance = balance - move_money;
                        account.myCD = balance;
                        char sndBuf[SNDBUFSIZE] = "Withdrawal successful! Balance updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                }
            }
            else if (account_name == my401k) {
                if (updateCounters(account_name, &counter) == 0) {
                    char sndBuf[SNDBUFSIZE] = "Too many withdraws in a minute. Try again later.";
                    send(clientSock, sndBuf, sizeof(sndBuf), 0);
                } else {
                    balance = account.my401K;
                    if (balance >= move_money) {
                        balance = balance - move_money;
                        account.my401K = balance;
                        char sndBuf[SNDBUFSIZE] = "Withdrawal successful! Balance updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                }
            }
            else if (account_name == my529) {
                if (updateCounters(account_name, &counter) == 0) {
                    char sndBuf[SNDBUFSIZE] = "Too many withdraws in a minute. Try again later.";
                    send(clientSock, sndBuf, sizeof(sndBuf), 0);
                } else {
                    balance = account.my529;
                    if (balance >= move_money) {
                        balance = balance - move_money;
                        account.my529 = balance;
                        char sndBuf[SNDBUFSIZE] = "Withdrawal successful! Balance updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                }
            }
            send(clientSock, &balance, sizeof(balance), 0);
        }

        //TRANSFER command for all account cases
        else if (operation == TRANSFER) {
            account_name2 = nameBuf[2];
            move_money = nameBuf[3];

            //Transferring from myChecking
            if (account_name == myChecking) {
                balance = account.myChecking;
                if (account_name2 == myChecking) {
                    balance2 = account.myChecking;
                    if (balance >= move_money) {
                        account.myChecking = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                } else if (account_name2 == mySavings) {
                    balance2 = account.mySavings;
                    if (balance >= move_money) {
                        account.myChecking -= move_money;
                        account.mySavings += move_money;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                } else if (account_name2 == myCD) {
                    balance2 = account.myCD;
                    if (balance >= move_money) {
                        balance = balance - move_money;
                        balance2 = balance2 + move_money;
                        account.myChecking = balance;
                        account.myCD = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                } else if (account_name2 == my401k) {
                    balance2 = account.my401K;
                    if (balance >= move_money) {
                        balance = balance - move_money;
                        balance2 = balance2 + move_money;
                        account.myChecking = balance;
                        account.my401K = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                } else if (account_name2 == my529) {
                    balance2 = account.my529;
                    if (balance >= move_money) {
                        balance = balance - move_money;
                        balance2 = balance2 + move_money;
                        account.myChecking = balance;
                        account.my529 = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                }
            }
                //Transferring from myChecking done

                //Transferring from mySavings
            else if (account_name == mySavings) {
                balance = account.mySavings;
                if (account_name2 == myChecking) {
                    balance2 = account.myChecking;
                    if (balance >= move_money) {
                        balance = balance - move_money;
                        balance2 = balance2 + move_money;
                        account.mySavings = balance;
                        account.myChecking = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                } else if (account_name2 == mySavings) {
                    balance2 = account.mySavings;
                    if (balance >= move_money) {
                        account.mySavings = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                } else if (account_name2 == myCD) {
                    balance2 = account.myCD;
                    if (balance >= move_money) {
                        balance = balance - move_money;
                        balance2 = balance2 + move_money;
                        account.mySavings = balance;
                        account.myCD = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                } else if (account_name2 == my401k) {
                    balance2 = account.my401K;
                    if (balance >= move_money) {
                        balance = balance - move_money;
                        balance2 = balance2 + move_money;
                        account.mySavings = balance;
                        account.my401K = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                } else if (account_name2 == my529) {
                    balance2 = account.my529;
                    if (balance >= move_money) {
                        balance = balance - move_money;
                        balance2 = balance2 + move_money;
                        account.mySavings = balance;
                        account.my529 = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                }
            }
                //Transferring from mySavings done

                //Transferring from myCD
            else if (account_name == myCD) {
                balance = account.myCD;
                if (account_name2 == myChecking) {
                    balance2 = account.myChecking;
                    if (balance >= move_money) {
                        balance = balance - move_money;
                        balance2 = balance2 + move_money;
                        account.myCD = balance;
                        account.myChecking = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                } else if (account_name2 == mySavings) {
                    balance2 = account.mySavings;
                    if (balance >= move_money) {
                        balance = balance - move_money;
                        balance2 = balance2 + move_money;
                        account.myCD = balance;
                        account.mySavings = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                } else if (account_name2 == myCD) {
                    balance2 = account.myCD;
                    if (balance >= move_money) {
                        account.myCD = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                } else if (account_name2 == my401k) {
                    balance2 = account.my401K;
                    if (balance >= move_money) {
                        balance = balance - move_money;
                        balance2 = balance2 + move_money;
                        account.myCD = balance;
                        account.my401K = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                } else if (account_name2 == my529) {
                    balance2 = account.my529;
                    if (balance >= move_money) {
                        balance = balance - move_money;
                        balance2 = balance2 + move_money;
                        account.myCD = balance;
                        account.my529 = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                }
            }
                //Transferring from myCD done

                //Transferring from my401k
            else if (account_name == my401k) {
                balance = account.my401K;
                if (account_name2 == myChecking) {
                    balance2 = account.myChecking;
                    if (balance >= move_money) {
                        balance = balance - move_money;
                        balance2 = balance2 + move_money;
                        account.my401K = balance;
                        account.myChecking = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                } else if (account_name2 == mySavings) {
                    balance2 = account.mySavings;
                    if (balance >= move_money) {
                        balance = balance - move_money;
                        balance2 = balance2 + move_money;
                        account.my401K = balance;
                        account.mySavings = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                } else if (account_name2 == myCD) {
                    balance2 = account.myCD;
                    if (balance >= move_money) {
                        balance = balance - move_money;
                        balance2 = balance2 + move_money;
                        account.my401K = balance;
                        account.myCD = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                } else if (account_name2 == my401k) {
                    balance2 = account.my401K;
                    if (balance >= move_money) {
                        account.my401K = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                } else if (account_name2 == my529) {
                    balance2 = account.my529;
                    if (balance >= move_money) {
                        balance = balance - move_money;
                        balance2 = balance2 + move_money;
                        account.my401K = balance;
                        account.my529 = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                }
            }
                //Transferring from my401k done

                //Transferring from my529
            else if (account_name == my529) {
                balance = account.my529;
                if (account_name2 == myChecking) {
                    balance2 = account.myChecking;
                    if (balance >= move_money) {
                        balance = balance - move_money;
                        balance2 = balance2 + move_money;
                        account.my529 = balance;
                        account.myChecking = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                } else if (account_name2 == mySavings) {
                    balance2 = account.mySavings;
                    if (balance >= move_money) {
                        balance = balance - move_money;
                        balance2 = balance2 + move_money;
                        account.my529 = balance;
                        account.mySavings = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                } else if (account_name2 == myCD) {
                    balance2 = account.myCD;
                    if (balance >= move_money) {
                        balance = balance - move_money;
                        balance2 = balance2 + move_money;
                        account.my529 = balance;
                        account.myCD = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                } else if (account_name2 == my401k) {
                    balance2 = account.my401K;
                    if (balance >= move_money) {
                        balance = balance - move_money;
                        balance2 = balance2 + move_money;
                        account.my529 = balance;
                        account.my401K = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                } else if (account_name2 == my529) {
                    balance2 = account.my529;
                    if (balance >= move_money) {
                        account.my529 = balance2;
                        char sndBuf[SNDBUFSIZE] = "Transfer successful. Balances updated.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    } else {
                        char sndBuf[SNDBUFSIZE] = "Insufficient funds.";
                        send(clientSock, sndBuf, sizeof(sndBuf), 0);
                    }
                }

            }//Transferring from my529 done

        }//Transfer operations done

        close(clientSock);

        time(&time_now);
        if (difftime(begin_time, time_now) >= 60) {
            //one minute interval expires, reset count for all accounts
            time(&begin_time);
            counter.count_checking = 5;
            counter.count_savings = 5;
            counter.count_cd = 5;
            counter.count_401k = 5;
            counter.count_529 = 5;
        }


	/* Extract the account name from the packet, store in nameBuf */

	/* Look up account balance, store in balance */
	/*	FILL IN	    */

	/* Return account balance to client */
	/*	FILL IN	    */

    } //while(1) end

}

//helper function

int updateCounters(int account, struct counters *counter) {
    if (account == myChecking) {
        counter->count_checking--;
        return counter->count_checking;
    }
    else if (account == mySavings){
        counter->count_savings--;
        return counter->count_savings;
    }
    else if (account == myCD){
        counter->count_cd--;
        return counter->count_cd;
    }
    else if (account == my401k){
        counter->count_401k--;
        return counter->count_401k;
    }
    else if (account == my529){
        counter->count_529--;
        return counter->count_529;
    }
}


