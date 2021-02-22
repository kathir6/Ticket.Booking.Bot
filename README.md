# Ticket_Booking_Bot

main()
{
	create_socket()
	bind_socket()
	listen_socket()
	while(1)
	{
		accept_client()
		thread_create(server_client)
		thread_join()
	}
}

server_client()
{
	send_welcome_note()
	recv_request()
	check_movie():
	search_for_movie_name()
		set_general_flag
		set_movie_found_flag
	search_for_day()
		set_general_flag
		set_day_found_flag
	search_for_show()
		set_general_flag
		set_show_found_flag
	if(movie_found_flag)
	{
		send_movie_date_time()
	}
	else if(day_found_flag)
	{
		send_movie_list_time()
	}
	else if(time_found_flag)
	{
		send_movie_list_day()
	}
	else if(movie_found_flag && day_found_flag)
	{
		send_time()
	}
	else if(movie_found_flag && time_found_flag)
	{
		send_day()
	}
	else if(day_found_flag && time_found_flag)
	{
		send_movie_list()
	}
	else if(movie_found_flag && day_found_flag && time_found_flag)
	{
		send_particular_show()
	}
	else if(no_general_flag_continue)
	{
		send_again_request()
		goto check_movie()
	}
	else if(no_general_flag_no_continue)
	{
		send_end_note()
		close_connection()
	}
	recv_option()
	send_ticket_count()
	recv_ticket_count()
	send_ticket_price()
	recv_confirmation()
	send_end_note()
	close_connection()
}