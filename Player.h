#pragma once
#include <string>
#include <Marker.h>

class Player 
{

public:

	//properties
	int p_playerid;
	Marker p_pieces[5];

	//methods
	//CONSTRUCTOR

	Player()
	{
		//nothing
	}

	Player(int x_id)
	{

		int x_counter = 0;

		this->p_playerid = x_id;

		for (x_counter = 1; x_counter <= 4; x_counter++)
		{
			
			
			this->p_pieces[x_counter] = Marker(x_id);

			
		}

	}


	Marker p_assignpiece(int x_player)
	{

				
		return Marker(x_player);


	}

	int p_get_playerid()
	{
		return this->p_playerid;
	}

	bool p_check_game_status(Player x_player) {

		int x_counter = 0;

		//int x_temp_location = 0;

		bool x_game_over = 1;

				
		for (x_counter = 1; x_counter <= 4; x_counter++)
		{
			
			if (x_player.p_pieces[x_counter].m_get_location() != 120)
			{
				
				x_game_over = 0;
				
				
			}

			

			
		}

		return x_game_over;
	}


};