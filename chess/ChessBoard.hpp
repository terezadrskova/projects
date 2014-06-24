#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP

#include <map>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Piece;

class ChessBoard {
private:
  bool is_player_white;
  bool is_in_checkmate;
  int is_in_check;
  vector<string> valid_moves_vec;
  map<string, Piece*> chess_map;
  map<string, Piece*> new_chess_map;
  bool is_game_over;
public:
  void resetBoard();
  void get_board();
  ChessBoard(){
    is_player_white = true;
    resetBoard();
    is_in_checkmate = false;
    is_game_over = false;
  }
  bool validate_input(string source_square, string destination_square);
  bool submitMove(string source_square,string destination_square);
  void move_piece(string source_square,string destination_square);
  // this is move for checking checkmate - not deleting pieces
  void possible_move(string source_square,string destination_square);
  void possible_unmove(string source_square,string destination_square);
  bool is_player_colour_white();
  map<string, Piece*> initMap();
  int king_in_check(bool in_checkmate, string white_king_pos, string black_king_pos);
  bool king_in_checkmate(int is_in_check, string source_square);
  bool stalemate(int is_in_check);
  string get_white_king_pos();
  string get_black_king_pos();
  vector<string> get_blocking_move(string attacking_piece, string king_pos);
  ~ChessBoard();
  void print_chess_map();
};
#endif
