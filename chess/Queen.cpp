#include <iostream>
#include <map>
#include <vector>

#include "ChessBoard.hpp"
#include "Piece.hpp"
#include "Queen.hpp"

using namespace std;

// returns vector with possible moves in  diagonal and inline direction
// from Piece class 
vector<string> Queen::valid_moves(string source_square){
  vector<string> valid_moves_inline = inline_moves(source_square);
  vector<string> valid_moves_diagonal = diagonal_moves(source_square);

  // preallocate memory
  valid_moves_vec.reserve(valid_moves_inline.size()
			  + valid_moves_diagonal.size() );
  valid_moves_vec.insert(valid_moves_vec.end(),
			 valid_moves_inline.begin(), valid_moves_inline.end() );
  valid_moves_vec.insert(valid_moves_vec.end(), valid_moves_diagonal.begin(),
			 valid_moves_diagonal.end() );


  return valid_moves_vec;
}

bool Queen::get_piece_colour(){
  return p_is_white;
}

string Queen::get_name(){
  return name;
}
