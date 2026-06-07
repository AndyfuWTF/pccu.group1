#include <iostream>
#include <string>
#include <cctype>
#include "Chess.h"
#include "Judge.h"

using namespace std;

bool parseCoordinates(const string& input, int& r, int& c) {
    if (input.length() != 2) return false;
    c = input[0] - 'a';
    r = 8 - (input[1] - '0');
    return (r >= 0 && r < 8 && c >= 0 && c < 8);
}

int main() {
    Chess game;
    Judge judge;
    int fullMoveCount = 1; 

    cout << "======================================\n";
    cout << "                開始棋局               \n";
    cout << "======================================\n";
    cout << "輸入格式範例: 'e2 e4' (移動棋子) 或 'O-O' (短易位), 'O-O-O' (長易位)\n";
    cout << "大寫=白棋  小寫=黑棋              \n";
    cout << "K=王  Q=后  R=車  B=象  N=馬  P=兵        \n";
    cout << "======================================\n";

    if (judge.checkGameOver(game, fullMoveCount)) return 0;

    while (true) {
        game.displayBoard();
        bool isWhite = game.isWhiteTurn();
        

        cout << "第 " << fullMoveCount << " 回合 | " << (isWhite ? "白棋 (大寫)" : "黑棋 (小寫)") << " 的回合。\n";
        
        if (judge.isKingInCheck(game, isWhite)) {
            cout << "你的王正在被將軍！\n";
        }
        cout << "請輸入指令: ";
        
        string input1, input2;
        cin >> input1;

        bool moved = false; 

        if (input1 == "O-O" || input1 == "O-O-O") {
            bool kingSide = (input1 == "O-O");
            if (judge.canCastling(game, isWhite, kingSide)) {
                judge.performCastling(game, isWhite, kingSide);
                cout << "玩家進行了 " << (kingSide ? "短易位" : "長易位") << "\n";
                judge.incrementHalfMove();
                moved = true; 
            } else {
                cout << "不符合王城換位條件。\n";
                continue;
            }
        } else {
            cin >> input2;
            int r1, c1, r2, c2;
            if (!parseCoordinates(input1, r1, c1) || !parseCoordinates(input2, r2, c2)) {
                cout << "輸入格式錯誤。\n";
                continue;
            }

            char movingPiece = game.getPiece(r1, c1);
            char targetPiece = game.getPiece(r2, c2);

            if (movingPiece == '.' || (isWhite && islower(movingPiece)) || (!isWhite && isupper(movingPiece))) {
                cout << "你不能移動空白格或對方的棋子。\n";
                continue;
            }

            if (judge.isValidMove(game, r1, c1, r2, c2)) {
                if (tolower(movingPiece) == 'p' || targetPiece != '.') {
                    judge.resetHalfMove();
                } else {
                    judge.incrementHalfMove();
                }

                if (tolower(movingPiece) == 'k') game.setKingMoved(isWhite);
                if (tolower(movingPiece) == 'r') {
                    if (c1 == 0) game.setRookMoved(isWhite, true);
                    if (c1 == 7) game.setRookMoved(isWhite, false);
                }

                game.setPiece(r2, c2, movingPiece);
                game.setPiece(r1, c1, '.');

                cout << "成功將 " << movingPiece << " 從 " << input1 << " 移動到 " << input2 << " 。" <<"\n";
                moved = true; 
            } else {
                cout << "該棋子無法如此移動。\n";
                continue;
            }
        }

        if (moved) {
            if (!isWhite) {
                fullMoveCount++;
            }
            
            game.switchTurn();

            if (judge.checkGameOver(game, fullMoveCount)) {
                break; 
            }
        }
    }

    return 0;
}
