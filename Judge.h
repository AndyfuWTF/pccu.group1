#ifndef JUDGE_H
#define JUDGE_H

#include "Chess.h"

using namespace std;

class Judge {
private:
    int halfMoveClock;

    bool isPathClear(const Chess& game, int r1, int c1, int r2, int c2) const;
    bool isSquareAttacked(const Chess& game, int r, int c, bool byWhite) const;

public:
    Judge();
    
    bool isValidMove(Chess& game, int r1, int c1, int r2, int c2, bool simulation = false);
    
    bool canCastling(const Chess& game, bool isWhite, bool kingSide);
    void performCastling(Chess& game, bool isWhite, bool kingSide);

    bool isKingInCheck(const Chess& game, bool isWhite) const;
    bool hasLegalMoves(Chess& game, bool isWhite);
    bool checkGameOver(Chess& game, int fullMoveCount);
    
    void incrementHalfMove() { halfMoveClock++; }
    void resetHalfMove() { halfMoveClock = 0; }
    int getHalfMoveClock() const { return halfMoveClock; }
};

#endif
