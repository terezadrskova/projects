#ifndef PAWN_HPP
#define PAWN_HPP

#include <string>
#include <vector>

#include "ChessBoard.hpp"
#include "Piece.hpp"

class Pawn : public Piece {
  string name;
public:
  Pawn(ChessBoard* board, bool is_white) : Piece(board,is_white) {
    name = "Pawn";
  }  
  vector<string> valid_moves(string source_square);
  bool get_piece_colour();
  string get_name();
};

 
#endif

