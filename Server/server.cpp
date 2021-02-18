#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>

using namespace std;

sockaddr_in servAddr, new_serverAddr;

int create_socket(int port_fun)
{
    //setup a socket and connection tools
                                      
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port_fun);

    //open stream oriented socket with internet address
    //also keep track of the socket descriptor
    return socket(AF_INET, SOCK_STREAM, 0);
}

int bind_socket(int serverFD)
{
    //bind the socket to its local address
    return bind(serverFD, (struct sockaddr*) &servAddr, sizeof(servAddr));
    
}

int listen_IP(int serverFD)
{
    //listen for up to 5 requests at a time
    return listen(serverFD, 5);
}

int request_accept(int serverFD)
{
    //accept, create a new socket descriptor to 
    //handle the new connection with client
   // return accept(serverFD, (sockaddr *)&new_serverAddr, (socklen_t *) sizeof(new_serverAddr));
    return accept(serverFD, (sockaddr *)NULL, NULL);
}

int send_Welcome_msg(int sFD)
{
    char msg[1000];
    recv(sFD, (char*)&msg, sizeof(msg), 0);
    strcpy(msg, "\nWelcome to xxx movie ticket booking\nPlease choose a option\n   1. Show Movie List\n   2. Show theatre List\n");
    return send(sFD, (char *)&msg, strlen(msg), 0);
}

int main(int argc, char *argv[])
{
    //for the server, we only need to specify a port number
    if(argc != 2)
    {
        cout << "Mention the port number\n";
        exit(0);
    }
    
    //grab the port number
    /*convert port number string (argv[1]) to integer (port)*/
    int port = atoi(argv[1]);   
    
    int serverFD = create_socket(port);
    if(serverFD == -1)
    {
        cout << "Error establishing the server socket" << endl;
        exit(0);
    }
    else
    {
        cout << "socket created successfully\n";
    }
    
    int bindStatus = bind_socket(serverFD);
    if(bindStatus == -1)
    {
        cout << "Error binding socket to local address" << endl;
        exit(0);
    }
    else 
    {
        cout << "socket get binded to the IP\n";
    }
    
    int listenS = listen_IP(serverFD);
    if( listenS == 0)
    {
        cout << "server is listening . . . \n";
    }
    else if (listenS == -1)
    {
        cout << "Error in listening\n";
        exit(0);
    }

    int new_serverFD = request_accept(serverFD);
    if(new_serverFD == -1)
    {
        cout << "Error accepting request from client! --- " << strerror(errno) << endl;
        exit(0);
    }
    else
    {
        cout << "Connected with client :))) " << endl;
    }

    if(send_Welcome_msg(new_serverFD) == -1)
    {
        cout << "Welcome Message Failed --- " << strerror(errno) << endl;
        exit(0);
    }
    else
    {
        cout << "Welcome message sent\n";
    }
/*
    while(1)
    {
        //receive a message from the client (listen)
        cout << "Awaiting client response..." << endl;
        memset(&msg, 0, sizeof(msg));//clear the buffer
        bytesRead += recv(newSd, (char*)&msg, sizeof(msg), 0);
        if(!strcmp(msg, "exit"))
        {
            cout << "Client has quit the session" << endl;
            break;
        }
        cout << "Client: " << msg << endl;
        cout << ">";
        string data;
        //data = "mes from server";
        //strcpy(data.c_str(), "message from client");
        getline(cin, data);
        memset(&msg, 0, sizeof(msg)); //clear the buffer
        strcpy(msg, data.c_str());
        if(data == "exit")
        {
            //send to the client that server has closed the connection
            send(newSd, (char*)&msg, strlen(msg), 0);
            break;
        }
        
    }
   */
    int closeS1 = close(new_serverFD);
    if(closeS1 == 0)
    {
        if(close(serverFD) == 0)
        {
            cout << "Connection closed..." << endl;
        }
        else
        {
            cout << "Connection not closed\n";
        }
    }
    else
    {
        cout << "Connection not closed\n";
    }
    return 0;   
}
