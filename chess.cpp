#include <iostream>
using namespace std;

//印出目前的棋盤狀態
void printBoard(char board[8][8]) {
    cout << "\n   0 1 2 3 4 5 6 7  (X軸/直行)\n";
    cout << "  -----------------\n";
    for (int i = 0; i < 8; ++i) {
        cout << i << " |";
        for (int j = 0; j < 8; ++j) {
            cout << board[i][j] << " ";
        }
        cout << "| " << i << "\n";
    }
    cout << "  -----------------\n";
    cout << "   0 1 2 3 4 5 6 7 \n";
    cout << "      (Y軸/橫列)\n\n";
}

int main() {
    //初始化棋盤，大寫為白子，小寫為黑子
    char board[8][8] = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}, 
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'}, 
        {'.', '.', '.', '.', '.', '.', '.', '.'}, 
        {'.', '.', '.', '.', '.', '.', '.', '.'}, 
        {'.', '.', '.', '.', '.', '.', '.', '.'}, 
        {'.', '.', '.', '.', '.', '.', '.', '.'}, 
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'}, 
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}  
    };

    int startX, startY, endX, endY;

    cout << "西洋棋盤:\n";
    cout << "說明：依序輸入 X (0-7) 與 Y (0-7) 座標來選擇與移動棋子。\n";

    //遊戲
    while (true) {
        printBoard(board);

        //選擇要移動的棋子
        cout << "請輸入要移動的棋子座標 (X Y)，中間以空白隔開 (輸入 -1 結束程式): ";
        cin >> startX;
        if (startX == -1) {
            cout << "遊戲結束！\n";
            break;
        }
        cin >> startY;

        //檢查輸入座標是否在範圍內
        if (startX < 0 || startX > 7 || startY < 0 || startY > 7) {
            cout << "座標超出範圍請重新輸入！\n";
            continue;
        }

        // 檢查該位置是否有棋子
        if (board[startY][startX] == '.') {
            cout << "座標 (" << startX << ", " << startY << ") 上沒有棋子！\n";
            continue;
        }

        cout << "你選擇了棋子: '" << board[startY][startX] << "'\n";

        //選擇目標位置
        cout << "請輸入目標座標 (X Y)，中間以空白隔開: ";
        cin >> endX >> endY;

        //檢查目標座標是否在範圍內
        if (endX < 0 || endX > 7 || endY < 0 || endY > 7) {
            cout << "目標座標超出範圍 (0-7)，請重新輸入！\n";
            continue;
        }

        //執行移動
        board[endY][endX] = board[startY][startX]; //將棋子放到新位置
        board[startY][startX] = '.';             

        cout << "移動完成！\n";
    }

    return 0;
}