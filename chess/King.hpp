#ifndef KING_HPP
#define KING_HPP

#include <string>
#include <vector>

#include "ChessBoard.hpp"
#include "Piece.hpp"

class King : public Piece {
  string name;
public:
  King(ChessBoard* board, bool is_white) : Piece(board,is_white){
    name = "King";
  }
  vector<string> valid_moves(string source_square);
  bool get_piece_colour();
  string get_name();
};

 
#endif

