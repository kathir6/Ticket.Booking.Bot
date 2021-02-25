#include "server_client.h"

void *server_client(void *arg_thread) {
  struct arg *args_thread = (struct arg *)arg_thread;
  int connFD = args_thread->connFD, sendS, recvS, closeS, showslot,
      ticket_count, ticket_price, movie_flag = 0, day_flag = 0, show_flag = 0;
  char recv_msg[1500], tp[100], mf[100], df[100], sf[100];
  // char *send_msg = new char[2000];
  char send_msg[1500];
  int list_movie_flag = 0, show_check_flag = 0, movie_check_flag = 0;
  vector<string> movie_vect, day_vect, show_vect;
  char *words;

  /* recv_start_dummy_msg*/
  bzero(recv_msg, strlen(recv_msg));
  recv(connFD, (char *)&recv_msg, sizeof(recv_msg), 0);
  // while (1)
  {
    /* Welcome_Note */
    bzero(send_msg, strlen(send_msg));
    strcpy(send_msg,
           "Welcome to xxx ticket booking system\nHow can I help you???\n");
    sendS = send(connFD, (char *)&send_msg, sizeof(send_msg), 0);
    if (sendS == -1) {
      cout << "      to client     : failed   --- " << strerror(errno) << "\n";
      exit(0);
    } else {
      cout << "      to client     : success\n";
    }

    /*list*/
    movie_vect.push_back("uppena");
    movie_vect.push_back("master");
    movie_vect.push_back("black widow");
    movie_vect.push_back("the batman");
    day_vect.push_back("tomorrow");
    day_vect.push_back("day after tomorrow");
    show_vect.push_back("morning");
    show_vect.push_back("noon");
    show_vect.push_back("evening");
    show_vect.push_back("night");

  check_movie:
    /* recv_request */
    bzero(recv_msg, sizeof(recv_msg));
    recvS = recv(connFD, (char *)&recv_msg, sizeof(recv_msg), 0);
    cout << "      from client   : " << recv_msg << "\n";
    if (strcmp(recv_msg, "bye") == 0) {
      goto connect_end;
    }

    /*check for string content*/

  check_recv_msg_end:

    list_movie_flag = list_movie_fun(recv_msg);
    show_check_flag = show_check_fun(recv_msg);
    movie_flag = day_flag = show_flag = 0;
    for (int i = 0; i < sizeof(recv_msg); ++i) {
      recv_msg[i] = tolower(recv_msg[i]);
    }
    for (int i = 0; i < movie_vect.size(); i++) {
      if (strstr(recv_msg, ((movie_vect[i]).c_str()))) {
        movie_flag = 1;
        strcpy(mf, (movie_vect[i]).c_str());
        cout << "    movie found     : success\n";
        break;
      }
    }
    // for(int i=0; i < day_vect.size(); ++i)
    {
      if (strstr(recv_msg, ((day_vect[1]).c_str()))) {
        day_flag = 1;
        strcpy(df, (day_vect[1]).c_str());
        cout << "      day found     : success\n";
      } else if (strstr(recv_msg, ((day_vect[0]).c_str()))) {
        day_flag = 1;
        strcpy(df, (day_vect[0]).c_str());
        cout << "      day found     : success\n";
      }
    }
    for (int i = 0; i < show_vect.size(); ++i) {
      if (strstr(recv_msg, ((show_vect[i]).c_str()))) {
        show_flag = 1;
        strcpy(sf, (show_vect[i]).c_str());
        cout << "     show found     : success\n";
        break;
      }
    }
    if (show_flag || show_check_flag) {
      show_flag = 1;
    }
    /* movie_found_flag && !(day_found_flag) && !(time_found_flag) */
    bzero(send_msg, strlen(send_msg));
    if ((movie_flag) && (!day_flag) && (!show_flag)) {
      strcpy(send_msg, "Choose one timings from the list below\n\n");
      strcat(send_msg, "     Movie Name    - ");
      strcat(send_msg, mf);
      strcat(send_msg, "\n       Timing       \n                    ");
      strcat(send_msg, "       01              02              03              "
                       "04        \n");
      strcat(send_msg, "      Tomorrow      ");
      strcat(send_msg, "    09:00 AM        01:00 PM        05:00 PM        "
                       "09:00 PM     \n");
      strcat(send_msg, "                    ");
      strcat(send_msg, "       05              06              07              "
                       "08        \n");
      strcat(send_msg, " Day after Tomorrow ");
      strcat(
          send_msg,
          "    09:00 AM        01:00 PM        05:00 PM        09:00 PM    \n");
      strcat(send_msg, "\nPlease select a show timing by choosing a number "
                       "near to it    \n");
    }
    /* (!movie_found_flag) && (day_found_flag) && !(time_found_flag) */
    else if ((!movie_flag) && (day_flag) && (!show_flag)) {
      strcpy(send_msg, "Choose one timings from the list below\n");
      int show_slot = 01;
      strcat(send_msg, "-------------------------------------------------------"
                       "--------------------------\n");
      for (int j = 0; j < movie_vect.size(); ++j) {
        strcat(send_msg, "     Movie Name    - ");
        strcat(send_msg, (movie_vect[j]).c_str());
        strcat(send_msg, "\n       Timing       \n                           ");
        for (int k = 0; k < 4; k++) {
          sprintf(tp, "%.2d", show_slot);
          show_slot++;
          strcat(send_msg, tp);
          strcat(send_msg, "              ");
        }
        if (strcmp(df, "tomorrow") == 0) {
          strcat(send_msg, "\n      Tomorrow      ");
        } else if (strcmp(df, "day after tomorrow") == 0) {
          strcat(send_msg, "\n Day after Tomorrow ");
        }
        strcat(send_msg, "    09:00 AM        01:00 PM        05:00 PM        "
                         "09:00 PM    \n");
        strcat(send_msg, "-----------------------------------------------------"
                         "----------------------------\n");
      }
      strcat(send_msg, "\nPlease select a show timing by choosing a number "
                       "near to it    \n");
    }
    /* (!movie_found_flag) && (!day_found_flag) && (time_found_flag) */
    else if ((!movie_flag) && (!day_flag) && (show_flag)) {
      strcpy(send_msg, "Choose one timings from the list below\n");
      int show_slot = 01;
      for (int j = 0; j < movie_vect.size(); ++j) {
        strcat(send_msg, "-----------------------------------------------------"
                         "----------------------------\n");
        strcat(send_msg, "\n     Movie Name    - ");
        strcat(send_msg, (movie_vect[j]).c_str());
        strcat(send_msg, "\n       Timing       \n                           ");
        sprintf(tp, "%.2d", show_slot);
        show_slot++;
        strcat(send_msg, tp);
        strcat(send_msg, "\n      Tomorrow      ");
        if (strcmp(sf, "morning") == 0 || show_check_flag == 1) {
          strcat(send_msg, "    09:00 AM\n");
        } else if (strcmp(sf, "noon") == 0 || show_check_flag == 2) {
          strcat(send_msg, "    01:00 PM\n");
        } else if (strcmp(sf, "evening") == 0 || show_check_flag == 3) {
          strcat(send_msg, "    05:00 PM\n");
        } else if (strcmp(sf, "night") == 0 || show_check_flag == 4) {
          strcat(send_msg, "    09:00 PM\n");
        }
        strcat(send_msg, "                           ");
        sprintf(tp, "%.2d", show_slot);
        show_slot++;
        strcat(send_msg, tp);
        strcat(send_msg, "\n Day after Tomorrow ");
        if (strcmp(sf, "morning") == 0 || show_check_flag == 1) {
          strcat(send_msg, "    09:00 AM\n");
        } else if (strcmp(sf, "noon") == 0 || show_check_flag == 2) {
          strcat(send_msg, "    01:00 PM\n");
        } else if (strcmp(sf, "evening") == 0 || show_check_flag == 3) {
          strcat(send_msg, "    05:00 PM\n");
        } else if (strcmp(sf, "night") == 0 || show_check_flag == 4) {
          strcat(send_msg, "    09:00 PM\n");
        }
      }
      strcat(send_msg, "-------------------------------------------------------"
                       "--------------------------\n");
      strcat(send_msg, "\nPlease select a show timing by choosing a number "
                       "near to it    \n");
    }
    /* (movie_found_flag) && (day_found_flag) && (!time_found_flag) */
    else if (((movie_flag) && (day_flag) && (!show_flag))) {
      strcpy(send_msg, "Choose one timings from the list below\n");
      strcat(send_msg, "     Movie Name    - ");
      strcat(send_msg, mf);
      strcat(send_msg, "\n       Timing       \n                    ");
      strcat(send_msg, "       01              02              03              "
                       "04        \n");
      if (strcmp(df, "tomorrow") == 0) {
        strcat(send_msg, "      Tomorrow      ");
      } else {
        strcat(send_msg, " Day after Tomorrow ");
      }
      strcat(send_msg, "    09:00 AM        01:00 PM        05:00 PM        "
                       "09:00 PM     \n");

      strcat(send_msg, "\nPlease select a show timing by choosing a number "
                       "near to it    \n");
    }
    /* (movie_found_flag) && (!day_found_flag) && (time_found_flag) */
    else if (((movie_flag) && (!day_flag) && (show_flag))) {
      strcpy(send_msg, "Choose one timings from the list below\n");
      int show_slot = 01;
      strcat(send_msg, "\n     Movie Name    - ");
      strcat(send_msg, mf);
      strcat(send_msg, "\n       Timing       \n                           ");
      sprintf(tp, "%.2d", show_slot);
      show_slot++;
      strcat(send_msg, tp);
      strcat(send_msg, "\n      Tomorrow      ");
      if (strcmp(sf, "morning") == 0 || show_check_flag == 1) {
        strcat(send_msg, "    09:00 AM\n");
      } else if (strcmp(sf, "noon") == 0 || show_check_flag == 2) {
        strcat(send_msg, "    01:00 PM\n");
      } else if (strcmp(sf, "evening") == 0 || show_check_flag == 3) {
        strcat(send_msg, "    05:00 PM\n");
      } else if (strcmp(sf, "night") == 0 || show_check_flag == 4) {
        strcat(send_msg, "    09:00 PM\n");
      }
      strcat(send_msg, "                           ");
      sprintf(tp, "%.2d", show_slot);
      show_slot++;
      strcat(send_msg, tp);
      strcat(send_msg, "\n Day after Tomorrow ");
      if (strcmp(sf, "morning") == 0 || show_check_flag == 1) {
        strcat(send_msg, "    09:00 AM\n");
      } else if (strcmp(sf, "noon") == 0 || show_check_flag == 2) {
        strcat(send_msg, "    01:00 PM\n");
      } else if (strcmp(sf, "evening") == 0 || show_check_flag == 3) {
        strcat(send_msg, "    05:00 PM\n");
      } else if (strcmp(sf, "night") == 0 || show_check_flag == 4) {
        strcat(send_msg, "    09:00 PM\n");
      }

      strcat(send_msg, "\nPlease select a show timing by choosing a number "
                       "near to it    \n");
    }

    else if ((!movie_flag) && (day_flag) && (show_flag)) {
      strcpy(send_msg, "Choose one timings from the list below\n");
      int show_slot = 01;
      for (int j = 0; j < movie_vect.size(); ++j) {
        strcat(send_msg, "\n     Movie Name    - ");
        strcat(send_msg, (movie_vect[j]).c_str());
        strcat(send_msg, "\n       Timing       \n                           ");
        sprintf(tp, "%.2d", show_slot);
        show_slot++;
        strcat(send_msg, tp);
        if (strcmp(df, "tomorrow") == 0) {
          strcat(send_msg, "\n      Tomorrow      ");
        } else {
          strcat(send_msg, "\n Day after Tomorrow ");
        }
        if (strcmp(sf, "morning") == 0 || show_check_flag == 1) {
          strcat(send_msg, "    09:00 AM\n");
        } else if (strcmp(sf, "noon") == 0 || show_check_flag == 2) {
          strcat(send_msg, "    01:00 PM\n");
        } else if (strcmp(sf, "evening") == 0 || show_check_flag == 3) {
          strcat(send_msg, "    05:00 PM\n");
        } else if (strcmp(sf, "night") == 0 || show_check_flag == 4) {
          strcat(send_msg, "    09:00 PM\n");
        }
      }
      strcat(send_msg, "\nPlease select a show timing by choosing a number "
                       "near to it    \n");
    }

    else if ((movie_flag) && (day_flag) && (show_flag)) {
      strcpy(send_msg, "Choose one timings from the list below\n");
      int show_slot = 01;
      strcat(send_msg, "\n     Movie Name    - ");
      strcat(send_msg, mf);
      strcat(send_msg, "\n       Timing       \n                           ");
      sprintf(tp, "%.2d", show_slot);
      show_slot++;
      strcat(send_msg, tp);
      if (strcmp(df, "tomorrow") == 0) {
        strcat(send_msg, "\n      Tomorrow      ");
      } else {
        strcat(send_msg, "\n Day after Tomorrow ");
      }
      if (strcmp(sf, "morning") == 0 || show_check_flag == 1) {
        strcat(send_msg, "    09:00 AM\n");
      } else if (strcmp(sf, "noon") == 0 || show_check_flag == 2) {
        strcat(send_msg, "    01:00 PM\n");
      } else if (strcmp(sf, "evening") == 0 || show_check_flag == 3) {
        strcat(send_msg, "    05:00 PM\n");
      } else if (strcmp(sf, "night") == 0 || show_check_flag == 4) {
        strcat(send_msg, "    09:00 PM\n");
      }

      strcat(send_msg, "\nPlease select a show timing by choosing a number "
                       "near to it    \n");
    }
    /*--------------------------------------------------------------------
    ----------------------------------------------------------------------*/
    else if (list_movie_flag == 1 ||
             (!movie_flag) && (!day_flag) && (!show_flag)) {
      if (list_movie_flag == 1) {
        strcpy(send_msg, "List of Movies currently playing\n");
      } else {
        strcpy(send_msg, "Sorry :((( Movie Not Found\nBelow are the movies "
                         "currently available . . .\n");
      }
      for (int i = 0; i < movie_vect.size(); ++i) {
        strcat(send_msg, "    ");
        strcat(send_msg, (movie_vect[i]).c_str());
        strcat(send_msg, "\n");
      }
      strcat(send_msg, "Do you want to continue? (yes/no)\n");
      /* send continue request */
      sendS = send(connFD, (char *)&send_msg, sizeof(send_msg), 0);
      if (sendS == -1) {
        cout << "      to client     : failed   --- " << strerror(errno)
             << "\n";
        exit(0);
      } else {
        cout << "      to client     : success\n";
      }

      /* recv_continue */
      bzero(recv_msg, strlen(recv_msg));
      recvS = recv(connFD, (char *)&recv_msg, sizeof(recv_msg), 0);
      cout << "      from client   : " << recv_msg << "\n";

      if (recvS == -1) {
        cout << "      from client   : failed\n";
        exit(0);
      } else if (strcmp(recv_msg, "bye") == 0) {
        goto connect_end;
      } else if (strcmp(recv_msg, "yes") == 0) {
        strcpy(send_msg, "Please enter a movie name \n");

        /* send_request */
        sendS = send(connFD, (char *)&send_msg, sizeof(send_msg), 0);
        if (sendS == -1) {
          cout << "      to client     : failed   --- " << strerror(errno)
               << "\n";
          exit(0);
        } else {
          cout << "      to client     : success\n";
        }
        goto check_movie;
      } else if (strcmp(recv_msg, "no") == 0) {
        cout << "      from client   : " << recv_msg << "\n";
        strcpy(send_msg, "Thank You for contacting :)))\nPlease reach out the "
                         "system for any help\nEnter \"bye\"\n");
        /* send_end_note */
        sendS = send(connFD, (char *)&send_msg, sizeof(send_msg), 0);
        goto connect_end;
        if (sendS == -1) {
          cout << "      to client     : failed   --- " << strerror(errno)
               << "\n";
          exit(0);
        } else {
          cout << "No Continue\n      to client     : success\n";
        }
      }
    }

    /* send option list */ // strcpy(send_msg, "dummy2");

    sendS = send(connFD, (char *)&send_msg, sizeof(send_msg), 0);
    if (sendS == -1) {
      cout << "      to client     : failed   --- " << strerror(errno) << "\n";
    } else {
      cout << "      to client     : success\n";
    }

    /* recv_option */
    bzero(recv_msg, strlen(recv_msg));
    recvS = recv(connFD, (char *)&recv_msg, sizeof(recv_msg), 0);
    cout << "Option\n      from client   : " << recv_msg << "\n";
    if (strcmp(recv_msg, "bye") == 0) {
      goto connect_end;
    }

    /* send_ticket_count */
    bzero(send_msg, strlen(send_msg));
    showslot = atoi(recv_msg);
    strcpy(send_msg,
           "\nHow many tickets do you want?\nCount should be from 1 to 10\n");

    sendS = send(connFD, (char *)&send_msg, sizeof(send_msg), 0);
    if (sendS == -1) {
      cout << "      to client     : failed   --- " << strerror(errno) << "\n";
    } else {
      cout << "      to client     : success\n";
    }

    /* recv_ticket_count */
  ticket_count:
    bzero(recv_msg, strlen(recv_msg));
    recvS = recv(connFD, (char *)&recv_msg, sizeof(recv_msg), 0);
    cout << "      from client   : " << recv_msg << "\n";
    /* send_ticket_price */
    if (recvS == -1) {
      cout << "      from client   : failed\n" << strerror(errno) << "\n";
      exit(0);
    } else if (strcmp(recv_msg, "bye") == 0) {
      goto connect_end;
    } else {
      bzero(send_msg, strlen(send_msg));
      ticket_count = atoi(recv_msg);
      if ((ticket_count > 0) && (ticket_count < 11)) {
        ticket_price = ticket_count * 140;
        strcpy(send_msg, "Ticket Price : ");
        sprintf(tp, "%d", ticket_price);
        strcat(send_msg, tp);
        strcat(send_msg, "\nPlease confirm to book tickets (yes/no) . . . ");
      } else {
        bzero(send_msg, strlen(send_msg));
        strcpy(send_msg, "Ticket Count seems invalid\nPlease input again "
                         "within the limit of 1 to 10. . .\n");
        sendS = send(connFD, (char *)&send_msg, strlen(send_msg), 0);
        if (sendS == -1) {
          cout << "      to client     : failed   --- " << strerror(errno)
               << "\n";
        } else {
          cout << "      to client     : success\n";
        }
        goto ticket_count;
      }
    }

    sendS = send(connFD, (char *)&send_msg, strlen(send_msg), 0);
    if (sendS == -1) {
      cout << "      to client     : failed   --- " << strerror(errno) << "\n";
    } else {
      cout << "      to client     : success\n";
    }

    /* recv_confirmation */
    bzero(recv_msg, strlen(recv_msg));
    recvS = recv(connFD, (char *)&recv_msg, sizeof(recv_msg), 0);
    cout << "      from client   : " << recv_msg << "\n";

    /* send_end_note */
    if (recvS == -1) {
      cout << "      from client   : failed\n";
    } else if (strcmp(recv_msg, "bye") == 0) {
      goto connect_end;
    } else {
      if (strcmp(recv_msg, "yes") == 0) {
        bzero(send_msg, strlen(send_msg));
        strcpy(send_msg,
               "      Thank You :)))\n      Your Tickets have been "
               "booked\n\nThank You for "
               "contacting :)))\nPlease reach out the system for any help\nTo "
               "close connection enter \"bye\"");
      } else if (strcmp(recv_msg, "no") == 0) {
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

    /* recv_bye */
    bzero(recv_msg, strlen(recv_msg));
    recvS = recv(connFD, (char *)&recv_msg, sizeof(recv_msg), 0);
    cout << "      from client   : " << recv_msg << "\n";
    if (recvS == -1) {
      cout << "      from client   : failed\n";
    } else {
      if (strcmp(recv_msg, "bye") == 0) {
        goto connect_end;
      } else if (strcmp(recv_msg, "bye") != 0) {
        goto check_recv_msg_end;
      }
    }

  connect_end:
    closeS = close(connFD);
    if (closeS == 0) {
      cout << "\n      stops with    : " << connFD << "\n\n";
    } else {
      cout << "       close        : failed\n";
      exit(0);
    }
  }
  return 0;
}