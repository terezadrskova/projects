#ifndef QUEEN_HPP
#define QUEEN_HPP

#include <string>
#include <vector>

#include "ChessBoard.hpp"
#include "Piece.hpp"

class Queen : public Piece {
  string name;
public:
  Queen(ChessBoard* board,bool is_white) : Piece(board,is_white) {
    name = "Queen";
  }
  vector<string> valid_moves(string source_square);
  bool get_piece_colour();
  string get_name();
};
 
#endif

