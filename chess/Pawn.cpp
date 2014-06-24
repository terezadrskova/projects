#include <iostream>
#include <map>
#include <vector>

#include "ChessBoard.hpp"
#include "Piece.hpp"
#include "Pawn.hpp"

using namespace std;

// returns vector with possible moves for Pawn
vector<string>  Pawn::valid_moves(string source_square){ 
  valid_moves_vec.clear();
  map<string, Piece*> new_chess_map = board->initMap();

  //!!! might not need the destination square
  
  // --------- valid moves for white figures ---------
  if (p_is_white){
    if (get_piece_colour()){	
	//--- GENERATE VECTOR WITH ALL POSSIBLE MOVES
	
	// first move of the pawn, can move by one or two places
	if (source_square[1]=='2'){
	  string temp_square_0 = string(1,source_square[0]) + "3";
	  string temp_square_1 = string(1,source_square[0]) + "4";

	 
	  // check for if there is a figure in that place
	  // no piece at the destination
	  if (new_chess_map.count(temp_square_1)==0 && new_chess_map.count(temp_square_0)==0) {
	    valid_moves_vec.push_back (temp_square_1);
	  }
	} // end of Pawn's first move (can move by two)
	  
	// moving one step forward
	string temp_square_2 = string(1,source_square[0])+string(1,source_square[1]+1);
	if (is_valid_field(temp_square_2) && (!new_chess_map.count(temp_square_2))){
	    valid_moves_vec.push_back (temp_square_2);
	}
	//taking sideways on the left
	string temp_square_3 = string(1,source_square[0]-1)+string(1,source_square[1]+1);
	if (is_valid_field(temp_square_3) && new_chess_map.count(temp_square_3)){
	  for (map<string, Piece*>::iterator it=new_chess_map.begin();
	    it!=new_chess_map.end(); ++it){
	    if(it->first==temp_square_3) {
	      // check for the possible destination with a figure on it
	      if(it->second->get_piece_colour() != get_piece_colour()) {
		// the player's colour and piece's colour must not the same
		// in order to be a valid move
		valid_moves_vec.push_back (temp_square_3);    
	      }
	    }
	  }
	}
	//taking sideways on the right
	string temp_square_4 = string(1,source_square[0]+1)+string(1,source_square[1]+1);
	if (is_valid_field(temp_square_4) && new_chess_map.count(temp_square_4)){
	  for (map<string, Piece*>::iterator it=new_chess_map.begin();
	    it!=new_chess_map.end(); ++it){
	    if(it->first==temp_square_4) {
	      // check for the possible destination with a figure on it
	      if(it->second->get_piece_colour() != get_piece_colour()) {
		// the player's colour and piece's colour must not the same
		// in order to be a valid move
		valid_moves_vec.push_back (temp_square_4);    
	      }
	    }
	  }
	}
      }
  }

  // -------- valid moves for black figures --------
  else {
    if (!(get_piece_colour())){ {
      //--- GENERATE VECTOR WITH ALL POSSIBLE MOVES
      // first move of the pawn, can move by one or two places
	if (source_square[1]=='7'){
	  string temp_square_0 = string(1,source_square[0]) + "6";
	  string temp_square_1 = string(1,source_square[0]) + "5";
	 
	  // check for if there is a figure in that place
	  // no piece at the destination
	  if (new_chess_map.count(temp_square_1)==0 && new_chess_map.count(temp_square_0)==0) {
	    valid_moves_vec.push_back (temp_square_1);
	  }
	} // end of Pawn's first move (can move by two)

	
	// moving one step forward
	string temp_square_2 = string(1,source_square[0])+string(1,source_square[1]-1);
	if (is_valid_field(temp_square_2) && (!new_chess_map.count(temp_square_2))){
	    valid_moves_vec.push_back (temp_square_2);
	}
	//taking sideways on the left
	string temp_square_3 = string(1,source_square[0]-1)+string(1,source_square[1]-1);
	if (is_valid_field(temp_square_3) && new_chess_map.count(temp_square_3)){
	  for (map<string, Piece*>::iterator it=new_chess_map.begin();
	    it!=new_chess_map.end(); ++it){
	    if(it->first==temp_square_3) {
	      // check for the possible destination with a figure on it
	      if(it->second->get_piece_colour() != get_piece_colour()) {
		// the player's colour and piece's colour must not the same
		// in order to be a valid move
		valid_moves_vec.push_back (temp_square_3);    
	      }
	    }
	  }
	}
	//taking sideways on the right
	string temp_square_4 = string(1,source_square[0]+1)+string(1,source_square[1]-1);
	if (is_valid_field(temp_square_4) && new_chess_map.count(temp_square_4)){
	  for (map<string, Piece*>::iterator it=new_chess_map.begin();
	    it!=new_chess_map.end(); ++it){
	    if(it->first==temp_square_4) {
	      // check for the possible destination with a figure on it
	      if(it->second->get_piece_colour() != get_piece_colour()) {
		// the player's colour and piece's colour must not the same
		// in order to be a valid move
		valid_moves_vec.push_back (temp_square_4);    
	      }
	    }
	  }
	}
      }
    } // end pawn's colour is white	
  } // end black's player turn

  return valid_moves_vec;
}

bool Pawn::get_piece_colour(){
  return p_is_white;
}

string Pawn::get_name(){
  return name;
}
