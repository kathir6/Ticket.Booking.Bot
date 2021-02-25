#include "server_client.h"

int list_movie_fun(char *check_msg) {
  char f1[] = "list";
  char f2[] = "movie";
  bool lsmov = ((strstr(check_msg, f1)) && (strstr(check_msg, f2)));
  if (lsmov) {
    return 1;
  } else {
    return 0;
  }
}

int show_check_fun(char *check_msg) {
  char s1[] = "1st", s2[] = "first";
  char s3[] = "2nd", s4[] = "second";
  char s5[] = "3rd", s6[] = "third";
  char s7[] = "4th", s8[] = "fourth";

  char s0[] = "show";

  if ((strstr(check_msg, s1) || strstr(check_msg, s2)) &&
      strstr(check_msg, s0)) {
    cout << "       show found   : success\n";

    return 1;
  } else if ((strstr(check_msg, s3) || strstr(check_msg, s4)) &&
             strstr(check_msg, s0)) {
    cout << "       show found   : success\n";
    return 2;
  } else if ((strstr(check_msg, s5) || strstr(check_msg, s6)) &&
             strstr(check_msg, s0)) {
    cout << "       show found   : success\n";
    return 3;
  } else if ((strstr(check_msg, s7) || strstr(check_msg, s8)) &&
             strstr(check_msg, s0)) {
    cout << "       show found   : success\n";
    return 4;
  } else {
    return 0;
  }
}
