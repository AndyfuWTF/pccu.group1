#ifndef JUDGE_H
#define JUDGE_H

#include "Chess.h"

using namespace std;

class Judge {
private:
    int halfMoveClock; // 用於計算50步和棋規則（無兵的移動且無吃子）

    bool isPathClear(const Chess& game, int r1, int c1, int r2, int c2) const;
    bool isSquareAttacked(const Chess& game, int r, int c, bool byWhite) const;

public:
    Judge();
    
    // 基礎移動驗證
    bool isValidMove(Chess& game, int r1, int c1, int r2, int c2, bool simulation = false);
    
    // 特殊規則：王城換位
    bool canCastling(const Chess& game, bool isWhite, bool kingSide);
    void performCastling(Chess& game, bool isWhite, bool kingSide);

    // 遊戲狀態與和棋判斷
    bool isKingInCheck(const Chess& game, bool isWhite) const;
    bool hasLegalMoves(Chess& game, bool isWhite);
    bool checkGameOver(Chess& game, int fullMoveCount);
    
    void incrementHalfMove() { halfMoveClock++; }
    void resetHalfMove() { halfMoveClock = 0; }
    int getHalfMoveClock() const { return halfMoveClock; }
};

#endif