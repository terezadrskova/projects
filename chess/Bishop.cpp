#include <iostream>
#include <map>
#include <vector>

#include "ChessBoard.hpp"
#include "Piece.hpp"
#include "Bishop.hpp"

using namespace std;

// returns vector with possible moves in  diagonal direction from Piece.cpp 
vector<string> Bishop::valid_moves(string source_square){
  valid_moves_vec = diagonal_moves(source_square);
  return valid_moves_vec;
}

bool Bishop::get_piece_colour(){
  return p_is_white;
}

string  Bishop::get_name(){
  return name;
}

