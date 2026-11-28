# Linux Device Driver Study (`linux-dd-study`)

書籍 *Linux Device Driver Programming*（平田豊 著）をベースに、現代の Linux カーネル（6.x 系）環境でのビルド・動作確認・差分やハマりどころを記録する学習リポジトリです。

## 学習環境

- **ホスト / OS**：Windows 11（WSL2 Ubuntu 24.04 / Linux 6.6.x 系）
- **カーネルソース**：`~/kernel-src/WSL2-Linux-Kernel`（本学習リポジトリとは分離して管理）
- **主な確認コマンド**：`make`、`insmod`、`rmmod`、`modprobe`、`lsmod`、`dmesg`、`nm` / `readelf`

## ディレクトリ構成

章・テーマごとにディレクトリを分け、各フォルダにソースコード・`Makefile`・確認手順やメモをまとめた `README.md` を配置しています。

```text
linux-dd-study/
├── README.md
├── 04/
│   ├── ex1_hello/          # 基本的なカーネルモジュール
│   ├── ex2_multi/          # 複数ファイル構成のモジュール
│   ├── ex3_kconfig_char/   # Kconfig / Makefile 連携とパッチ
│   └── ex4_dependencies/   # 外部シンボル参照とモジュール依存関係（mii）
└── 05/
    └── ex1_linked_list/    # カーネル双方向連結リスト（list_head）の操作確認
```

## 進め方の基本ルール

1. **ビルド生成物はコミットしない**：`*.ko`、`*.o`、`.cmd`、`Module.symvers`、`modules.order` などは `make clean` で削除してからコミットする（`.gitignore` の活用も推奨）。
2. **カーネル 2.6 系前提コードとの差分を残す**：ヘッダファイルの追加（例：`#include <linux/slab.h>`）、API・マクロや書式指定子（`%p` / `%px`）の違い、構文・型チェックの厳格化などを各章の `README.md` にメモする。
3. **検証後は必ずアンロード・後片付けをする**：`sudo rmmod <モジュール名>` と `make clean` で環境をクリーンに保つ。
