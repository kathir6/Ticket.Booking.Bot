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
#include <ctime> /* for tomorrow date calculation */ 

using namespace std;

void *server_client(void *);

int list_movie_fun(char *check_msg);
int show_check_fun(char *check_msg);

struct arg {
  int connFD;
};

struct struct_date
{
  int date;
  int month;
  int year;
};

string tomorrow();
string day_after_tomorrow();

int get_next_two_day(char *check_msg);

struct ticket_detail
{
  char movie_name[100];
  char movie_time[100];
  int ticket_count;
  int ticket_price;
};