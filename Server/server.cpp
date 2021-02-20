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
void *server_client(void *);

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
        cout << "      Mention port number\n      Recommended : ./server <port>\n";
        return 0;
    }

    /*convert port to integer from string*/
    portNo = atoi(argv[1]);
    /*Check with range for the port number*/
    if ((portNo > 65535) || (portNo < 2000)) {
        cerr << "      Please enter a port number between 2000 - 65535" << endl;
        return 0;
    }

    /*create a socket*/
    serverFD = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFD == -1) 
    {
        cerr << "      socket        : failed  --- " << strerror(errno) << endl;
        return 0;
    }
    else
    {
        cout << "      socket        : success\n";
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
        cerr << "      bind          : failed  --- " << strerror(errno) << endl;
        return 0;
    }
    else if(bindS == 0)
    {
        cout << "      bind          : success\n";
    }

    listenS = listen(serverFD, 5);
    if(listenS == 0)
    {
        cout << "      listen        : success\n";
    }
    else if(listenS == -1)
    {
        cout << "      listen        : failed  --- " << strerror(errno) << "\n";
    }

    socklen_t len = sizeof(ClientAddr);

    while(1) 
    {
        cout << "\n\n- - - waiting for client to connect - - -\n\n";
        connFD = accept(serverFD, (struct sockaddr *)&ClientAddr, &len);
        thread_vect.push_back(0);

        if (connFD < 0) 
        {
            cerr << "      connection    : failed  --- " << endl;
            return 0;
        } 
        else 
        {
            cout << "      connection    : success" << endl;
            cout << "      start with    : " << connFD << "\n\n";
        }
        struct arg args_in;
        args_in.connFD = connFD;
        pthread_create(&thread_vect[thread_vect.size()], NULL, &server_client, (void *)&args_in);
        pthread_join(thread_vect[thread_vect.size()], NULL);
    }
   return 0;
}

void *server_client(void *arg_thread) 
{
    struct arg *args_thread = (struct arg *)arg_thread;
    int connFD = args_thread->connFD, sendS, recvS;
    char recv_msg[1500], send_msg[1500];
    bzero(recv_msg, strlen(recv_msg));
    recv(connFD, (char*)&recv_msg, sizeof(recv_msg), 0);
    while (1) 
    {
        bzero(send_msg, strlen(send_msg));
        strcpy(send_msg, "msg from server");
        sendS = send(connFD, (char *)&send_msg, sizeof(send_msg), 0);
        if(sendS == -1)
        {
            cout << "      to client     : failed   --- " << strerror(errno) << "\n";
        }
        else
        {
            cout << "      to client     : success\n";
        }
        bzero(recv_msg, 256);
        recvS = recv(connFD, (char*)&recv_msg, sizeof(recv_msg), 0);
        if(strcmp(recv_msg,"bye") == 0)
        {
            if(close(connFD) == 0)
            {
                cout << "\n      stops with    : " << connFD << "\n\n";
                break;
            }
            else
            {
                cout << "      close         : failed\n";
            }
        }
        cout << "      from client   : " << recv_msg << "\n";
        
    }
}