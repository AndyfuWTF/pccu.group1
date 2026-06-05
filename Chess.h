#ifndef CHESS_H
#define CHESS_H

#include <vector>
#include <string>

using namespace std;

class Chess {
private:
    vector<vector<char>> board;
    bool whiteTurn;
    
    bool whiteKingMoved;
    bool whiteLeftRookMoved;
    bool whiteRightRookMoved;
    bool blackKingMoved;
    bool blackLeftRookMoved;
    bool blackRightRookMoved;

public:
    Chess();
    void displayBoard() const;
    char getPiece(int r, int c) const;
    void setPiece(int r, int c, char piece);
    bool isWhiteTurn() const;
    void switchTurn();
    
    bool hasKingMoved(bool isWhite) const;
    bool hasLeftRookMoved(bool isWhite) const;
    bool hasRightRookMoved(bool isWhite) const;
    void setKingMoved(bool isWhite);
    void setRookMoved(bool isWhite, bool isLeft);
};

#endif
