#include "server_client.h"

void tomorrow_date() {
  struct tm time_struct;
  cout << time_struct.tm_sec << "\n";
  // time_struct = gmtime();
  cout << time_struct.tm_sec << "\n";
}