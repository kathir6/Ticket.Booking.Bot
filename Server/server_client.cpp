#include "server_client.h"

void *server_client(void *arg_thread) 
{
  struct arg *args_thread = (struct arg *)arg_thread;
  int connFD = args_thread->connFD, sendS, recvS, showslot, ticket_count,
      ticket_price;
  char recv_msg[1500], send_msg[1500], tp[100];
  vector<string> movie_vect, day_vect, show_vect;

  bzero(recv_msg, strlen(recv_msg));
  recv(connFD, (char *)&recv_msg, sizeof(recv_msg), 0);
  // while (1)
  {
    bzero(send_msg, strlen(send_msg));
    strcpy(send_msg,
           "Welcome to xxx ticket booking system\nHow can I help you???\n");
    sendS = send(connFD, (char *)&send_msg, sizeof(send_msg), 0);
    if (sendS == -1) {
      cout << "      to client     : failed   --- " << strerror(errno) << "\n";
    } else {
      cout << "      to client     : success\n";
    }
  check_movie:
    bzero(recv_msg, strlen(recv_msg));
    recvS = recv(connFD, (char *)&recv_msg, sizeof(recv_msg), 0);
    if (strcmp(recv_msg, "bye") == 0) {
      if (close(connFD) == 0) {
        cout << "\n      stops with    : " << connFD << "\n\n";
        goto connect_end;
        // break;
      } else {
        cout << "      close         : failed\n";
      }
    }
    cout << "      from client   : " << recv_msg << "\n";
    /*check for string content*/
    movie_vect.push_back("chakra");
    movie_vect.push_back("uppena");
    day_vect.push_back("today");
    day_vect.push_back("tomorrow");
    show_vect.push_back("morning");
    show_vect.push_back("noon");
    show_vect.push_back("evening");
    show_vect.push_back("night");

    char *words;
    char mf[100], df[100], sf[100];
    int mf_flag = 0, g_flag = 0, day_flag = 0, show_flag = 0;
    words = strtok(recv_msg, " ");
    while (words != NULL) {
      for (int i = 0; i < strlen(words); ++i) {
        words[i] = tolower(words[i]);
      }
      for (int i = 0; i < movie_vect.size(); ++i) {
        if (strcmp(words, (movie_vect[i]).c_str()) == 0) {
          g_flag = mf_flag = 1;
          strcpy(mf, words);
          cout << "      movie found   : success\n";
          break;
        }
      }
      if (mf_flag == 1) {
        break;
      }
      words = strtok(NULL, " ");
    }
    bzero(send_msg, strlen(send_msg));
    if (mf_flag == 1) {
      strcpy(send_msg, "Choose one timings from the list below\n");
      strcat(send_msg, "     Movie Name    - ");
      strcat(send_msg, mf);
      strcat(send_msg, "\n       Timing       \n");
      // tomorrow_date();
      strcat(send_msg, "      Tomorrow      01 09:00 AM     02 01:00 PM     03 "
                       "05:00 PM     04 09:00 PM\n Day after tomorrow ");
      strcat(send_msg, "05 09:00 AM     06 01:00 PM     07 05:00 PM     08 "
                       "09:00 PM\n\nPlease select a show timing by choosing a "
                       "number near to it\n");
    } else {
      mf_flag = 0;
      strcpy(send_msg,
             "Sorry :((( Movie Not Found\nBelow are the movies currently "
             "available . . .\nDo you want to continue???\n");
    }

    sendS = send(connFD, (char *)&send_msg, sizeof(send_msg), 0);
    if (sendS == -1) {
      cout << "      to client     : failed   --- " << strerror(errno) << "\n";
    } else {
      cout << "      to client     : success\n";
    }

    bzero(recv_msg, strlen(recv_msg));
    recvS = recv(connFD, (char *)&recv_msg, sizeof(recv_msg), 0);
    if (strcmp(recv_msg, "bye") == 0) {
      if (close(connFD) == 0) {
        cout << "\n      stops with    : " << connFD << "\n\n";
        goto connect_end;
        // break;
      } else {
        cout << "      close         : failed\n";
      }
    }
    cout << "      from client   : " << recv_msg << "\n";

    bzero(send_msg, strlen(send_msg));
    if (mf_flag == 1) {
      showslot = atoi(recv_msg);
      strcpy(
          send_msg,
          "\nHow many tickets do you want???\nCount should be from 1 to 10\n");
    } else if ((mf_flag == 0) && (strcmp(recv_msg, "yes") == 0)) {
      strcpy(send_msg, "Please enter a movie name \n");
    } else if ((mf_flag == 0) && (strcmp(recv_msg, "no") == 0)) {
      strcpy(send_msg, "Thank You for contacting :)))\nPlease reach out the "
                       "system for any help\n");
    }
    sendS = send(connFD, (char *)&send_msg, sizeof(send_msg), 0);
    if (sendS == -1) {
      cout << "      to client     : failed   --- " << strerror(errno) << "\n";
    } else {
      cout << "      to client     : success\n";
    }
    if ((mf_flag == 0) && (strcmp(recv_msg, "yes") == 0)) {
      goto check_movie;
    }
    ticket_count:
    bzero(recv_msg, strlen(recv_msg));
    recvS = recv(connFD, (char *)&recv_msg, sizeof(recv_msg), 0);
    if (recvS == -1) {
      cout << "      from client   : failed\n";
    } else if (strcmp(recv_msg, "bye") == 0) {
      if (close(connFD) == 0) {
        cout << "\n      stops with    : " << connFD << "\n\n";
        goto connect_end;
        // break;
      } else {
        cout << "      close         : failed\n";
      }
    } else {
      cout << "      from client   : " << recv_msg << "\n";
      bzero(send_msg, strlen(send_msg));
      ticket_count = atoi(recv_msg);
      ticket_price = ticket_count * 140;
      strcpy(send_msg, "Ticket Price : ");
      sprintf(tp, "%d", ticket_price);
      strcat(send_msg, tp);
      strcat(send_msg, "\nPlease confirm to book tickets (y/n) . . . ");
    }
    if ((ticket_count < 1) || (ticket_count > 11)) 
    {
        bzero(send_msg, strlen(send_msg));
        strcpy(send_msg, "Ticket Count seems invalid\nPlease input again within the limit of 1 to 10. . .\n");
        sendS = send(connFD, (char *)&send_msg, strlen(send_msg), 0);
        if (sendS == -1) {
        cout << "      to client     : failed   --- " << strerror(errno) << "\n";
        } else {
        cout << "      to client     : success\n";
        }
        goto ticket_count;    
    }

    sendS = send(connFD, (char *)&send_msg, strlen(send_msg), 0);
    if (sendS == -1) {
      cout << "      to client     : failed   --- " << strerror(errno) << "\n";
    } else {
      cout << "      to client     : success\n";
    }

    bzero(recv_msg, strlen(recv_msg));
    recvS = recv(connFD, (char *)&recv_msg, sizeof(recv_msg), 0);
    if (recvS == -1) {
      cout << "      from client   : failed\n";
    } else if (strcmp(recv_msg, "bye") == 0) {
      if (close(connFD) == 0) {
        cout << "\n      stops with    : " << connFD << "\n\n";
        goto connect_end;
        // break;
      } else {
        cout << "      close         : failed\n";
      }
    } else {
      cout << "      from client   : " << recv_msg << "\n";
      if (strcmp(recv_msg, "y") == 0) {
        bzero(send_msg, strlen(send_msg));
        strcpy(send_msg,
               "Thank You :)))\nYour Tickets have been booked\n\nThank You for "
               "contacting :)))\nPlease reach out the system for any help\nTo "
               "close connection enter \"bye\"");
      } else if (strcmp(recv_msg, "n") == 0) {
        bzero(send_msg, strlen(send_msg));
        strcpy(send_msg,
               "\n\nThank You for contacting :)))\nPlease reach out the system "
               "for any help\nTo close connection enter \"bye\"");
      }
    }

    sendS = send(connFD, (char *)&send_msg, strlen(send_msg), 0);
    if (sendS == -1) {
      cout << "      to client     : failed   --- " << strerror(errno) << "\n";
    } else {
      cout << "      to client     : success\n";
    }

    bzero(recv_msg, strlen(recv_msg));
    recvS = recv(connFD, (char *)&recv_msg, sizeof(recv_msg), 0);
    if (recvS == -1) {
      cout << "      from client   : failed\n";
    } else if (strcmp(recv_msg, "bye") == 0) {
      if (close(connFD) == 0) {
        cout << "\n      stops with    : " << connFD << "\n\n";
        goto connect_end;
        // break;
      } else {
        cout << "      close         : failed\n";
      }
    } else {
      cout << "      from client   : " << recv_msg << "\n";
      if (strcmp(recv_msg, "bye") == 0) {
        goto connect_end;
      } else {
        goto check_movie;
      }
    }
  }
connect_end:
  int dummy;
}