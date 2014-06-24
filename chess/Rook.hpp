#ifndef ROOK_HPP
#define ROOK_HPP

#include <string>
#include <vector>

#include "ChessBoard.hpp"
#include "Piece.hpp"


class Rook : public Piece {
private:
  string name;
public:
  Rook(ChessBoard* board, bool is_white) : Piece(board,is_white) {
    name = "Rook";
  }
  vector<string> valid_moves(string source_square);
  bool get_piece_colour();
  string get_name();
};


 
#endif

