#include <iostream>
#include <string>
#include <map>

#include "ChessBoard.hpp"
#include "Piece.hpp"
#include "King.hpp"
#include "Rook.hpp"
#include "Bishop.hpp"
#include "Queen.hpp"
#include "Knight.hpp"
#include "Pawn.hpp"

using namespace std;

// this function is also used for initialisation of the board
void ChessBoard::resetBoard()  {
  cout << "A new chess game is started!" << endl;

  chess_map.clear();
  is_game_over = false;

  // create STL map for all the chess pieces
  chess_map["A1"] = new (nothrow) Rook(this,true);
  chess_map["B1"] = new (nothrow) Knight(this,true);
  chess_map["C1"] = new (nothrow) Bishop(this,true);
  chess_map["D1"] = new (nothrow) Queen(this,true);
  chess_map["E1"] = new (nothrow) King(this,true);
  chess_map["F1"] = new (nothrow) Bishop(this,true);
  chess_map["G1"] = new (nothrow) Knight(this,true);
  chess_map["H1"] = new (nothrow) Rook(this,true);

  chess_map["A2"] = new (nothrow) Pawn(this,true);
  chess_map["B2"] = new (nothrow) Pawn(this,true);
  chess_map["C2"] = new (nothrow) Pawn(this,true);
  chess_map["D2"] = new (nothrow) Pawn(this,true);
  chess_map["E2"] = new (nothrow) Pawn(this,true);
  chess_map["F2"] = new (nothrow) Pawn(this,true);
  chess_map["G2"] = new (nothrow) Pawn(this,true);
  chess_map["H2"] = new (nothrow) Pawn(this,true);

  chess_map["A7"] = new (nothrow) Pawn(this,false);
  chess_map["B7"] = new (nothrow) Pawn(this,false);
  chess_map["C7"] = new (nothrow) Pawn(this,false);
  chess_map["D7"] = new (nothrow) Pawn(this,false);
  chess_map["E7"] = new (nothrow) Pawn(this,false);
  chess_map["F7"] = new (nothrow) Pawn(this,false);
  chess_map["G7"] = new (nothrow) Pawn(this,false);
  chess_map["H7"] = new (nothrow) Pawn(this,false);

  chess_map["A8"] = new (nothrow) Rook(this,false);
  chess_map["B8"] = new (nothrow) Knight(this,false);
  chess_map["C8"] = new (nothrow) Bishop(this,false);
  chess_map["D8"] = new (nothrow) Queen(this,false);
  chess_map["E8"] = new (nothrow) King(this,false);
  chess_map["F8"] = new (nothrow) Bishop(this,false);
  chess_map["G8"] = new (nothrow) Knight(this,false);
  chess_map["H8"] = new (nothrow) Rook(this,false);

}

bool ChessBoard::submitMove(string source_square, string destination_square){
  // if the game is over - this move is not valid
  if(is_game_over){
    cout << "The game is over" << endl;
    return false;
  }
  Piece *tkn_piece_ptr = NULL;
  bool is_piece_tkn = false;
  // validate the input (original square and destination square)
  if (!validate_input(source_square,destination_square)){
    return false;
  }

  // validate the move - this should call the relevant class
  vector<string> this_valid_moves_vec = chess_map[source_square]->valid_moves(source_square);
  
  // is destination in the valid_moves_vec? if yes, move the piece
  for (unsigned int i=0; i<this_valid_moves_vec.size(); i++) {
    if (destination_square==this_valid_moves_vec.at(i)){
      if(chess_map.count(destination_square) == 1){
	// this saves the taken piece - just in case it needs to be returned
	tkn_piece_ptr = chess_map[destination_square];
	is_piece_tkn = true;
      }
      move_piece(source_square,destination_square);
      break;
    } 
    else {
      // check for all the possible moves before throwing an error
      if (i==this_valid_moves_vec.size()-1){
	for (map<string, Piece*>::iterator it=chess_map.begin(); it!=chess_map.end(); ++it){
	  if(it->first==source_square) {
	    if(is_player_white){
	      cout << "White's ";
	    } else {
	      cout << "Black's ";
	    }
	    cout << it->second->get_name() << " cannot move to " 
		 <<  destination_square << "!" << endl;
	  }
	}
	break;
      }
    }
  }
  
  // clear the vector before the next move
  this_valid_moves_vec.clear();

  // return false when not calling from the checkmate
  string white_king_pos = get_white_king_pos();
  string black_king_pos = get_black_king_pos();
  
  // case for a white player
  if(is_player_white){
    // checking if the player put himself/herself in check
    if (king_in_check(is_in_checkmate,white_king_pos,black_king_pos) ==  1){
      cout << "This move puts your king in check!" << endl;
      // reverse the last move
      chess_map[source_square] = chess_map[destination_square];
      chess_map.erase(destination_square);
      // return back the removed piece because the move is invalid
      if (is_piece_tkn){
	chess_map[destination_square] = tkn_piece_ptr;
	is_piece_tkn = false;
      }
      return false;
    }

    if (king_in_checkmate(2, source_square)){
      // games ends in checkmate
      if (king_in_check(is_in_checkmate,white_king_pos,black_king_pos) != 0){
	cout << "Black is in checkmate" << endl;
	is_game_over = true;
      }
      // games ends in stalemate
      else {
	cout << "Game ends in stalemate" << endl;
	is_game_over = true;
      }
    } 
    // player would put itself in check
    if (king_in_checkmate(1, source_square)){
      cout << "This move is invalid" << endl;
    }
  }
  // case for a black player
  else {
    // checking if the player put himself/herself in check
    if (king_in_check(is_in_checkmate,white_king_pos,black_king_pos) ==  2){
      cout << "This move puts your king in check!" << endl;
    }
    //cout << "CHecking for checkmate white player - black in checkmate" << endl;
    if (king_in_checkmate(1,source_square)){
      // games ends in checkmat
      if (king_in_check(is_in_checkmate,white_king_pos,black_king_pos) != 0){
	cout << "White is in checkmate" << endl;
	is_game_over = true;
      }
      // games ends in stalemate
      else {
	cout << "Game ends in stalemate" << endl;
	is_game_over = true;
      }
    } 
    // player would put itself in check
    if (king_in_checkmate(2, source_square)){
      cout << "This move is invalid" << endl;
    }
  }

   // if king is in check - check if it's not a checkmate
  if (king_in_check(is_in_checkmate,white_king_pos,black_king_pos) == 1
      && !(king_in_checkmate(1,source_square))){
    cout << "White in check" << endl;
  }
  else if (king_in_check(is_in_checkmate,white_king_pos,black_king_pos) == 2
	   &&
	   !(king_in_checkmate(2,source_square))){
    cout << "Black in check" << endl;
  }
 
 
  // switches the colour between white and black for each turn
  is_player_white = (!is_player_white);
  
  return true;
}

// function to validate if the source_square and destination_square input
bool ChessBoard::validate_input(string source_square, string destination_square){ 
  // checks if the input has exactly two charracters
  if (source_square.size()!=2 ||destination_square.size()!=2){
    cout << "The position is not on the map!" << endl;
    return false;
  }
 
   // is the first character a capital letter from A to H?
  if ((source_square[0] < 'A' || source_square[0] > 'H')
      ||
      (destination_square[0] < 'A' || destination_square[0] > 'H')){
    cout << "The position is not on the map!" << endl;
    return false; 
  }

  // is the second character a number between 1 and 8?
  if ((source_square[1] < '1' || source_square[1] > '8')     
      ||
      (destination_square[1] < '1' || destination_square[1] > '8')){
    cout << "The position is not on the map!" << endl;
    return false;
  }

  // checks if there is a piece on the source position
  if (!(chess_map.count(source_square))){
    cout << "There is no piece at position "  << source_square << "!" <<  endl;
    return false;
  }

  // checks if the piece belongs to the player that should move the figure
  for (map<string, Piece*>::iterator it=chess_map.begin(); it!=chess_map.end(); ++it){
    if(it->first==source_square) {
      if(is_player_colour_white() && (!it->second->get_piece_colour())) {
	 cout << "It's not White's turn to move!" << endl;
	 return false;
      }
      else if ((!is_player_colour_white()) && it->second->get_piece_colour()) {
	 cout << "It's not Black's turn to move!" << endl;
	 return false;
      }
    }
  }
  
  return true;
}

// moving piece to a new location
void ChessBoard::move_piece(string source_square,string destination_square){

  // getting the correct name for the source figure
  if(is_player_white){
    cout << "White's ";
  } else {
    cout << "Black's ";
  }
  // move piece from source to destination square
  for (map<string, Piece*>::iterator it=chess_map.begin(); it!=chess_map.end(); ++it){
    if(it->first==source_square) {
      cout << it->second->get_name() << " moves from " << source_square << " to " 
	   <<  destination_square;
    }
  }

  // if there is a piece in the destination square - remove it
  // valid_moves - already checked if it's opponent's piece
  for (map<string, Piece*>::iterator it=chess_map.begin(); it!=chess_map.end(); ++it){
    if(it->first==destination_square) {
      chess_map.erase(destination_square);
      cout << " taking ";
      if(is_player_white){
	 cout << "Black's ";
      } else {
	 cout << "White's ";
      }
      cout << it->second->get_name(); 
    }
  }
  cout << endl;
 
  // assign the piece from the source square to the new destination
  for (map<string, Piece*>::iterator it=chess_map.begin(); it!=chess_map.end(); ++it){
    if(it->first==source_square) {
      chess_map[destination_square] = it->second;
    }
  }

  // erase the piece from its original destination
  chess_map.erase(source_square);
}



// function to find out wheater king is in check
// go through all the pieces to check if there is at least one
// piece that has a valid move the opposite king's square
int ChessBoard::king_in_check(bool in_checkmate, string white_king_pos, string black_king_pos){
  for (map<string, Piece*>::iterator it2=chess_map.begin(); it2!=chess_map.end(); ++it2){
    // check for white pieces - black  king in check
    if (it2->second->get_piece_colour()){
      vector<string> temp_vector = chess_map[it2->first]->valid_moves(it2->first);
      for (unsigned int i=0; i< temp_vector.size(); i++) {
	if((!in_checkmate) &&temp_vector.at(i) == get_black_king_pos()){
	  return 2;
	}
	// if the position of the black king has changed
	if (in_checkmate && temp_vector.at(i) == black_king_pos){;
	  return 2;
	}
      }
    }
    // check for black pieces - white  king in check
    else {
      vector<string> temp_vector = chess_map[it2->first]->valid_moves(it2->first);
      for (unsigned int i=0; i< temp_vector.size(); i++) {
	//white piece has white king square as his possible valid move
	if(temp_vector.at(i) == get_white_king_pos()){
	  return 1;
	}
	// if the position of the white king has changed
	if (in_checkmate && temp_vector.at(i) == white_king_pos){
	  return 1;
	}
      }
    }
  }
  return 0;
}

/* this function goes through all the pieces and moves them to all
   their possible moves - if there is at least one that doesn't
   end in check - then it's not a checkmate

   this function also works as stalemate - it's a checkmate but
   not in check

   is_in_check = 1 (white king is in check)
   is_in_check = 2 (black king is in check)
*/
bool ChessBoard::king_in_checkmate(int is_in_check, string source_square){
  // ---------- go through all pieces with the same colour and move them - after each 
  // check if king is still in check
  // black king in check
  string white_king_pos = "";
  string black_king_pos = "";
  bool is_in_checkmate = true;
  string this_piece;
  vector<string> possible_moves;
  Piece* this_piece_ptr  = 0;

  vector<string> white_pieces;
  vector<string> black_pieces;

  Piece *tkn_piece_ptr = NULL;
  bool is_piece_tkn = false;
  bool is_check = true;
  for (map<string, Piece*>::iterator it=chess_map.begin(); it!=chess_map.end(); ++it){
    if (it->second!=NULL && !(it->second->get_piece_colour()))
      black_pieces.push_back(it->first);
  }
  for (map<string, Piece*>::iterator it=chess_map.begin(); it!=chess_map.end(); ++it){
    if (it->second!=NULL && it->second->get_piece_colour())
      white_pieces.push_back(it->first);
  }

  // black king in checkmate/stalemate
  if (is_in_check == 2){
    for (vector<string>::iterator it=black_pieces.begin(); it!=black_pieces.end(); ++it){
      // going through all black pieces and moving them to all possible
      // valid moves they have in vector
      this_piece = *it;
      this_piece_ptr = chess_map[this_piece];
      	
      // check white piece's vector 
      possible_moves.clear();
      possible_moves = chess_map[this_piece]->valid_moves(this_piece);

      for (unsigned int i=0; i<possible_moves.size(); i++) {
	string p_move = possible_moves[i];
	// moving the piece to its possible move
	if (chess_map.count(p_move) == 0){
	  chess_map[p_move] = chess_map[this_piece];
	  chess_map.erase(this_piece);
	}
	else{
	  is_piece_tkn = true;
	  tkn_piece_ptr = chess_map[p_move];
	  chess_map[p_move] = chess_map[this_piece];
	  chess_map.erase(this_piece);
	}
	// if the king has been moved - change his location for the 
	// king in check function
	if (this_piece_ptr->get_name() == "King") {
	  black_king_pos = possible_moves.at(i);
	} else {
	  black_king_pos = get_black_king_pos();
	}
	// check if it's still check
	if (king_in_check(is_in_checkmate,white_king_pos,black_king_pos)==0){
	  is_check = false;
	} else{
	  is_check = true;
	}
	
	// unmoves the possible move of the piece
	chess_map[this_piece] = chess_map[p_move];
	chess_map.erase(p_move);
	if (is_piece_tkn){
	  // returns its piece
	  chess_map[p_move] = tkn_piece_ptr;
	  is_piece_tkn = false;
	}
	if (!is_check)
	  return false;
      }
    } 
  }


  // white king in checkmate/stalemate
  if (is_in_check == 1){
    for (vector<string>::iterator it=white_pieces.begin(); it!=white_pieces.end(); ++it){
      // going through all black pieces and moving them to all possible
      // valid moves they have in vector
      this_piece = *it;
      this_piece_ptr = chess_map[this_piece];
      	
      // check white piece's vector  
      possible_moves.clear();
      possible_moves = chess_map[this_piece]->valid_moves(this_piece);

      for (unsigned int i=0; i<possible_moves.size(); i++) {
	// moving the piece to its possible move
	string p_move = possible_moves[i];
	if (chess_map.count(p_move) == 0){
	  chess_map[p_move] = chess_map[this_piece];
	  chess_map.erase(this_piece);
	}
	else{
	  is_piece_tkn = true;
	  tkn_piece_ptr = chess_map[p_move];
	  chess_map[p_move] = chess_map[this_piece];
	  chess_map.erase(this_piece);
	}
	
        // if the king has been moved - change his location for the 
	// king in check function
	if (this_piece_ptr->get_name() == "King") {
	  white_king_pos = possible_moves.at(i);
	} else {
	  white_king_pos = get_white_king_pos();
	}
	// check if the king is still check
	if (king_in_check(is_in_checkmate,white_king_pos,black_king_pos)==0){
	  is_check = false;
	} else{
	  is_check = true;
	}
	
	// unmove the possible move
	chess_map[this_piece] = chess_map[p_move];
	chess_map.erase(p_move);
	if (is_piece_tkn){
	  // return the piece if taken away
	  chess_map[p_move] = tkn_piece_ptr;
	  is_piece_tkn = false;
	}
	if (!is_check)
	  return false;
      }
    }
  }
 
  return true;
}

// return the current position of the white king
string ChessBoard::get_white_king_pos(){
  string white_king_pos;
  for (map<string, Piece*>::iterator it=chess_map.begin(); it!=chess_map.end(); ++it){
    if(it->second->get_name()=="King"){
      if (it->second->get_piece_colour()){
	white_king_pos = it->first;
	return white_king_pos;
      }
    }
  }
  return "";
}

// return the current position of the black king
string ChessBoard::get_black_king_pos(){
  string black_king_pos;
  for (map<string, Piece*>::iterator it=chess_map.begin(); it!=chess_map.end(); ++it){
    if(it->second->get_name()=="King"){
      if (!(it->second->get_piece_colour())){
	black_king_pos = it->first;
	return black_king_pos;
      }
    }
  }
  return "";
}

// return the colour of the current player
// note: not trying to be racist here =)
bool ChessBoard::is_player_colour_white(){
  return is_player_white;
}


// return map for the sub-pieces
map<string, Piece*> ChessBoard::initMap(){
  return chess_map;
}

// destructor for the ChessBoard class - deleting all the pieces
ChessBoard::~ChessBoard()  {
  for (map<string, Piece*>::iterator it=chess_map.begin(); it!=chess_map.end(); ++it){
    delete it->second;
  }
}

// helper function to see all the pieces position
void ChessBoard::print_chess_map(){
  for (map<string, Piece*>::iterator it = chess_map.begin(); it != chess_map.end(); ++it)
    cout << it->first << " - " << it->second << " - COLOUR == " << it->second->get_piece_colour() << endl;
}
