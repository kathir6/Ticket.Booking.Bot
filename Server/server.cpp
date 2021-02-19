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
#include <vector>

using namespace std;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
vector <pthread_t> thread_vect;
void *task1(void *);

struct arg
{
    int connFD;
};

int main(int argc, char *argv[]) 
{
    int serverFD, portNo, bindS, listenS, connFD;
    struct sockaddr_in ServerAddr, ClientAddr;

    //pthread_t thread[10];

    if (argc != 2) 
    {
        cout << "Mention port number\nRecommended : ./server <port>\n";
        return 0;
    }

    /*convert port to integer from string*/
    portNo = atoi(argv[1]);
    /*Check with range for the port number*/
    if ((portNo > 65535) || (portNo < 2000)) {
        cerr << "Please enter a port number between 2000 - 65535" << endl;
        return 0;
    }

    /*create a socket*/
    serverFD = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFD == -1) 
    {
        cerr << "Cannot open socket : " << strerror(errno) << endl;
        return 0;
    }
    else
    {
        cout << "socket gets created\n";
    }

    /*set ServerAddr*/
    bzero((char *)&ServerAddr, sizeof(ServerAddr));
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = INADDR_ANY;
    ServerAddr.sin_port = htons(portNo);

    /*bind socket to local IP address*/
    bindS = bind(serverFD, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr));
    if (bindS == -1) 
    {
        cerr << "Cannot bind : " << strerror(errno) << endl;
        return 0;
    }
    else if(bindS == 0)
    {
        cout << "bind success\n";
    }

    listenS = listen(serverFD, 5);
    if(listenS == 0)
    {
        cout << "server starts to listen . . .\n";
    }
    else if(listenS == -1)
    {
        cout << "error in listening : " << strerror(errno) << "\n";
    }

    while(1) 
    {
        socklen_t len = sizeof(ClientAddr);

        // this is where client connects. svr will hang in this mode until client
        connFD = accept(serverFD, (struct sockaddr *)&ClientAddr, &len);
        thread_vect.push_back(0);

        if (connFD < 0) 
        {
            cerr << "Cannot accept connection" << endl;
            return 0;
        } 
        else 
        {
            cout << "Connection successful" << endl;
        }
        struct arg args_in;
        args_in.connFD = connFD;
        pthread_create(&thread_vect[thread_vect.size()], NULL, &task1, (void *)&args_in);
    }
    /*
    while(1) 
    {
        pthread_join(thread[i], NULL);
    }
    */
   return 0;
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