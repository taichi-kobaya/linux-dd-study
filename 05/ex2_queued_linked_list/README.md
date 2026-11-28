# 第5章：キュー型双方向連結リスト（`list_add_tail`）の動作確認

`ex1_stacked_linked_list`（旧 `ex1_linked_list`）の `list_add()` を `list_add_tail()` に変更し、要素の追加順（`1` → `2` → `3`）がそのまま維持されるキュー（FIFO）的な並びになることを `insmod` / `dmesg` で確認した記録。

## ディレクトリ構成

```text
ex2_queued_linked_list/
├── main.c     # add_struct() で list_add_tail() を使う連結リストサンプル
├── Makefile   # KDIR に WSL2-Linux-Kernel を指定してビルドする Makefile
└── README.md
```

## `ex1_stacked_linked_list` との主な変更点

- `add_struct()` 内の挿入マクロを `list_add(&ptr->list, &head.list)` から `list_add_tail(&ptr->list, &head.list)` に変更。
- ディレクトリ名を `05/ex1_linked_list` から `05/ex1_stacked_linked_list` に変更（先頭挿入＝スタック的な並びと対比しやすくするため）。

## 手順

```bash
cd ~/linux-dd-study/05/ex2_queued_linked_list
make
sudo insmod main.ko
sudo dmesg | tail -n 35
sudo rmmod main
make clean
```

## 実行結果の比較（`list_add` vs `list_add_tail`）

要素 `no=1` → `no=2` → `no=3` を順に追加したときの並びと解放順の違い：

- **`list_add()`（`ex1_stacked_linked_list`・先頭追加）**：`no=3` → `no=2` → `no=1` の順に並び、`no=2` 削除後は `Free: no=3` → `Free: no=1` の順で解放される。
- **`list_add_tail()`（`ex2_queued_linked_list`・末尾追加）**：`no=1` → `no=2` → `no=3` の順（追加順）に並び、`no=2` 削除後は `Free: no=1` → `Free: no=3` の順で解放される。

※ `kmalloc()` の確保アドレスや `printk("%p", ...)` の出力値は実行ごと・環境ごとに変化する。