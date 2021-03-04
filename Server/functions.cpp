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

int wel_fun(char *check_msg)
{
  vector<string> welcome_note_vect;
  int temp_flag = 0;
  welcome_note_vect.push_back("hi");
  welcome_note_vect.push_back("hello");
  welcome_note_vect.push_back("good");
  welcome_note_vect.push_back("hai");
  welcome_note_vect.push_back("hey");
  for(int i=0; i<welcome_note_vect.size(); i++)
  {
    if( strstr(check_msg, (welcome_note_vect[i]).c_str()) )
    {
      temp_flag = 1;
      break;
    }
    else 
    {
      temp_flag = 0;
    }
  }
  return temp_flag;
}

string good_fun()
{
  char good[100];
  time_t c_time;
  time(&c_time);
  tm *cs_time = localtime(&c_time);
  char wel[100];

  if( ( 0 <= cs_time->tm_hour ) && ( cs_time->tm_hour <= 11 ) )
  {
    return "Good Morning";
  }
  else if ( ( 12 <= cs_time->tm_hour ) && ( cs_time->tm_hour <= 16 ) )
  {
    return "Good Afternoon";
  }
  else if ( ( 17 <= cs_time->tm_hour ) && ( cs_time->tm_hour <= 21 ) )
  {
    return "Good Evening";
  }
  else
  {
    return "";
  }
}

int daysPerMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int leapyear(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
    }

struct_date get_tomorrow_date(struct struct_date now) {
    int daysmon = daysPerMonth[now.month-1];
    if (leapyear(now.year) && now.month == 2)
        daysmon++;
    if (++now.date > daysmon) {
        now.date = 1;
        if (++now.month > 12) {
            now.month = 1;
            now.year++;
        }
    }
    return now;
}

string tomorrow()
{
  char tom_date[100];

  time_t c_time;
  time(&c_time);
  tm *cs_time = localtime(&c_time);
  char c_date[500], temp[100];

  struct_date today_date, tomorrow_date;
  today_date.date = cs_time->tm_mday;
  today_date.month = cs_time->tm_mon + 1;
  today_date.year = cs_time->tm_year + 1900;

  tomorrow_date = get_tomorrow_date(today_date);

  //strcpy(tom_date, "     ");
  sprintf(temp, "%.2d", tomorrow_date.date);
  strcpy(tom_date, temp);
  strcat(tom_date, "-");
  sprintf(temp, "%.2d", tomorrow_date.month);
  strcat(tom_date, temp);
  strcat(tom_date, "-");
  sprintf(temp, "%.2d", tomorrow_date.year);
  strcat(tom_date, temp);
  strcat(tom_date, "     ");

  return tom_date;
}

string day_after_tomorrow()
{
  char tom_date[100];

  time_t c_time;
  time(&c_time);
  tm *cs_time = localtime(&c_time);
  char c_date[500], temp[100];

  struct_date today_date, tomorrow_date, day_after;
  today_date.date = cs_time->tm_mday;
  today_date.month = cs_time->tm_mon + 1;
  today_date.year = cs_time->tm_year + 1900;

  day_after = get_tomorrow_date(today_date);
  tomorrow_date = get_tomorrow_date(day_after);
  
  //strcpy(tom_date, "     ");
  sprintf(temp, "%.2d", tomorrow_date.date);
  strcpy(tom_date, temp);
  strcat(tom_date, "-");
  sprintf(temp, "%.2d", tomorrow_date.month);
  strcat(tom_date, temp);
  strcat(tom_date, "-");
  sprintf(temp, "%.2d", tomorrow_date.year);
  strcat(tom_date, temp);
  strcat(tom_date, "     ");

  return tom_date;
}



int get_next_two_day(char *check_msg)
{
  char tom_date[100], day_after_tom[100];
  time_t c_time;
  time(&c_time);
  tm *cs_time = localtime(&c_time);
  char c_date[500], temp[100];

  struct_date today_date, tomorrow_date, day_after;
  today_date.date = cs_time->tm_mday;
  today_date.month = cs_time->tm_mon + 1;
  today_date.year = cs_time->tm_year + 1900;
  
  tomorrow_date = get_tomorrow_date(today_date);

  sprintf(tom_date, "%.2d", tomorrow_date.date);
  strcat(tom_date, "-");
  sprintf(temp, "%.2d", tomorrow_date.month);
  strcat(tom_date, temp);
  strcat(tom_date, "-");
  sprintf(temp, "%.2d", tomorrow_date.year);
  strcat(tom_date, temp);

  char tom_date_bs[100];

  sprintf(tom_date_bs, "%.2d", tomorrow_date.date);
  strcat(tom_date_bs, "/");
  sprintf(temp, "%.2d", tomorrow_date.month);
  strcat(tom_date_bs, temp);
  strcat(tom_date_bs, "/");
  sprintf(temp, "%.2d", tomorrow_date.year);
  strcat(tom_date_bs, temp);

  day_after = get_tomorrow_date(tomorrow_date);
  
  sprintf(day_after_tom, "%.2d", day_after.date);
  strcat(day_after_tom, "-");
  sprintf(temp, "%.2d", day_after.month);
  strcat(day_after_tom, temp);
  strcat(day_after_tom, "-");
  sprintf(temp, "%.2d", day_after.year);
  strcat(day_after_tom, temp);

  char day_after_bs[100];

  sprintf(day_after_bs, "%.2d", day_after.date);
  strcat(day_after_bs, "/");
  sprintf(temp, "%.2d", day_after.month);
  strcat(day_after_bs, temp);
  strcat(day_after_bs, "/");
  sprintf(temp, "%.2d", day_after.year);
  strcat(day_after_bs, temp);

  if( (strstr(check_msg, tom_date)) || (strstr(check_msg, tom_date_bs)) )
  {
    return 1;
  }
  else if( (strstr(check_msg, day_after_tom)) || (strstr(check_msg, day_after_bs)) )
  {
    return 2;
  }
  else
  {
    return 0;
  }
}