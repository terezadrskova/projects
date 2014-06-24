chess: ChessMain.o ChessBoard.o Piece.o King.o Rook.o Bishop.o Queen.o Knight.o Pawn.o
	g++ -g -Wall ChessMain.o ChessBoard.o Piece.o King.o Rook.o Bishop.o Queen.o Knight.o Pawn.o -o chess
ChessMain.o: ChessMain.cpp
	g++ -g -Wall -c ChessMain.cpp 
ChessBoard.o: ChessBoard.cpp ChessBoard.hpp  
	g++ -g -Wall -c ChessBoard.cpp
Piece.o: Piece.cpp Piece.hpp
	g++ -g -Wall -c Piece.cpp 
King.o: King.cpp King.hpp
	g++ -g -Wall -c King.cpp 
Rook.o: Rook.cpp Rook.hpp
	g++ -g -Wall -c Rook.cpp 
Bishop.o: Bishop.cpp Bishop.hpp 
	g++ -g -Wall -c Bishop.cpp
Queen.o: Queen.cpp Queen.hpp
	g++ -g -Wall -c Queen.cpp 
Knight.o: Knight.cpp Knight.hpp
	g++ -g -Wall -c Knight.cpp 
Pawn.o: Pawn.cpp Pawn.hpp
	g++ -g -Wall -c Pawn.cpp 
clean:
	rm -f chess *.o 
