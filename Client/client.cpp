#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cerr << "\tUsage: ip_address port" << endl;
    exit(0);
  }
  char *serverIp = argv[1];
  int port = atoi(argv[2]), closeS, recvS, sendS;
  char send_msg[1500], recv_msg[1500];
  struct hostent *host = gethostbyname(serverIp);

  sockaddr_in sendSockAddr;

  bzero((char *)&sendSockAddr, sizeof(sendSockAddr));
  sendSockAddr.sin_family = AF_INET;
  sendSockAddr.sin_addr.s_addr =
      inet_addr(inet_ntoa(*(struct in_addr *)*host->h_addr_list));
  sendSockAddr.sin_port = htons(port);

  int clientFD = socket(AF_INET, SOCK_STREAM, 0);
  int connectS =
      connect(clientFD, (sockaddr *)&sendSockAddr, sizeof(sendSockAddr));
  if (connectS == -1) {
    cout << "\n     connection     : failed  --- " << strerror(errno) << "\n";
    exit(0);
  } else if (connectS == 0) {
    cout << "\n     connection     : success\n\n";
  }
  strcpy(send_msg, "ping message");
  while (1) {
    send(clientFD, (char *)&send_msg, strlen(send_msg), 0);
    if (strcmp(send_msg, "bye") == 0) {
      goto exit_connect;
    }
    bzero(recv_msg, strlen(recv_msg));
    recvS = recv(clientFD, (char *)&recv_msg, sizeof(recv_msg), 0);
    if (recvS == -1) {
      cout << "        recv        : failed  --- " << strerror(errno) << "\n";
      exit(0);
    } else {
      cout << recv_msg << "\n";
    }
    cout << "                                                            --> ";
    bzero(send_msg, strlen(send_msg));
    cin.getline(send_msg, 1000);
  }
exit_connect:
  closeS = close(clientFD);
  if (closeS == 0) {
    cout << "       close        : success\n";
  } else {
    cout << "       close        : failed  --- " << strerror(errno) << "\n";
    exit(0);
  }
  return 0;
}