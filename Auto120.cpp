// Auto120.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Game.h>
#include <time.h>
#include <thread>
#include <random>
#include <chrono>




int main()
{
    int x_counter = 0;
    int x_clockstop = 0;
    int x_playerid = 0;
    int x_loop_player = 0;
    int x_loop_piece = 0;

    bool x_gameover = false;
    
    Game* thisGame = new Game;

      for (x_counter = 0; x_counter <= 10000; ++x_counter)
      {
         thisGame->g_logmove("------------------------------------------------------------" + to_string(x_counter));
         
         thisGame->g_logmove("Game Move: " + to_string(x_counter));
                          
          x_playerid = thisGame->g_get_playerturn();
   
          x_gameover = thisGame->g_player_action(x_playerid);

          if (x_gameover ) {

              x_clockstop = x_counter;
              x_counter = 9999999;

          }

//
         // for (x_loop_player = 1; x_loop_player <= 2; x_loop_player++)
       //   {
        //      for (x_loop_piece = 1; x_loop_piece <= 4; x_loop_piece++)
         //    {
           //       cout << "Player " + to_string(x_loop_player) +  " Piece " + to_string(x_loop_piece) + " is at location: ";
             //     cout << to_string(thisGame->g_players[x_loop_player].p_pieces[x_loop_piece].m_get_location()) << "\n";

;
            //  }
      //    }

         // cout << "\n";

          thisGame->g_flip_player();

          if (x_counter % 250 == 0)
          {
              srand((unsigned int)time(NULL));
          }
          


      }

      //delete[] thisGame;
      thisGame->g_displaylog();

      cout << "\nFINAL MOVE: " << to_string(x_clockstop) + "\n";

   }


