#include "server_client.h"

void *server_client(void *arg_thread) {
  struct arg *args_thread = (struct arg *)arg_thread;
  int connFD = args_thread->connFD, sendS, recvS, closeS, showslot,
      ticket_count, ticket_price, movie_flag = 0, day_flag = 0, show_flag = 0, wel_flag = 0, list_movie_flag = 0, show_check_flag = 0, movie_check_flag = 0, next_2_day = 0, show_slot_dis, highest_slot;
  char send_msg[1500], recv_msg[1500], tp[100], mf[100], df[100], sf[100], temp[100];
  char *words;
  struct ticket_detail movie_ticket;
  vector<string> movie_vect, day_vect, show_vect;
  map <int, ticket_detail> dis_timings;
  
  /* recv_start_dummy_msg*/
  bzero(recv_msg, strlen(recv_msg));
  recv(connFD, (char *)&recv_msg, sizeof(recv_msg), 0);

  /* Start of conversation */
  // while (1)
  {
    /* Welcome_Note */
    bzero(send_msg, strlen(send_msg));
    strcpy(send_msg,
           "Welcome to xxx ticket booking system\nHow can I help you???\n");
    sendS = send(connFD, (char *)&send_msg, sizeof(send_msg), 0);
    if (sendS == -1) {
      cout << "     to client      : failed   --- " << strerror(errno) << "\n";
      exit(0);
    } else {
      cout << "     to client      : success\n";
    }

    /* list of movies | day | time */
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
    memset(&movie_ticket, 0, sizeof(movie_ticket));
    bzero(recv_msg, sizeof(recv_msg));
    recvS = recv(connFD, (char *)&recv_msg, sizeof(recv_msg), 0);
    cout << "    from client     : " << recv_msg << "\n";
    if (strcmp(recv_msg, "bye") == 0) {
      goto connect_end;
    }

  check_recv_msg_end:
    /*check for string content*/
    list_movie_flag = list_movie_fun(recv_msg);
    show_check_flag = show_check_fun(recv_msg);
    movie_flag = day_flag = show_flag = 0;
    next_2_day = get_next_two_day(recv_msg);
    wel_flag = wel_fun(recv_msg);
    for (int i = 0; i < sizeof(recv_msg); ++i) {
      recv_msg[i] = tolower(recv_msg[i]);
    }
    /* check for movie */
    for (int i = 0; i < movie_vect.size(); i++) {
      if (strstr(recv_msg, ((movie_vect[i]).c_str()))) {
        movie_flag = 1;
        strcpy(mf, (movie_vect[i]).c_str());
        cout << "    movie found     : success\n";
        break;
      }
    }
    /* check for day */
    {
      if (strstr(recv_msg, ((day_vect[1]).c_str())) || (next_2_day == 2)) {
        day_flag = 1;
        strcpy(df, (day_vect[1]).c_str());
        cout << "     day found      : success\n";
      } else if (strstr(recv_msg, ((day_vect[0]).c_str())) || (next_2_day == 1)) {
        day_flag = 1;
        strcpy(df, (day_vect[0]).c_str());
        cout << "     day found      : success\n";
      }
    }
    /* check for show */
    for (int i = 0; i < show_vect.size(); ++i) {
      if (strstr(recv_msg, ((show_vect[i]).c_str())) && (!strstr(recv_msg, "good"))) {
        show_flag = 1;
        strcpy(sf, (show_vect[i]).c_str());
        cout << "     show found     : success\n";
        break;
      }
    }
    if (show_flag || show_check_flag) {
      show_flag = 1;
    }
    /* only movie found */
    /* movie_found_flag && !(day_found_flag) && !(time_found_flag) */
    bzero(send_msg, strlen(send_msg));
    if ((movie_flag) && (!day_flag) && (!show_flag)) {
  // map
  highest_slot = 8;
      for(show_slot_dis = 1; show_slot_dis < 9; ++show_slot_dis)
      {
        strcpy(movie_ticket.movie_name, mf); 
        if(show_slot_dis <= 4)
          strcpy(movie_ticket.date, tomorrow().c_str()); 
        else if (show_slot_dis > 4)
                strcpy(movie_ticket.date, day_after_tomorrow().c_str());
          if(show_slot_dis%4 == 1)
          strcpy(movie_ticket.show, "09:00 AM");
          else if (show_slot_dis%4 == 2)
          strcpy(movie_ticket.show, "01:00 PM");
          else if (show_slot_dis%4 == 3)
          strcpy(movie_ticket.show, "05:00 PM");
          else if (show_slot_dis%4 == 0)
          strcpy(movie_ticket.show, "09:00 PM");
        
        dis_timings.insert(pair<int, ticket_detail>(show_slot_dis, movie_ticket));
      }
/* Display */
      strcpy(send_msg, "Choose one timings from the list below\n\n");
      strcat(send_msg, "\tMovie Name    - ");
      strcat(send_msg, mf);
      strcat(send_msg, "\n       Timing       \n          ");
      strcat(send_msg, "       01              02              03              "
                       "04        \n");

      strcat(send_msg, tomorrow().c_str());
      strcat(send_msg, "    09:00 AM        01:00 PM        05:00 PM        "
                       "09:00 PM     \n");
      strcat(send_msg, "          ");
      strcat(send_msg, "       05              06              07              "
                       "08        \n");
      strcat(send_msg, day_after_tomorrow().c_str());
      strcat(
          send_msg,
          "    09:00 AM        01:00 PM        05:00 PM        09:00 PM    \n");
      strcat(send_msg, "\nPlease select a show timing by choosing a number "
                       "near to it    \n");
    }
    /* only date found */
    /* (!movie_found_flag) && (day_found_flag) && !(time_found_flag) */
    else if ((!movie_flag) && (day_flag) && (!show_flag)) {
      //map
      highest_slot = movie_vect.size() * 4;
      int q=0;
      for(show_slot_dis = 1; show_slot_dis <= (movie_vect.size() * 4); ++show_slot_dis)
      {
        q = (show_slot_dis-1)/4;
      strcpy(movie_ticket.movie_name, (movie_vect[q]).c_str());
        

        if (strcmp(df, "tomorrow") == 0) 
                strcpy(movie_ticket.date, tomorrow().c_str());
         else if (strcmp(df, "day after tomorrow") == 0) 
          strcpy(movie_ticket.date, day_after_tomorrow().c_str());
        

          if(show_slot_dis%4 == 1)
          strcpy(movie_ticket.show, "09:00 AM");
          else if (show_slot_dis%4 == 2)
          strcpy(movie_ticket.show, "01:00 PM");
          else if (show_slot_dis%4 == 3)
          strcpy(movie_ticket.show, "05:00 PM");
          else if (show_slot_dis%4 == 0)
          strcpy(movie_ticket.show, "09:00 PM");
        
        dis_timings.insert(pair<int, ticket_detail>(show_slot_dis, movie_ticket));
        
      }
    /* Display */
      int show_slot = 01;
            strcpy(send_msg, "Choose one timings from the list below\n");

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
          strcat(send_msg, "\n");      strcat(send_msg, tomorrow().c_str());
        } else if (strcmp(df, "day after tomorrow") == 0) {
          strcat(send_msg, "\n");      strcat(send_msg, day_after_tomorrow().c_str());
        }
        strcat(send_msg, "    09:00 AM        01:00 PM        05:00 PM        "
                         "09:00 PM    \n");
        strcat(send_msg, "-----------------------------------------------------"
                         "----------------------------\n");
      }
      strcat(send_msg, "\nPlease select a show timing by choosing a number "
                       "near to it    \n");
    }
    /*only show found */
    /* (!movie_found_flag) && (!day_found_flag) && (time_found_flag) */
    else if ((!movie_flag) && (!day_flag) && (show_flag)) {
      strcpy(send_msg, "Choose one timings from the list below\n");


      highest_slot = movie_vect.size() * 2;
      // map
      int q=0;
      for(show_slot_dis = 1; show_slot_dis <= (movie_vect.size() * 2); ++show_slot_dis)
      {
        //cout << show_slot_dis << "\n";
        //for(; q < movie_vect.size(); q = (show_slot_dis-1)/4 )
        q = (show_slot_dis-1)/2;
        {strcpy(movie_ticket.movie_name, (movie_vect[q]).c_str());
       // cout << q << "\n";
        } // map

        if (show_slot_dis % 2 == 1) {
                strcpy(movie_ticket.date, tomorrow().c_str());
        } else if (show_slot_dis % 2 == 0) {
          strcpy(movie_ticket.date, day_after_tomorrow().c_str());
        }
/*
          if(show_slot_dis == 1)
          strcpy(movie_ticket.show, "09:00 AM");
          else if (show_slot_dis == 2)
          strcpy(movie_ticket.show, "01:00 PM");
          else if (show_slot_dis == 3)
          strcpy(movie_ticket.show, "05:00 PM");
          else if (show_slot_dis == 4)
          strcpy(movie_ticket.show, "09:00 PM");
*/
        if (strcmp(sf, "morning") == 0 || show_check_flag == 1) {
          strcpy(movie_ticket.show, "09:00 AM");
        } else if (strcmp(sf, "noon") == 0 || show_check_flag == 2) {
          strcpy(movie_ticket.show, "01:00 PM");
        } else if (strcmp(sf, "evening") == 0 || show_check_flag == 3) {
          strcpy(movie_ticket.show, "05:00 PM");
        } else if (strcmp(sf, "night") == 0 || show_check_flag == 4) {
          strcpy(movie_ticket.show, "09:00 PM");
        }

        dis_timings.insert(pair<int, ticket_detail>(show_slot_dis, movie_ticket));
        
      }
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
        strcat(send_msg, "\n");      strcat(send_msg, tomorrow().c_str());
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
        strcat(send_msg, "\n");      strcat(send_msg, day_after_tomorrow().c_str());
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
    /* movie and date found */
    /* (movie_found_flag) && (day_found_flag) && (!time_found_flag) */
    else if (((movie_flag) && (day_flag) && (!show_flag))) {
      strcpy(send_msg, "Choose one timings from the list below\n");

      highest_slot = 4;
      // map
      int q=0;
      for(show_slot_dis = 1; show_slot_dis < 5; ++show_slot_dis)
      {
        strcpy(movie_ticket.movie_name, mf);

        if (strcmp("tomorrow", df) == 0){
                strcpy(movie_ticket.date, tomorrow().c_str());
        } else if (strcmp("day after tomorrow", df) == 0) {
          strcpy(movie_ticket.date, day_after_tomorrow().c_str());
        }

        if(show_slot_dis == 1)
          strcpy(movie_ticket.show, "09:00 AM");
          else if (show_slot_dis == 2)
          strcpy(movie_ticket.show, "01:00 PM");
          else if (show_slot_dis == 3)
          strcpy(movie_ticket.show, "05:00 PM");
          else if (show_slot_dis == 4)
          strcpy(movie_ticket.show, "09:00 PM");

        dis_timings.insert(pair<int, ticket_detail>(show_slot_dis, movie_ticket));
        
      }

      strcat(send_msg, "     Movie Name    - ");
      strcat(send_msg, mf);
      strcat(send_msg, "\n       Timing       \n                    ");
      strcat(send_msg, "       01              02              03              "
                       "04        \n");
      if (strcmp(df, "tomorrow") == 0) {
        strcat(send_msg, tomorrow().c_str());
      } else {
        strcat(send_msg, day_after_tomorrow().c_str());
      }
      strcat(send_msg, "    09:00 AM        01:00 PM        05:00 PM        "
                       "09:00 PM     \n");

      strcat(send_msg, "\nPlease select a show timing by choosing a number "
                       "near to it    \n");
    }
    /* movie and time found */
    /* (movie_found_flag) && (!day_found_flag) && (time_found_flag) */
    else if (((movie_flag) && (!day_flag) && (show_flag))) {
      strcpy(send_msg, "Choose one timings from the list below\n");

      highest_slot = 2;
      // map
      int q=0;
      for(show_slot_dis = 1; show_slot_dis <= 2; ++show_slot_dis)
      {
        strcpy(movie_ticket.movie_name, mf);
      
        if (show_slot_dis % 2 == 1) {
                strcpy(movie_ticket.date, tomorrow().c_str());
        } else if (show_slot_dis % 2 == 0) {
          strcpy(movie_ticket.date, day_after_tomorrow().c_str());
        }
/*
          if(show_slot_dis == 1)
          strcpy(movie_ticket.show, "09:00 AM");
          else if (show_slot_dis == 2)
          strcpy(movie_ticket.show, "01:00 PM");
          else if (show_slot_dis == 3)
          strcpy(movie_ticket.show, "05:00 PM");
          else if (show_slot_dis == 4)
          strcpy(movie_ticket.show, "09:00 PM");
*/
        if (strcmp(sf, "morning") == 0 || show_check_flag == 1) {
          strcpy(movie_ticket.show, "09:00 AM");
        } else if (strcmp(sf, "noon") == 0 || show_check_flag == 2) {
          strcpy(movie_ticket.show, "01:00 PM");
        } else if (strcmp(sf, "evening") == 0 || show_check_flag == 3) {
          strcpy(movie_ticket.show, "05:00 PM");
        } else if (strcmp(sf, "night") == 0 || show_check_flag == 4) {
          strcpy(movie_ticket.show, "09:00 PM");
        }

        dis_timings.insert(pair<int, ticket_detail>(show_slot_dis, movie_ticket));
        
      }

      int show_slot = 01;
      strcat(send_msg, "\n     Movie Name    - ");
      strcat(send_msg, mf);
      strcat(send_msg, "\n       Timing       \n                           ");
      sprintf(tp, "%.2d", show_slot);
      show_slot++;
      strcat(send_msg, tp);
      strcat(send_msg, "\n");      strcat(send_msg, tomorrow().c_str());
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
      strcat(send_msg, "\n");      strcat(send_msg, day_after_tomorrow().c_str());
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
    /*date and time found*/
        /* (!movie_found_flag) && (day_found_flag) && (time_found_flag) */
    else if ((!movie_flag) && (day_flag) && (show_flag)) {
      strcpy(send_msg, "Choose one timings from the list below\n");

      highest_slot = movie_vect.size();
      // map
      int q=0;
      for(show_slot_dis = 1; show_slot_dis <= movie_vect.size(); ++show_slot_dis)
      {
        //cout << show_slot_dis << "\n";
        //for(; q < movie_vect.size(); q = (show_slot_dis-1)/4 )
        q = (show_slot_dis-1);
        {strcpy(movie_ticket.movie_name, (movie_vect[q]).c_str());
       // cout << q << "\n";
        } // map

        if (strcmp(df, "tomorrow") == 0) {
                strcpy(movie_ticket.date, tomorrow().c_str());
        } else if (strcmp(df, "day after tomorrow") == 0) {
          strcpy(movie_ticket.date, day_after_tomorrow().c_str());
        }
/*
          if(show_slot_dis == 1)
          strcpy(movie_ticket.show, "09:00 AM");
          else if (show_slot_dis == 2)
          strcpy(movie_ticket.show, "01:00 PM");
          else if (show_slot_dis == 3)
          strcpy(movie_ticket.show, "05:00 PM");
          else if (show_slot_dis == 4)
          strcpy(movie_ticket.show, "09:00 PM");
*/
        if (strcmp(sf, "morning") == 0 || show_check_flag == 1) {
          strcpy(movie_ticket.show, "09:00 AM");
        } else if (strcmp(sf, "noon") == 0 || show_check_flag == 2) {
          strcpy(movie_ticket.show, "01:00 PM");
        } else if (strcmp(sf, "evening") == 0 || show_check_flag == 3) {
          strcpy(movie_ticket.show, "05:00 PM");
        } else if (strcmp(sf, "night") == 0 || show_check_flag == 4) {
          strcpy(movie_ticket.show, "09:00 PM");
        }

        dis_timings.insert(pair<int, ticket_detail>(show_slot_dis, movie_ticket));
        
      }


      int show_slot = 01;
      for (int j = 0; j < movie_vect.size(); ++j) {
        strcat(send_msg, "\n     Movie Name    - ");
        strcat(send_msg, (movie_vect[j]).c_str());
        strcat(send_msg, "\n       Timing       \n                           ");
        sprintf(tp, "%.2d", show_slot);
        show_slot++;
        strcat(send_msg, tp);
        if (strcmp(df, "tomorrow") == 0) {
          strcat(send_msg, "\n");      strcat(send_msg, tomorrow().c_str());
        } else {
          strcat(send_msg, "\n");      strcat(send_msg, day_after_tomorrow().c_str());
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
    /* all found */
    /* (movie_found_flag) && (day_found_flag) && (time_found_flag) */

    else if ((movie_flag) && (day_flag) && (show_flag)) {
      strcpy(send_msg, "Choose one timings from the list below\n");

      highest_slot = 1;
      // map
      int q=0;
      for(show_slot_dis = 1; show_slot_dis <= 1; ++show_slot_dis)
      {
        //cout << show_slot_dis << "\n";
        //for(; q < movie_vect.size(); q = (show_slot_dis-1)/4 )
        q = (show_slot_dis-1);
        {strcpy(movie_ticket.movie_name, mf);
       // cout << q << "\n";
        } // map

        if (strcmp(df, "tomorrow") == 0) {
                strcpy(movie_ticket.date, tomorrow().c_str());
        } else if (strcmp(df, "day after tomorrow") == 0) {
          strcpy(movie_ticket.date, day_after_tomorrow().c_str());
        }
/*
          if(show_slot_dis == 1)
          strcpy(movie_ticket.show, "09:00 AM");
          else if (show_slot_dis == 2)
          strcpy(movie_ticket.show, "01:00 PM");
          else if (show_slot_dis == 3)
          strcpy(movie_ticket.show, "05:00 PM");
          else if (show_slot_dis == 4)
          strcpy(movie_ticket.show, "09:00 PM");
*/
        if (strcmp(sf, "morning") == 0 || show_check_flag == 1) {
          strcpy(movie_ticket.show, "09:00 AM");
        } else if (strcmp(sf, "noon") == 0 || show_check_flag == 2) {
          strcpy(movie_ticket.show, "01:00 PM");
        } else if (strcmp(sf, "evening") == 0 || show_check_flag == 3) {
          strcpy(movie_ticket.show, "05:00 PM");
        } else if (strcmp(sf, "night") == 0 || show_check_flag == 4) {
          strcpy(movie_ticket.show, "09:00 PM");
        }

        dis_timings.insert(pair<int, ticket_detail>(show_slot_dis, movie_ticket));
        
      }


      int show_slot = 01;
      strcat(send_msg, "\n     Movie Name    - ");
      strcat(send_msg, mf);
      strcat(send_msg, "\n       Timing       \n                           ");
      sprintf(tp, "%.2d", show_slot);
      show_slot++;
      strcat(send_msg, tp);
      if (strcmp(df, "tomorrow") == 0) {
        strcat(send_msg, "\n");      strcat(send_msg, tomorrow().c_str());
      } else {
        strcat(send_msg, "\n");      strcat(send_msg, day_after_tomorrow().c_str());
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
    /* list movie found and nothing found */
    else if (list_movie_flag == 1 ||
             (!movie_flag) && (!day_flag) && (!show_flag)) {
      if(wel_flag == 1)
      {
        if( strstr(recv_msg, "hi") )
        {
          strcpy(send_msg, "Hi\n");
        }
        else if ( strstr(recv_msg, "hello") )
        {
          strcpy(send_msg, "Hello\n");
        }
        else if ( strstr(recv_msg, "hai") )
        {
          strcpy(send_msg, "Hai\n");
        }
        else if ( strstr(recv_msg, "hey") )
        {
          strcpy(send_msg, "Hey\n");
        }
        else if ( strstr(recv_msg, "good") )
        {
          strcpy(send_msg, (good_fun()).c_str());
          strcat(send_msg, "\n");
        }
        strcat(send_msg, "Please check out the list of movies that are available currently\nSelect a movie to book a ticket\n");
      }
      else if (list_movie_flag == 1) {
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
      if( wel_flag != 1)
      {
        strcat(send_msg, "Do you want to continue? (yes/no)\n");
      }
      /* send continue request */
      sendS = send(connFD, (char *)&send_msg, sizeof(send_msg), 0);
      if (sendS == -1) {
        cout << "      to client     : failed   --- " << strerror(errno)
             << "\n";
        exit(0);
      } else {
        cout << "      to client     : success\n";
      }
      if( wel_flag == 1)
      {
        goto check_movie;
      }
yes_or_no_cont:
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
        strcpy(send_msg, "Thank You for contacting :)))\nHave a Nice Day\n\tPlease reach out the "
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
      else
      {
        bzero(send_msg, sizeof(send_msg));
        strcpy(send_msg, "\nPlease re-enter (yes/no) to continue\n");
        sendS = send(connFD, (char *)&send_msg, sizeof(send_msg), 0);
        goto yes_or_no_cont;
      }
    }
/* end of option list creation */
/*-------------------------------------------------
-----------------------------------------------------
-------------------------------------------------------
---------------------------------------------------------*/
    /* send option list */ // strcpy(send_msg, "dummy2");

    sendS = send(connFD, (char *)&send_msg, sizeof(send_msg), 0);
    if (sendS == -1) {
      cout << "      to client     : failed   --- " << strerror(errno) << "\n";
    } else {
      cout << "      to client     : success\n";
    }
listen_show_slot:
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
    cout << showslot << "\n";
    if( (showslot < 1) || (showslot > highest_slot) )
    {
      strcpy(send_msg, "\nPlease enter the correct show slot\n");
      sendS = send(connFD, (char *)&send_msg, sizeof(send_msg), 0);
      goto listen_show_slot;
    }
    else
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

      map<int, ticket_detail>::iterator itr; 
      itr = dis_timings.find(showslot);

      itr->second.ticket_count = ticket_count;

      if ((ticket_count > 0) && (ticket_count < 11)) {
        ticket_price = ticket_count * 140;
        strcpy(send_msg, "\n******TICKET DETAILS******\nMovie Name   : ");
        strcat(send_msg, itr->second.movie_name);
        strcat(send_msg, "\nDate         : ");
        strcat(send_msg, itr->second.date);
        strcat(send_msg, "\nTime         : ");
        strcat(send_msg, itr->second.show);
        strcat(send_msg, "\nTicket Count : ");
        sprintf(tp, "%d", itr->second.ticket_count);
        strcat(send_msg, tp);
        strcat(send_msg, "\nTicket Price : ");
        sprintf(tp, "%d", ticket_price);
        strcat(send_msg, tp);
        strcat(send_msg, "\n\nPlease confirm to book tickets (yes/no) . . . ");
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
yes_or_no_confirm:
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
               "contacting :)))\nHave a Nice Day\nPlease reach out the system for any help\nTo "
               "close connection enter \"bye\"");
      } else if (strcmp(recv_msg, "no") == 0) {
        bzero(send_msg, strlen(send_msg));
        strcpy(send_msg,
               "\n\nThank You for contacting :)))\nHave a Nice Day\nPlease reach out the system "
               "for any help\nTo close connection enter \"bye\"");
      }
      else
      {
        bzero(send_msg, strlen(send_msg));
        strcpy(send_msg, "\nPlease re-enter your confirmation (yes/no)\n");
        sendS = send(connFD, (char *)&send_msg, strlen(send_msg), 0);
        goto yes_or_no_confirm;
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