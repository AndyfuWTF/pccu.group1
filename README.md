# 組別:第一組

# 系級班級:資工1A

# 成員資訊:組長、傅榆翔，組員、林享聖、劉佩翰

# 小專題題目:西洋棋棋盤

# 程式介紹:

## 類別與方法說明:

### Chess 類別:

| 方法 | 功能 |
|--------|--------|
| Chess() | 初始化棋盤 |
| displayBoard() | 顯示棋盤 |
| getPiece() | 取得指定位置棋子 |
| setPiece() | 設定指定位置棋子 |
| isWhiteTurn() | 取得目前回合 |
| switchTurn() | 切換回合 |
| setKingMoved() | 記錄王已移動 |
| setRookMoved() | 記錄車已移動 |
------------------------------

### Judge 類別:

| 方法 | 功能 |
|--------|--------|
| isValidMove() | 驗證移動是否合法 |
| isKingInCheck() | 判斷是否被將軍 |
| hasLegalMoves() | 檢查是否有合法走法 |
| canCastling() | 判斷是否可王城換位 |
| performCastling() | 執行王城換位 |
| checkGameOver() | 判定遊戲是否結束 |
----------------------------------

### main.cpp:

- 接收玩家輸入
- 顯示棋盤
- 輸出規則判定
- 更新棋局狀態
- 判定勝負
- 升變棋子

## 核心架構簡述:

### Chess->管理建立棋盤和 Judge->判斷移動是否合法 main->使用者的操作和更新現在的狀態

# 系統功能說明:

### 使用者在棋盤上需要操控兩方進行遊戲直到遊戲結束，遊戲結束的方式有方式（將殺、和棋）

# 程式安裝方式:

### 下載所有檔案，並且放到支援C++的ide並且放到同一個資料夾，然後執行main.cpp

# 程式使用方式:

### 使用者輸入X和Y軸來選擇棋子，並再以相同方式選擇目的地

# UML圖:
![image](https://github.com/AndyfuWTF/pccu.group1/blob/main/UML.png)

# 流程圖:
![image](https://github.com/AndyfuWTF/pccu.group1/blob/main/流程圖.png)

# 運行畫面截圖:
![image](https://github.com/AndyfuWTF/pccu.group1/blob/main/test1.png)
![image](https://github.com/AndyfuWTF/pccu.group1/blob/main/test2.png)

# 分工資訊

### 傅榆翔:程式製作80%、PPT20%
### 林享聖:程式製作10%、PPT20%    
### 劉佩翰:程式製作10%、PPT60%






