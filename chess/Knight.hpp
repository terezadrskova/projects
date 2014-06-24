#ifndef KNIGHT_HPP
#define KNIGHT_HPP

#include <string>
#include <vector>

#include "ChessBoard.hpp"
#include "Piece.hpp"

class Knight : public Piece {
private:
  string name;
public:
  Knight(ChessBoard* board,bool is_white) : Piece(board,is_white) {
    name = "Knight";
  }
  vector<string> valid_moves(string source_square);
  bool get_piece_colour();
  string get_name();
};

#endif

