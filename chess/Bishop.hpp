#ifndef BISHOP_HPP
#define BISHOP_HPP

#include <string>
#include <vector>

#include "ChessBoard.hpp"
#include "Piece.hpp"

class Bishop : public Piece {
  string name;
public:
  Bishop(ChessBoard* board, bool is_white) : Piece(board,is_white){
    name = "Bishop";
  }
  vector<string> valid_moves(string source_square);
  bool get_piece_colour();
  string get_name();
};

 
#endif

