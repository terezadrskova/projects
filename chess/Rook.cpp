#include <iostream>
#include <map>
#include <vector>

#include "ChessBoard.hpp"
#include "Piece.hpp"
#include "Rook.hpp"

using namespace std;

// returns vector with possible moves in inline direction from Piece class
vector<string> Rook::valid_moves(string source_square){
  valid_moves_vec = inline_moves(source_square);
  return valid_moves_vec;
}

bool Rook::get_piece_colour(){
  return p_is_white;
}

string Rook::get_name(){
  return name;
}
