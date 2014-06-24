#include <iostream>
#include <map>
#include <vector>

#include "ChessBoard.hpp"
#include "Piece.hpp"
#include "King.hpp"

using namespace std;

// returns vector with possible moves for king
vector<string> King::valid_moves(string source_square){
  map<string, Piece*> new_chess_map = board->initMap();
  valid_moves_vec.clear();
  
  // -------------- VALID MOVES FOR THE KING ----------------

 string temp_square[8];
 temp_square[0] = string(1,source_square[0])   + string(1,source_square[1]+1);
 temp_square[1] = string(1,source_square[0]+1) + string(1,source_square[1]+1);
 temp_square[2] = string(1,source_square[0]+1) + string(1,source_square[1]);
 temp_square[3] = string(1,source_square[0]+1) + string(1,source_square[1]-1);
 temp_square[4] = string(1,source_square[0])   + string(1,source_square[1]-1);
 temp_square[5] = string(1,source_square[0]-1) + string(1,source_square[1]-1);
 temp_square[6] = string(1,source_square[0]-1) + string(1,source_square[1]);
 temp_square[7] = string(1,source_square[0]-1) + string(1,source_square[1]+1);

 for (int i = 0; i <= 7; i++){
   if(is_valid_field(temp_square[i])){
     // check if there is any piece on the way
     if (new_chess_map.count(temp_square[i])) {
       // is piece is it the same colour as the player?
       for (map<string, Piece*>::iterator it=new_chess_map.begin(); it!=new_chess_map.end(); ++it){
	 if(it->first==temp_square[i]) {
	   // check for the possible destination with a figure on it
	   if(it->second->get_piece_colour() != get_piece_colour()) {
	     // the player's colour and piece's colour must not the same
	     // in order to be a valid move
	     valid_moves_vec.push_back(temp_square[i]);  
	   }
	 }
       }
     } else {
       valid_moves_vec.push_back(temp_square[i]);
     }
   }
 }

 return valid_moves_vec;
}

bool King::get_piece_colour(){
  return p_is_white;
}

string  King::get_name(){
  return name;
}
