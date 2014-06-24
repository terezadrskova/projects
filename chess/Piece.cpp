#include <iostream>
#include <map>
#include <vector>

#include "ChessBoard.hpp"
#include "Piece.hpp"


using namespace std;

// returns true if the field is valid
bool Piece::is_valid_field(string square){
  if (square[0] >= 'A' && square[0] <= 'H'){
    if (square[1] >= '1' && square[1] <= '8'){
      return true;
    }
  }
  return false;
}

// returns vector of possible moves for Rook and Queen
vector<string> Piece::inline_moves(string source_square){
  map<string, Piece*> new_chess_map = board->initMap();
  valid_moves_vec.clear();
  
  // -------------- VALID INLINE MOVES FOR THE ROOK/QUEEN ----------------

 // ------------going vertically up--------------------------
 for(char i = source_square[1]+1; i <= '8'; i++){
   // check if there is any piece on the way
   string temp_square = string(1,source_square[0]) + i;
   if (new_chess_map.count(temp_square)) {
     // is piece is it the same colour as the player?
     for (map<string, Piece*>::iterator it=new_chess_map.begin(); it!=new_chess_map.end(); ++it){
       if(it->first==temp_square) {
	 // check for the possible destination with a figure on it
	 if(it->second->get_piece_colour() != get_piece_colour()) {
	   valid_moves_vec.push_back (temp_square);  
	  }
	}
     }
     // end to loop if there is a piece on the way
     break;
   }
   else {
     // no piece on the way - add it to the vector
     valid_moves_vec.push_back (temp_square);
   }
 }

//----------------- going vertically down---------------------
 for(char i = source_square[1]-1; i >= '1'; i--){
   // check if there is any piece on the way
   string temp_square = string(1,source_square[0]) + i;
   if (new_chess_map.count(temp_square)) {
     // is piece is it the same colour as the player?
     for (map<string, Piece*>::iterator it=new_chess_map.begin(); it!=new_chess_map.end(); ++it){
       if(it->first==temp_square) {
	 // check for the possible destination with a figure on it
	 if(it->second->get_piece_colour() != get_piece_colour()) {
	   // the player's colour and piece's colour must not the same
	   valid_moves_vec.push_back (temp_square);    
	  }
	}
     }
     // end to loop if there is a piece on the way
      break;
   }
   else {
     // no piece on the way - add it to the vector
     valid_moves_vec.push_back (temp_square);
   }
 }

// ---------------going horrizontally right--------------------
 for(char i = source_square[0]+1; i <= 'H'; i++){
   // check if there is any piece on the way
   string temp_square = i + string(1,source_square[1]);
   if (new_chess_map.count(temp_square)) {
     // is piece is it the same colour as the player?
     for (map<string, Piece*>::iterator it=new_chess_map.begin(); it!=new_chess_map.end(); ++it){
       if(it->first==temp_square) {
	 // check for the possible destination with a figure on it
	 if(it->second->get_piece_colour() != get_piece_colour()) {
	   // the player's colour and piece's colour must not the same
	   valid_moves_vec.push_back (temp_square);  
	  }
	}
     }
     // end to loop if there is a piece on the way
     break;
   }
   else {
     // no piece on the way - add it to the vector
     valid_moves_vec.push_back (temp_square);
   }
 }

 //------------------- going horizontally left------------------
 for(char i = source_square[0]-1; i >= 'A'; i--){
   // check if there is any piece on the way
   string temp_square = i + string(1,source_square[1]);
   if (new_chess_map.count(temp_square)) {
     // is piece is it the same colour as the player?
     for (map<string, Piece*>::iterator it=new_chess_map.begin(); it!=new_chess_map.end(); ++it){
       if(it->first==temp_square) {
	 // check for the possible destination with a figure on it
	 if(it->second->get_piece_colour() != get_piece_colour()) {
	   // the player's colour and piece's colour must not the same
	   valid_moves_vec.push_back (temp_square);    
	  }
	}
     }
     // end to loop if there is a piece on the way
     break;
   }
   else {
     // no piece on the way - add it to the vector
     valid_moves_vec.push_back (temp_square);
   }
 }

  return valid_moves_vec;
}

// returns vector of possible moves for Bishop and Queen
vector<string> Piece::diagonal_moves(string source_square){
  map<string, Piece*> new_chess_map = board->initMap();
  valid_moves_vec.clear();

  // -------------- VALID MOVES FOR THE BISHOP ----------------

 // ------------going diagonally up right --------------------------
 char x = source_square[0]+1;
 for(char i = source_square[1]+1; i <= '8'; i++,x++){
   if(x>='A' && x<='H'){
     // check if there is any piece on the way
     string temp_square = string(1,x) + string(1,i);

     if (new_chess_map.count(temp_square)) {
       // is piece is it the same colour as the player?
       for (map<string, Piece*>::iterator it=new_chess_map.begin(); it!=new_chess_map.end(); ++it){
	 if(it->first==temp_square) {
	   // check for the possible destination with a figure on it
	   if(it->second->get_piece_colour() != get_piece_colour()) {
	     // the player's colour and piece's colour must not the same
	     // in order to be a valid move
	     valid_moves_vec.push_back (temp_square);  
	   }
	 }
       }
       // end to loop if there is a piece on the way
       break;
     }
     else {
       // no piece on the way - add it to the vector
       valid_moves_vec.push_back (temp_square);
     }
   }
 }

 //----------------- going diagonlly down right ---------------------
 x = source_square[0]+1;
 for(char i = source_square[1]-1; i >= '1'; i--,x++){
   if(x>='A' && x<='H'){
     // check if there is any piece on the way
     string temp_square = string(1,x) + string(1,i);

     if (new_chess_map.count(temp_square)) {
       // is piece is it the same colour as the player?
       for (map<string, Piece*>::iterator it=new_chess_map.begin(); it!=new_chess_map.end(); ++it){
	 if(it->first==temp_square) {
	   // check for the possible destination with a figure on it
	   if(it->second->get_piece_colour() != get_piece_colour()) {
	     // the player's colour and piece's colour must not the same
	     // in order to be a valid move
	     valid_moves_vec.push_back (temp_square);  
	   }
	 }
       }
       // end to loop if there is a piece on the way
       break;
     }
     else {
       // no piece on the way - add it to the vector
       valid_moves_vec.push_back (temp_square);
     }
   }
 }

 // ---------------going diagonally top left --------------------
 char i = source_square[1]+1;
 for(x = source_square[0]-1; x >= 'A'; i++, x--){
   if(i>='1' && i<='8'){
     // check if there is any piece on the way
     string temp_square = string(1,x) + string(1,i);

     if (new_chess_map.count(temp_square)) {
       // is piece is it the same colour as the player?
       for (map<string, Piece*>::iterator it=new_chess_map.begin(); it!=new_chess_map.end(); ++it){
	 if(it->first==temp_square) {
	   // check for the possible destination with a figure on it
	   if(it->second->get_piece_colour() != get_piece_colour()) {
	     // the player's colour and piece's colour must not the same
	     // in order to be a valid move
	     valid_moves_vec.push_back (temp_square);  
	   }
	 }
       }
       // end to loop if there is a piece on the way
       break;
     }
     else {
       // no piece on the way - add it to the vector
       valid_moves_vec.push_back (temp_square);
     }
   }
 }

 //------------------- going diagonally bottom left------------------
 i = source_square[1]-1;
 for(x = source_square[0]-1; x >= 'A'; i--, x--){
   if(i>='1' && i<='8'){
     // check if there is any piece on the way
     string temp_square = string(1,x) + string(1,i);

     if (new_chess_map.count(temp_square)) {
       // is piece is it the same colour as the player?
       for (map<string, Piece*>::iterator it=new_chess_map.begin(); it!=new_chess_map.end(); ++it){
	 if(it->first==temp_square) {
	   // check for the possible destination with a figure on it
	   if(it->second->get_piece_colour() != get_piece_colour()) {
	     // the player's colour and piece's colour must not the same
	     // in order to be a valid move
	     valid_moves_vec.push_back (temp_square);  
	   }
	 }
       }
       // end to loop if there is a piece on the way
       break;
     }
     else {
       // no piece on the way - add it to the vector
       valid_moves_vec.push_back (temp_square);
     }
   }
 }

 return valid_moves_vec;
}
