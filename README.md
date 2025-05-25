# FM-index

> **FM-index** (Full-text Minute index)：一種基於 **BWT** (Burrows-Wheeler Transform) 的資料結構，用來在大型文本中快速查找子字串

## 簡介

- 特性
    - 定義
        - $n$：文本長度
        - $m$：查詢字串長度
        - $k$：匹配次數
    - 建構時間：$O(n)$
    - 查詢次數 `count`：$O(m)$
    - 查詢位置 `locate`：$O(m + k)$
- 本次實作流程：
    - 使用 SA-IS 演算法建立後綴陣列 (SA)
    - 根據 SA 建立 BWT
    - 建構 C 表與 Occ 表
- 額外功能
    - SA 採樣與快速 locate
- 未來規劃
    - 雙向查詢功能

## 編譯與執行

```bash
mkdir build
cd build
cmake ..
cmake --build .
./fm_index
```