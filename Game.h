#pragma once
using namespace std;
#include <string>
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
#include <cstdlib>
#include <ctime>

#include <Marker.h>
#include <Player.h>

class Game {

public:

	//properties

	int g_movecounter = 0; //for logging

	std::string g_movelog[1000000];

	int g_playerturn;

	Player g_players[3] = { Player(0), Player(1), Player(2) };

	int g_dievalue = 0;

	bool g_gameover = 0;

	int m_owner;

	int m_location;

	//CONSTRUCTOR

	Game()
	{
		this->g_movecounter = 0;

		this->g_playerturn = this->g_return_random(2);

		this->g_logmove("Player " + to_string(this->g_playerturn) + " will go first.");

		this->g_dievalue = 0;

		this->g_gameover = 0;

		//rest of game

		this->g_newgame();

		//this->g_displaylog();

	}

	//DESTRUCTOR
	~Game()
	{


	}

	//methods
	void g_newgame() {
		//create players was done in constructor, now assign markers

		this->g_logmove("Player 1 created.");

		this->g_logmove("Player 2 created.");

	}

	int g_get_playerturn() {
		return this->g_playerturn;
	}

	void g_flip_player() {
		if (this->g_playerturn == 1) {
			this->g_playerturn = 2;
		}
		else
		{
			this->g_playerturn = 1;

		}
	}

	Player g_return_other_player() {


		if (this->g_playerturn == 1) {

			return this->g_players[2];


		}
		else
		{
			return this->g_players[1];

		}


	}

	void g_logmove(std::string x_logmove)
	{
		std::string x_timestamp;

		//get timestamp
		time_t _tm = time(NULL);

		struct tm* curtime = localtime(&_tm);

		x_timestamp = asctime(curtime);

		x_timestamp.pop_back();

		this->g_movelog[this->g_movecounter] = x_timestamp + " -- " + x_logmove;

		this->g_movecounter++;


	}

	int g_diceroll() {

		this->g_dievalue = this->g_return_random(6);
		
		return this->g_dievalue;
	}


	Player g_return_player(int x_playerid) {

		return this->g_players[x_playerid];
		

	}

	void g_displaylog()
	{
		int x_counter = 0;

		for (x_counter = 0; x_counter <= this->g_movecounter; x_counter++)
		{
			cout << this->g_movelog[x_counter] << "\n";
		}


	}

	//core calling function from controlling class
	bool g_player_action(int x_playerid) {

		//properties
		bool x_result = 0;

		std::string x_test = "";

		std::string x_temp_return = "";
				
		int x_temp_magic_numbers[] = { 0, 20, 24, 30, 40, 60, 120 };

		int x_temp_factor_numbers[] = { 0, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15, 25, 50 };

		std::string x_logstring = "";

		this->g_dievalue = this->g_diceroll();

		x_logstring = "[[[Player " + to_string(x_playerid) + " rolled a " + to_string(this->g_dievalue) + " ]]]";

		this->g_logmove(x_logstring);



		if (this->g_dievalue == 1) {

			this->g_logmove("Player " + to_string(x_playerid)  + " has to forfeit their move!");

		}
		else
		{

			//SET A

			x_result = this->g_target_magic_numbers(x_playerid, x_temp_magic_numbers, "penultimate",6);
				
			if (x_result)
			{
				x_temp_return = "TRUE";
			}
			else {
				x_temp_return = "FALSE";
			}

			x_temp_return = "";



			//SET B
			if (!x_result)
			{
				this->g_logmove("Player " + to_string(x_playerid) + " did not find any penultimate targets.");


					x_result = this->g_target_magic_numbers(x_playerid, x_temp_factor_numbers, "factor", 12);
				

				if (x_result)
				{
					x_temp_return = "TRUE";
				}
				else { x_temp_return = "FALSE";
				}

				x_temp_return = "";


			}

			//SET C
			if (!x_result)
			{
				//call function C here, return TRUE if it triggers successfully

				this->g_logmove("Player " + to_string(x_playerid) + " did not find any factor targets.");

				x_result = this->g_target_potential_clashes_set_C(this->g_players[x_playerid]);


				if (x_result)
				{
					x_temp_return = "TRUE";
				}
				else {
					x_temp_return = "FALSE";
				}


				x_temp_return = "";

			}

			//SET D
			if (!x_result)
			{

				x_result = this->g_move_onto_board_set_D(this->g_players[x_playerid]);

				if (x_result)
				{
					x_temp_return = "TRUE";
				}
				else {
					x_temp_return = "FALSE";
				}

				x_temp_return = "";

			
			}

		}

		if (this->g_gameover)
		{
			cout << "**************************************\n";

			cout << "***   Player " + to_string(x_playerid) + " HAS WON THE MATCH! ***\n";

			cout << "**************************************\n";

			this->g_logmove("************************************");

			this->g_logmove("***   Player " + to_string(x_playerid) + " HAS WON THE MATCH! ***");


			this->g_logmove("************************************");

		}

		return this->g_gameover;


	}


	bool g_move_onto_board_set_D(Player x_player)
	{


		int x_piece_pointer = this->g_find_to_move_onto_board(x_player);

		if (x_piece_pointer == 0)
		{

			//no more pieces to move on to the board
			this->g_logmove("Player " + to_string(x_player.p_get_playerid())  + " did not have pieces to move into play.");
			
			return 0;

			}

			else

			{
				this->g_marker_move(x_player.p_get_playerid(), x_piece_pointer);
								
				return 1;
			}
		}

	

	//D - find a marker not on the board, select one to move

	int g_find_to_move_onto_board(Player x_player) {

		int x_piece_array_pointer[5];

		int x_counter_array = 0;

		int x_counter = 0;




		for (x_counter = 1; x_counter <= 4; x_counter++) 
		{


			if (x_player.p_pieces[x_counter].m_get_location() == 1) {
	
				x_piece_array_pointer[++x_counter_array] = x_counter;

			}


		}

		//If there is one or more, return one at random
		if (x_counter_array > 0) {

			x_counter = this->g_return_random(x_counter_array);

			return x_piece_array_pointer[x_counter];
		}
		else
		{
			//there were no markers 'off the board'...return an empty pointer;

			this->g_logmove("Player " + to_string(x_player.p_get_playerid()) + " was in SET D but could not find markers.");


			return 0;
		}
	}

	void g_marker_move(int x_playerid, int x_piece_pointer)
	{
		std::string x_logstring = "";

		std::string x_text = "";

		int x_old_position = 0;

		int x_new_location = 0;

		bool x_clash_result = 0;

		//Player x_player = this->g_return_player(x_playerid);


		Player x_other_player = this->g_return_other_player();

		x_old_position = this->g_players[x_playerid].p_pieces[x_piece_pointer].m_get_location();

				
		if (this->g_dievalue != 0)
		{
		
			x_new_location = this->g_players[x_playerid].p_pieces[x_piece_pointer].m_calclocation(this->g_dievalue);

		}

		else

		{
			//Piece has been bumped to the start either due to clash or blowout
			x_new_location = 1;
		}

		if (x_new_location > 120)
		{
			//blowout!
			x_logstring = "Player " + to_string(x_playerid)  + " busted piece " + to_string(x_piece_pointer) + " to a value of " + to_string(x_new_location) + "!";

			this->g_logmove(x_logstring);

			x_new_location = 1;

		}

		//NOT SURE IF THIS IS WORKING!!
		
		this->g_players[x_playerid].p_pieces[x_piece_pointer].m_setlocation(x_new_location);

		x_logstring = "[[[Player " + to_string(x_playerid)  + " moved piece " + to_string(x_piece_pointer) + " from position " + to_string(x_old_position) + " to " + to_string(this->g_players[x_playerid].p_pieces[x_piece_pointer].m_get_location())  + "]]]";

		this->g_logmove(x_logstring);

		//call clash detect unless it has moved to 1

		if (x_new_location == 120)
		{
			//piece has made it to the end and will be disabled

			if (this->g_players[x_playerid].p_pieces[x_piece_pointer].m_get_status())
			{
				x_text = "ACTIVE";

			}

			else

			{
				x_text = "INACTIVE";

			}

			x_logstring = "Player " + to_string(x_playerid) + "s piece " + to_string(x_piece_pointer) + " has reached position " + to_string(this->g_players[x_playerid].p_pieces[x_piece_pointer].m_get_location())  + " successfully and is now " + x_text;

			this->g_logmove(x_logstring);

			}


		if (x_new_location != 1 && x_new_location != 120)
		{

			x_clash_result = this->g_detect_clash(this->g_players[x_playerid].p_pieces[x_piece_pointer].m_get_location(), x_other_player);
		}

		
		this->g_gameover = this->g_players[x_playerid].p_check_game_status(this->g_players[x_playerid]);

		
		}


	bool g_detect_clash(int x_location, Player x_player) {

		//iterate through opposing players active pieces and reset them if the new move has caused a clash

		  //temp properties
				
		int x_counter = 0;
		
		bool x_return_flag = 0;


		for (x_counter = 1; x_counter <= 4; x_counter++) {

			//find an (opposition) player's marker which is on the board but active

			if (x_player.p_pieces[x_counter].m_get_location() == x_location && x_player.p_pieces[x_counter].m_get_status())
			{

				//bump the clash piece

				this->g_dievalue = 0;

				this->g_marker_move(x_player.p_get_playerid(), x_counter);

				this->g_logmove("Player "  + to_string(x_player.p_get_playerid()) + "'s piece " + to_string(x_counter) + " was bumped to the start of the board!");
				
				x_return_flag = 1;

			} //if it doesn't find a clash, do nothing

		}

		return x_return_flag;
	}



	int g_find_to_move_in_play(Player x_player)
	{

		//temp properties
		int x_piece_array_pointer[5];

		int x_piece_array_backup[5];

		int x_counter_array = 0;

		int x_counter_loop = 0;

		int x_temp_value = 0;

		int x_counter = 0;

		int x_temp_magic_numbers[] = { 20,24,30,40,60 };

		bool x_test_1;

		bool x_test_2;

		for (x_counter = 1; x_counter <= 4; x_counter++)
		{

			//find a player's marker which is active

			if (x_player.p_pieces[x_counter].m_get_status())

			{
				x_counter_array++;

				x_piece_array_pointer[x_counter_array] = x_counter;

				x_piece_array_backup[x_counter_array] = x_counter;

			}

			
		}

		//If there is one or more, return one at random
		if (x_counter_array > 0)
		{

			this->g_logmove("Considering between " + to_string(x_counter_array) + " potential piece(s).");


			for (x_counter_loop = 1; x_counter_loop <= x_counter_array; x_counter_loop++)
			{
				x_test_1 = 0;

				x_test_2 = 0;

				x_temp_value = x_piece_array_pointer[x_counter_loop];

				this->g_logmove("Step " + to_string(x_counter_loop) + " of " + to_string(x_counter_array) + "..Looking at piece: " + to_string(x_temp_value) + " at location " + to_string(x_player.p_pieces[x_temp_value].m_get_location()));

				x_test_1 = std::find(std::begin(x_temp_magic_numbers), std::end(x_temp_magic_numbers), x_player.p_pieces[x_temp_value].m_get_location()) != std::end(x_temp_magic_numbers);

				if (x_test_1)
				{
					this->g_logmove("Considering ignoring piece " + to_string(x_temp_value) + " as it is on a penultimate number.");
				}

				if ((x_player.p_pieces[x_temp_value].m_get_location() * this->g_dievalue) > 120)
				{
					x_test_2 = 1;

					this->g_logmove("Considering ignoring piece " + to_string(x_temp_value) + " as it may cause a blowout.");

				}

				if (x_test_1 || x_test_2)
				{
					//one of these is a 'good' number we wish to avoid moving randomly, or could cause a blowout

					x_piece_array_backup[x_counter_loop] = 999;
				}

			}

			//find the number of pieces in the backup array
			x_counter = 0;

				for(x_counter_loop = 1; x_counter_loop <= 4; x_counter_loop++)
				{
					if (x_piece_array_backup[x_counter_loop] != 999 && x_piece_array_backup[x_counter_loop] > 0)
					{
						x_counter++;
					}
				}




				//pick a pointer at random from the remainder    
				if (x_counter == 0)
				{
					x_temp_value = 0;

					this->g_logmove("Ignored too many...reverting."); //must choose a random from the remaining 'good' pointers 

					for (x_counter_loop = 1; x_counter_loop <= 4; x_counter_loop++)
					
					{
						if (x_piece_array_pointer[x_counter_loop] != 999 && x_piece_array_pointer[x_counter_loop] > 0)
						{
							x_temp_value++;
						}
					}

					this->g_logmove("Player " + to_string(x_player.p_get_playerid()) + " has " + to_string(x_temp_value) + " possible piece(s) to move which are on the board.");


					x_test_1 = 1;

					while (x_test_1)
					{
						x_counter_array = this->g_return_random(4);

						if (x_piece_array_pointer[x_counter_array] > 0 && x_piece_array_pointer[x_counter_array] != 999)
						{
							x_test_1 = false; //found one to move
						}

					}

				}

				else

				{  //pick one from the backup array to use
					x_temp_value = 0;

					for (x_counter_loop = 1; x_counter_loop <= 4; x_counter_loop++)

					{
						if (x_piece_array_backup [x_counter_loop] != 999 && x_piece_array_backup[x_counter_loop] > 0)
						{
							x_temp_value++;
						}
					}

					this->g_logmove("Choosing one from the remaining markers..."); //must choose a random from the remaining 'good' pointers 

					this->g_logmove("Player " + to_string(x_player.p_get_playerid()) + " has " + to_string(x_temp_value) + " possible piece(s) to move which are on the board.");

					x_test_1 = 1;

					while (x_test_1)
					{
						x_counter_array = this->g_return_random(4);

						if (x_piece_array_pointer[x_counter_array] > 0 && x_piece_array_pointer[x_counter_array] != 999)
						{
							x_test_1 = false; //found one to move
						}

					}

				}


			
			this->g_logmove("Player " + to_string(x_player.p_get_playerid())  + " has chose to move " + to_string(x_piece_array_pointer[x_counter_array]));

		}


		else

		{

			//there were no markers 'on the board'...return an empty pointer;
			//this is captured by the calling function and acted upon        

			return 0;
		}

		return(x_piece_array_pointer[x_counter_array]);

	} //eofunc




	bool g_target_magic_numbers(int x_playerid, int x_magicnumbers[], std::string x_type, int x_magic_count)
	{

		//properties
		int x_forecast_pointers[5][3];
		bool x_found_target = 0;
		bool x_test_count_flag = 0;
		int x_piece_pointer = 0;
		int x_counter = 0;
		int x_counter_m = 0;
		int x_counter_test = 0;
		int x_temp_forecast = 0;
				
		//populate current players positions and status to temp array
		for (x_counter = 1; x_counter <= 4; x_counter++)
		{
			//HERE
			if (!this->g_players[x_playerid].p_pieces[x_counter].m_get_status())
				
			{
				//this piece is out of play - set up a dummy which will never get hit

				x_forecast_pointers[x_counter][0] = x_counter;

				x_forecast_pointers[x_counter][1] = 999;

				x_forecast_pointers[x_counter][2] = 0;

			}

			else

			{
				//otherwise, put in a forecast of where it would land based on the dice roll

				x_forecast_pointers[x_counter][0] = x_counter;

				x_forecast_pointers[x_counter][1] = this->g_players[x_playerid].p_pieces[x_counter].m_get_location() * this->g_dievalue;

				x_forecast_pointers[x_counter][2] = 0;

			}
		}


		//now for each potential location see if there is a match in magic numbers
		for (x_counter_m = 1; x_counter_m <= 4; x_counter_m++)
		{
			x_temp_forecast = x_forecast_pointers[x_counter_m][1];

			for (x_counter = 1; x_counter <= x_magic_count; x_counter++) //iterate through magic numbers to see if there is a match
			{
								
				if (x_magicnumbers[x_counter] == x_temp_forecast)
				{ //found one
					x_forecast_pointers[x_counter_m][2] = 1;
					x_counter = 999;
				
				}

			}

			//clear the array of player's pieces that are not a likely hit
			x_test_count_flag = 0;

			for (x_counter_test = 1; x_counter_test <= 4; x_counter_test++)
			{
				if (x_forecast_pointers[x_counter_test][2] == 1)
				{
					//found at least one
					x_test_count_flag = 1;

				}

				else

				{
					x_forecast_pointers[x_counter_test][2] = 999;

				}
			}


			//got at least one potential target
			if (x_test_count_flag)
			{

				//now we have an array of only the possible markers to select to target
				//loop until we find one that is not 999

				while (!x_found_target)
				{
					x_counter = this->g_return_random(4);

					if (x_forecast_pointers[x_counter][2] != 999)
					{
						//the piece we choose to move
						x_piece_pointer = x_forecast_pointers[x_counter][0];

						this->g_logmove("Player " + to_string(x_playerid) + " is targeting " + x_type + " number " + to_string(x_forecast_pointers[x_counter][1]) + " with piece " + to_string(x_piece_pointer));

						this->g_marker_move(x_playerid,  x_piece_pointer);
						//here??

						x_found_target = 1;
					

					}
				}
			}

			else

			{
				x_found_target = 0;

			}


		} //END OF SET B

		return x_found_target;
	}


	bool g_target_potential_clashes_set_C(Player x_player) {

		//properties
		int x_forecast_pointers[5][3];
		bool x_found_target = 0;
		bool x_test_count_flag = 0;
		int x_piece_pointer = 0;
		int x_temp_branch = 2;
		int x_counter = 0;
		int x_counter_o = 0;
		int x_counter_p = 0;
		int x_counter_test = 0;
		int x_temp_opp_location = 0;
		Player x_temp_opp = this->g_return_other_player();
		bool x_offboard_flag = 0;
		bool x_onboard_flag = 0;


		//populate current players positions and status to temp array
		for (x_counter = 1; x_counter <= 4; x_counter++)
		{

			if (!x_player.p_pieces[x_counter].m_get_status())
			{
				//this piece is out of play - set up a dummy which will never get hit

				x_forecast_pointers[x_counter][0] = x_counter;

				x_forecast_pointers[x_counter][1] = 999;

				x_forecast_pointers[x_counter][2] = 0;


			}

			else

			{
				//otherwise, put in a forecast of where it would land based on the dice roll

				x_forecast_pointers[x_counter][0] = x_counter;

				x_forecast_pointers[x_counter][1] = x_player.p_pieces[x_counter].m_get_location() * this->g_dievalue;

				x_forecast_pointers[x_counter][2] = 0;

			}
		}


		//now for each potential location see if there is a match in the opponent's pieces

		for (x_counter_o = 1; x_counter_o <= 4; x_counter_o++)
		{

			if (!x_temp_opp.p_pieces[x_counter_o].m_get_status())

			{
				//opp position is out of play and should be ignored -  dummy value
				this->g_logmove("Ignoring target piece " + to_string(x_counter_o) + " as it is out of play.");

				x_temp_opp_location = 888;
			}

			else

			{
				//hold the location of a potential target piece to hit
				x_temp_opp_location = x_temp_opp.p_pieces[x_counter_o].m_get_location();
				
			}

			for (x_counter_p = 1; x_counter_p <= 4; x_counter_p++)
			{
				//check that the locations match and that the opponents piece  is not at the start, or inactive:

				if (x_forecast_pointers[x_counter_p][1] == x_temp_opp_location && x_temp_opp_location != 1 && x_temp_opp_location != 888)

				{
					//we have a potential target
					x_forecast_pointers[x_counter_p][2] = 1;

					this->g_logmove("Player " + to_string(x_temp_opp.p_get_playerid()) + "'s marker " + to_string(x_counter_o) + " at location " + to_string(x_temp_opp.p_pieces[x_counter_o].m_get_location())  + " is a target of piece " + to_string(x_counter_p));

					//don't even know if this works in c++
					break; //we only need to know this once         

				}

			} //move on to next player piece

		} //move on to next opponent piece


		//clear the array of player's pieces that are not a likely hit
		x_test_count_flag = 0;

		for (x_counter_test = 1; x_counter_test <= 4; x_counter_test++)
		{
			if (x_forecast_pointers[x_counter_test][2] == 1)
			{
				//found at least one
				x_test_count_flag = 1;

			}

			else

			{
				x_forecast_pointers[x_counter_test][0] = 999;
			}
		}


		//got at least one potential target
		if (x_test_count_flag)
		{
			x_counter = 0;

			while (!x_found_target)
			{
				x_counter = this->g_return_random(4);

				if (x_forecast_pointers[x_counter][0] != 999)
				{

					x_piece_pointer = x_forecast_pointers[x_counter][0];

					this->g_logmove("Player " + to_string(x_player.p_get_playerid()) + " has targets to consider and chose to move piece " + to_string(x_piece_pointer));

					this->g_marker_move(x_player.p_get_playerid(), x_piece_pointer);

					x_found_target = 1;

					return x_found_target;
				}
			}
		}

		else

		{
			this->g_logmove("Player " + to_string(x_player.p_get_playerid()) + " could not find a clash target so is going to find a pointer at random to move.");

			//toss up between on or off board
			x_offboard_flag = 0;

			x_onboard_flag = 0;

			for (x_counter = 1; x_counter <= 4; x_counter++)
			{
				//loop and see if there is a mix of on-board or off-board marker; do a coin toss if there is
				if (x_player.p_pieces[x_counter].m_get_location() == 1 && x_player.p_pieces[x_counter].m_get_status())
				{
					x_offboard_flag = 1;
				}

				if (x_player.p_pieces[x_counter].m_get_location() > 1 && x_player.p_pieces[x_counter].m_get_status())
				{
					x_onboard_flag = 1;
				}

			}

			if (x_onboard_flag)
			{

				if (x_offboard_flag)
				{

					//choice is a wonderful thing - 1 is onboard, 2 is offboard
					x_temp_branch = this->g_return_random(2);

				}
				else

					x_temp_branch = 1;

			}

			if (x_temp_branch == 1)
			{

				x_piece_pointer = 0;

				this->g_logmove("Finding a piece on the board.");

				x_piece_pointer = this->g_find_to_move_in_play(x_player);

				if (x_piece_pointer != 0)
				{
					//found one... move it        

					this->g_marker_move(x_player.p_get_playerid(), x_piece_pointer);

					x_found_target = 1;

				}

				else

				{

					//didn't find one, have to force to go down the SET D path.

					x_temp_branch = 0;

					x_found_target = 0;

				}
			}

			if (x_temp_branch == 0)
			{

				x_found_target = 0;

				this->g_logmove("Finding a piece OFF the board using SET D.");
			}


		}
		
		return x_found_target;

	} //END OF SET C


	int g_return_random(int g_upper){
	
			
		int g_temp_random = rand() % g_upper + 1;

		return g_temp_random;
	}


};