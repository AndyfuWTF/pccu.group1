#include "Judge.h"
#include <iostream>
#include <cmath>
#include <cctype>

using namespace std;

Judge::Judge() : halfMoveClock(0) {}

bool Judge::isPathClear(const Chess& game, int r1, int c1, int r2, int c2) const {
    int dr = (r2 == r1) ? 0 : (r2 > r1 ? 1 : -1);
    int dc = (c2 == c1) ? 0 : (c2 > c1 ? 1 : -1);
    int r = r1 + dr;
    int c = c1 + dc;
    while (r != r2 || c != c2) {
        if (game.getPiece(r, c) != '.') return false;
        r += dr;
        c += dc;
    }
    return true;
}

bool Judge::isValidMove(Chess& game, int r1, int c1, int r2, int c2, bool simulation) {
    char piece = game.getPiece(r1, c1);
    char target = game.getPiece(r2, c2);
    
    if (piece == '.') return false;
    
    if (target != '.') {
        if (isupper(piece) && isupper(target)) return false;
        if (islower(piece) && islower(target)) return false;
    }

    int dr = abs(r2 - r1);
    int dc = abs(c2 - c1);
    bool validPieceMove = false;

    switch (tolower(piece)) {
        case 'p': 
            if (isupper(piece)) {
                if (c1 == c2 && target == '.' && r1 - r2 == 1) validPieceMove = true;
                if (c1 == c2 && target == '.' && r1 == 6 && r1 - r2 == 2 && game.getPiece(5, c1) == '.') validPieceMove = true;
                if (dr == 1 && dc == 1 && target != '.' && islower(target) && r1 > r2) validPieceMove = true;
            } else {
                if (c1 == c2 && target == '.' && r2 - r1 == 1) validPieceMove = true;
                if (c1 == c2 && target == '.' && r1 == 1 && r2 - r1 == 2 && game.getPiece(2, c1) == '.') validPieceMove = true;
                if (dr == 1 && dc == 1 && target != '.' && isupper(target) && r2 > r1) validPieceMove = true;
            }
            break;
        case 'r': 
            if (r1 == r2 || c1 == c2) validPieceMove = isPathClear(game, r1, c1, r2, c2);
            break;
        case 'n': 
            if ((dr == 2 && dc == 1) || (dr == 1 && dc == 2)) validPieceMove = true;
            break;
        case 'b': 
            if (dr == dc) validPieceMove = isPathClear(game, r1, c1, r2, c2);
            break;
        case 'q': 
            if (r1 == r2 || c1 == c2 || dr == dc) validPieceMove = isPathClear(game, r1, c1, r2, c2);
            break;
        case 'k': 
            if (dr <= 1 && dc <= 1) validPieceMove = true;
            break;
    }

    if (!validPieceMove) return false;

    if (simulation) return true;

    Chess tempGame = game;
    tempGame.setPiece(r2, c2, piece);
    tempGame.setPiece(r1, c1, '.');
    if (isKingInCheck(tempGame, isupper(piece))) {
        return false; 
    }

    return true;
}

bool Judge::isSquareAttacked(const Chess& game, int r, int c, bool byWhite) const {
    Chess temp = game;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            char p = temp.getPiece(i, j);
            if (p != '.' && ((byWhite && isupper(p)) || (!byWhite && islower(p)))) {
                Judge jg;
                if (jg.isValidMove(temp, i, j, r, c, true)) return true;
            }
        }
    }
    return false;
}

bool Judge::isKingInCheck(const Chess& game, bool isWhite) const {
    char targetKing = isWhite ? 'K' : 'k';
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (game.getPiece(r, c) == targetKing) {
                return isSquareAttacked(game, r, c, !isWhite);
            }
        }
    }
    return false;
}

bool Judge::canCastling(const Chess& game, bool isWhite, bool kingSide) {
    if (game.hasKingMoved(isWhite)) return false;
    if (isKingInCheck(game, isWhite)) return false; 

    int row = isWhite ? 7 : 0;
    if (kingSide) {
        if (game.hasRightRookMoved(isWhite)) return false;
        if (game.getPiece(row, 5) != '.' || game.getPiece(row, 6) != '.') return false;
        if (isSquareAttacked(game, row, 5, !isWhite) || isSquareAttacked(game, row, 6, !isWhite)) return false;
    } else {
        if (game.hasLeftRookMoved(isWhite)) return false;
        if (game.getPiece(row, 1) != '.' || game.getPiece(row, 2) != '.' || game.getPiece(row, 3) != '.') return false;
        if (isSquareAttacked(game, row, 2, !isWhite) || isSquareAttacked(game, row, 3, !isWhite)) return false;
    }
    return true;
}

void Judge::performCastling(Chess& game, bool isWhite, bool kingSide) {
    int row = isWhite ? 7 : 0;
    if (kingSide) {
        game.setPiece(row, 6, isWhite ? 'K' : 'k');
        game.setPiece(row, 5, isWhite ? 'R' : 'r');
        game.setPiece(row, 4, '.');
        game.setPiece(row, 7, '.');
    } else {
        game.setPiece(row, 2, isWhite ? 'K' : 'k');
        game.setPiece(row, 3, isWhite ? 'R' : 'r');
        game.setPiece(row, 4, '.');
        game.setPiece(row, 0, '.');
    }
    game.setKingMoved(isWhite);
}

bool Judge::hasLegalMoves(Chess& game, bool isWhite) {
    for (int r1 = 0; r1 < 8; ++r1) {
        for (int c1 = 0; c1 < 8; ++c1) {
            char p = game.getPiece(r1, c1);
            if (p != '.' && ((isWhite && isupper(p)) || (!isWhite && islower(p)))) {
                for (int r2 = 0; r2 < 8; ++r2) {
                    for (int c2 = 0; c2 < 8; ++c2) {
                        if (isValidMove(game, r1, c1, r2, c2, false)) return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Judge::checkGameOver(Chess& game, int fullMoveCount) {
    bool whiteTurn = game.isWhiteTurn();
    
    if (halfMoveClock >= 100) {
        cout << "和棋！\n";
        return true;
    }

    if (!hasLegalMoves(game, whiteTurn)) {
        if (isKingInCheck(game, whiteTurn)) {
            cout << "將死！" << (whiteTurn ? "黑方" : "白方") << " 獲勝！\n";
        } else {
            cout << "和棋！\n";
        }
        return true;
    }
    return false;
}
