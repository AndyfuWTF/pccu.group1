#include <iostream>
#include <string>
#include <cctype>
#include "Chess.h"
#include "Judge.h"

using namespace std;

// 將輸入的西洋棋座標（如 e2）轉換為陣列索引 (0~7)
bool parseCoordinates(const string& input, int& r, int& c) {
    if (input.length() != 2) return false;
    c = input[0] - 'a';
    r = 8 - (input[1] - '0');
    return (r >= 0 && r < 8 && c >= 0 && c < 8);
}

int main() {
    Chess game;
    Judge judge;
    int fullMoveCount = 1; // 總回合數

    cout << "======================================\n";
    cout << "                開始棋局               \n";
    cout << "======================================\n";
    cout << "輸入格式範例: 'e2 e4' (移動棋子) 或 'O-O' (短易位), 'O-O-O' (長易位)\n";
    cout << "大寫=白棋  小寫=黑棋              \n";
    cout << "K=王  Q=后  R=車  B=象  N=馬  P=兵        \n";
    cout << "======================================\n";

    // 修改點 1：在遊戲一開始，先檢查一次初始狀態是否結束（防禦性檢查）
    if (judge.checkGameOver(game, fullMoveCount)) return 0;

    while (true) {
        game.displayBoard();
        bool isWhite = game.isWhiteTurn();
        
        // 刪除原本在這邊的 checkGameOver 檢查，改移到玩家成功走棋之後

        cout << "第 " << fullMoveCount << " 回合 | " << (isWhite ? "白棋 (大寫)" : "黑棋 (小寫)") << " 的回合。\n";
        
        // 修改點 2：加強將軍提示，讓玩家明確知道必須解將
        if (judge.isKingInCheck(game, isWhite)) {
            cout << "⚠️  注意：你的王正在被將軍！你必須做出能解將的移動！\n";
        }
        cout << "請輸入指令: ";
        
        string input1, input2;
        cin >> input1;

        bool moved = false; // 用於紀錄本回合是否有成功執行合法的換位或移動

        // 處理王城換位指令
        if (input1 == "O-O" || input1 == "O-O-O") {
            bool kingSide = (input1 == "O-O");
            if (judge.canCastling(game, isWhite, kingSide)) {
                judge.performCastling(game, isWhite, kingSide);
                cout << "玩家進行了 " << (kingSide ? "短易位" : "長易位") << "\n";
                judge.incrementHalfMove();
                moved = true; // 標記成功行動
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

            // 檢查選取的棋子是否屬於當前回合的玩家
            if (movingPiece == '.' || (isWhite && islower(movingPiece)) || (!isWhite && isupper(movingPiece))) {
                cout << "你不能移動空白格或對方的棋子。\n";
                continue;
            }

            // 檢驗移動是否合法
            if (judge.isValidMove(game, r1, c1, r2, c2)) {
                // 50步規則判定：如果是移動兵 (Pawn) 或有吃子，重置計數器；否則累加
                if (tolower(movingPiece) == 'p' || targetPiece != '.') {
                    judge.resetHalfMove();
                } else {
                    judge.incrementHalfMove();
                }

                // 更新王車移動狀態（王城換位記錄）
                if (tolower(movingPiece) == 'k') game.setKingMoved(isWhite);
                if (tolower(movingPiece) == 'r') {
                    if (c1 == 0) game.setRookMoved(isWhite, true);
                    if (c1 == 7) game.setRookMoved(isWhite, false);
                }

                // 執行移動
                game.setPiece(r2, c2, movingPiece);
                game.setPiece(r1, c1, '.');

                cout << "成功將 " << movingPiece << " 從 " << input1 << " 移動到 " << input2 << "\n";
                moved = true; // 標記成功行動
            } else {
                cout << "違規移動！該棋子無法如此移動，或移動會導致己方國王被將軍（或無法解將）。\n";
                continue;
            }
        }

        // 修改點 3：當確認有成功移動時，更新回合、切換棋手，並「立刻」檢查對方是否被將死或逼和
        if (moved) {
            if (!isWhite) {
                fullMoveCount++;
            }
            
            // 換對方的回合
            game.switchTurn();

            // 立刻判定換手後的對方有沒有合法步可走（檢查將死/逼和）
            if (judge.checkGameOver(game, fullMoveCount)) {
                break; // 如果遊戲結束（將死或和棋），直接跳出迴圈
            }
        }
    }

    return 0;
}