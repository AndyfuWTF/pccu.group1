#include "Judge.h"
#include <iostream>
#include <cmath>
#include <cctype>

using namespace std;

Judge::Judge() : halfMoveClock(0) {}

// 檢查路徑內是否有阻擋（直線與對角線）
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

// 驗證常規移動是否合法
bool Judge::isValidMove(Chess& game, int r1, int c1, int r2, int c2, bool simulation) {
    char piece = game.getPiece(r1, c1);
    char target = game.getPiece(r2, c2);
    
    if (piece == '.') return false;
    
    // 不能吃自己的棋子
    if (target != '.') {
        if (game.isWhiteTurn() && isupper(target)) return false;
        if (!game.isWhiteTurn() && islower(target)) return false;
    }

    int dr = abs(r2 - r1);
    int dc = abs(c2 - c1);
    bool validPieceMove = false;

    switch (tolower(piece)) {
        case 'p': // 兵 (向前一格移動，斜一格吃子，第一次移動可以向前兩格)
            if (game.isWhiteTurn()) {
                if (c1 == c2 && target == '.' && r1 - r2 == 1) validPieceMove = true;
                if (c1 == c2 && target == '.' && r1 == 6 && r1 - r2 == 2 && game.getPiece(5, c1) == '.') validPieceMove = true;
                if (dr == 1 && dc == 1 && target != '.' && islower(target) && r1 > r2) validPieceMove = true;
            } else {
                if (c1 == c2 && target == '.' && r2 - r1 == 1) validPieceMove = true;
                if (c1 == c2 && target == '.' && r1 == 1 && r2 - r1 == 2 && game.getPiece(2, c1) == '.') validPieceMove = true;
                if (dr == 1 && dc == 1 && target != '.' && isupper(target) && r2 > r1) validPieceMove = true;
            }
            break;
        case 'r': // 車 (直線移動和吃子)
            if (r1 == r2 || c1 == c2) validPieceMove = isPathClear(game, r1, c1, r2, c2);
            break;
        case 'n': // 馬 (L型移動和吃子)
            if ((dr == 2 && dc == 1) || (dr == 1 && dc == 2)) validPieceMove = true;
            break;
        case 'b': // 象 (對角線移動和吃子)
            if (dr == dc) validPieceMove = isPathClear(game, r1, c1, r2, c2);
            break;
        case 'q': // 后 (直、斜移動和吃子)
            if (r1 == r2 || c1 == c2 || dr == dc) validPieceMove = isPathClear(game, r1, c1, r2, c2);
            break;
        case 'k': // 王 (口字移動和吃子)
            if (dr <= 1 && dc <= 1) validPieceMove = true;
            break;
    }

    if (!validPieceMove) return false;

    // 模擬移動，確保移動後自己的王不會被將軍
    if (!simulation) {
        Chess tempGame = game;
        tempGame.setPiece(r2, c2, piece);
        tempGame.setPiece(r1, c1, '.');
        if (isKingInCheck(tempGame, game.isWhiteTurn())) {
            return false; // 不能主動將死自己的王
        }
    }

    return true;
}

// 判斷某個格子是否正受到對方攻擊
bool Judge::isSquareAttacked(const Chess& game, int r, int c, bool byWhite) const {
    // 這裡藉由建立一個虛擬棋子來檢測敵方是否能走到此格子
    Chess temp = game;
    if (byWhite) temp.switchTurn(); // 調整回合以符合測試
    
    // 簡化檢測：遍歷棋盤，看敵方是否有棋子能合法走到 (r, c)
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            char p = temp.getPiece(i, j);
            if (p != '.' && ((byWhite && isupper(p)) || (!byWhite && islower(p)))) {
                // 暫時用一個假想的敵方棋子在該格，看能不能吃掉它
                Judge jg;
                if (jg.isValidMove(temp, i, j, r, c, true)) return true;
            }
        }
    }
    return false;
}

// 判斷王是否處於被將軍狀態
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


// 判斷王城換位是否合法
bool Judge::canCastling(const Chess& game, bool isWhite, bool kingSide) {
    if (game.hasKingMoved(isWhite)) return false;
    if (isKingInCheck(game, isWhite)) return false; // 被將軍時不能換位

    int row = isWhite ? 7 : 0;
    if (kingSide) { // 短易位
        if (game.hasRightRookMoved(isWhite)) return false;
        if (game.getPiece(row, 5) != '.' || game.getPiece(row, 6) != '.') return false;
        if (isSquareAttacked(game, row, 5, !isWhite) || isSquareAttacked(game, row, 6, !isWhite)) return false;
    } else { // 長易位
        if (game.hasLeftRookMoved(isWhite)) return false;
        if (game.getPiece(row, 1) != '.' || game.getPiece(row, 2) != '.' || game.getPiece(row, 3) != '.') return false;
        if (isSquareAttacked(game, row, 2, !isWhite) || isSquareAttacked(game, row, 3, !isWhite)) return false;
    }
    return true;
}

void Judge::performCastling(Chess& game, bool isWhite, bool kingSide) {
    int row = isWhite ? 7 : 0;
    if (kingSide) { // 短易位
        game.setPiece(row, 6, isWhite ? 'K' : 'k');
        game.setPiece(row, 5, isWhite ? 'R' : 'r');
        game.setPiece(row, 4, '.');
        game.setPiece(row, 7, '.');
    } else { // 長易位
        game.setPiece(row, 2, isWhite ? 'K' : 'k');
        game.setPiece(row, 3, isWhite ? 'R' : 'r');
        game.setPiece(row, 4, '.');
        game.setPiece(row, 0, '.');
    }
    game.setKingMoved(isWhite);
}

// 檢查目前玩家是否還有任何合法的移動
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

// 判定遊戲是否結束 (將死、逼和、50步和棋)
bool Judge::checkGameOver(Chess& game, int fullMoveCount) {
    bool whiteTurn = game.isWhiteTurn();
    
    // 50 步和棋規則 (當半回合時鐘達到 100，即雙方各50步沒吃子且沒動兵)
    if (halfMoveClock >= 100) {
        cout << "【棋局結束】和棋。\n";
        return true;
    }

    // 無合法移動時（分為將死或逼和）
    if (!hasLegalMoves(game, whiteTurn)) {
        if (isKingInCheck(game, whiteTurn)) {
            cout << (whiteTurn ? "黑方" : "白方") << " 獲勝！\n";
        } else {
            cout << "【棋局結束】逼和，遊戲以和局收場。\n";
        }
        return true;
    }
    return false;
}