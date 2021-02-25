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
#include <time.h>
#include <unistd.h>
#include <vector>

using namespace std;

void *server_client(void *);

int list_movie_fun(char *check_msg);
int show_check_fun(char *check_msg);

void tomorrow_date();
struct arg {
  int connFD;
};
