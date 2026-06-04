#include "Chess.h"
#include <iostream>
#include <cctype>

using namespace std;

Chess::Chess() {
    // 設定棋盤(白為大寫，黑為小寫)和白棋先走
    board = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };
    whiteTurn = true;
    whiteKingMoved = whiteLeftRookMoved = whiteRightRookMoved = false;
    blackKingMoved = blackLeftRookMoved = blackRightRookMoved = false;
}

void Chess::displayBoard() const {
    cout << "\n  a b c d e f g h\n";
    cout << "  ----------------\n";
    for (int i = 0; i < 8; ++i) {
        cout << 8 - i << "|";
        for (int j = 0; j < 8; ++j) {
            cout << board[i][j] << " ";
        }
        cout << "|" << 8 - i << "\n";
    }
    cout << "  ----------------\n";
    cout << "  a b c d e f g h\n\n";
}

char Chess::getPiece(int r, int c) const {
    if (r >= 0 && r < 8 && c >= 0 && c < 8) return board[r][c];
    return ' ';
}

void Chess::setPiece(int r, int c, char piece) {
    if (r >= 0 && r < 8 && c >= 0 && c < 8) board[r][c] = piece;
}

bool Chess::isWhiteTurn() const { return whiteTurn; }
void Chess::switchTurn() { whiteTurn = !whiteTurn; }


//檢測王城換位條件是否成立(如果王或車移動就就無法換位，且路徑內不可有阻擋)
bool Chess::hasKingMoved(bool isWhite) const { return isWhite ? whiteKingMoved : blackKingMoved; }
bool Chess::hasLeftRookMoved(bool isWhite) const { return isWhite ? whiteLeftRookMoved : blackLeftRookMoved; }
bool Chess::hasRightRookMoved(bool isWhite) const { return isWhite ? whiteRightRookMoved : blackRightRookMoved; }

void Chess::setKingMoved(bool isWhite) {
    if (isWhite) whiteKingMoved = true;
    else blackKingMoved = true;
}

void Chess::setRookMoved(bool isWhite, bool isLeft) {
    if (isWhite) {
        if (isLeft) whiteLeftRookMoved = true;
        else whiteRightRookMoved = true;
    } else {
        if (isLeft) blackLeftRookMoved = true;
        else blackRightRookMoved = true;
    }
}