#ifndef PIECE_HPP
#define PIECE_HPP

#include <string>
#include <vector>

#include "ChessBoard.hpp"

class ChessBoard;

class Piece {
protected:
  bool p_is_white;
  ChessBoard* board;
  string name;
  vector<string> valid_moves_vec;
public:
  Piece(ChessBoard* a_board, bool is_white) {
    p_is_white = is_white;
    board = a_board;
  }
  virtual vector<string> valid_moves(string source_square) = 0;
  virtual bool get_piece_colour() = 0;
  virtual string get_name() = 0;
  bool is_valid_field(string square);
  vector<string> inline_moves(string source_square);
  vector<string> diagonal_moves(string source_square);
  virtual ~Piece(){}
};

 
#endif

