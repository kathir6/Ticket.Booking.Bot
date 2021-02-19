#include <arpa/inet.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *task1(void *);

struct arg
{
    int connFD;
};

int main(int argc, char *argv[]) 
{
    int serverFD, portNo, bindS, connFD;
    struct sockaddr_in ServerAddr, ClientAddr;

    pthread_t thread[10];

    if (argc != 2) 
    {
        cout << "Mention port number\nRecommded : ./server <port>\n";
        return 0;
    }

    /*convert port to integer from string*/
    portNo = atoi(argv[1]);

    if ((portNo > 65535) || (portNo < 2000)) {
        cerr << "Please enter a port number between 2000 - 65535" << endl;
        return 0;
    }

    /*create a socket*/
    serverFD = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFD < 0) 
    {
        cerr << "Cannot open socket" << endl;
        return 0;
    }

    /*set ServerAddr*/
    bzero((char *)&ServerAddr, sizeof(ServerAddr));
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = INADDR_ANY;
    ServerAddr.sin_port = htons(portNo);

    /*bind socket to local IP address*/
    bindS = bind(serverFD, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr));
    if (bindS < 0) 
    {
        cerr << "Cannot bind" << endl;
        return 0;
    }

    listen(serverFD, 5);

    int noThread = 0;

    while (noThread < 3) 
    {
        socklen_t len = sizeof(ClientAddr);

        cout << "Listening" << endl;

        // this is where client connects. svr will hang in this mode until client
        // conn
        connFD = accept(serverFD, (struct sockaddr *)&ClientAddr, &len);

        if (connFD < 0) {
        cerr << "Cannot accept connection" << endl;
        return 0;
        } else {
        cout << "Connection successful" << endl;
        }
        struct arg args_in;
        args_in.connFD = connFD;
        pthread_create(&thread[noThread], NULL, &task1, (void *)&args_in);

        noThread++;
    }

    for (int i = 0; i < 3; i++) 
    {
        pthread_join(thread[i], NULL);
    }
}

void *task1(void *arg_thread) 
{
    pthread_mutex_lock(&lock);
    struct arg *args_thread = (struct arg *)arg_thread;
    int connFD = args_thread->connFD;
    
    cout << "new connection\n";
    char test[256];
    bzero(test, 256);
    recv(connFD, (char*)&test, sizeof(test), 0);
    while (1) 
    {
        char sendmsg[100] = "message from Server";
        bzero(sendmsg, 100);
        strcpy(sendmsg, "msg a");
        cout << "enter msg to client : ";
        cin >> sendmsg;
        int sendS;
        sendS = send(connFD, (char *)&sendmsg, sizeof(sendmsg), 0);
        if(sendS == -1)
        {
            cout << "error in msg sent : " << strerror(errno) << "\n";
        }
        else
        {
            cout << "message sent success\n";
        }
        bzero(test, 256);
        recv(connFD, (char*)&test, sizeof(test), 0);
        if(strcmp(test,"bye") == 0)
        {
            close(connFD);
            break;
        }
        printf("Here is the message: %s\n", test);
        
    }
    cout << "\nClosing thread and conn" << endl;
    pthread_mutex_unlock(&lock);
    cout << "thread unlocked\n";
}