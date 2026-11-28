# 第5章：カーネルの双方向連結リスト（`list_head`）の操作確認

`struct list_head` と関連マクロ（`INIT_LIST_HEAD`、`list_add`、`list_del`、`list_entry`、`list_for_each`）を使い、要素の追加・途中削除・解放時のポインタ変化（`next` / `prev`）を `dmesg` で確認した記録。

## ディレクトリ構成

```text
ex1_linked_list/
├── main.c     # 連結リスト操作のテスト用カーネルモジュール
├── Makefile   # KDIR に WSL2-Linux-Kernel を指定してビルドする Makefile
└── README.md
```

## 手順

### 1. `Makefile` の設定（`KDIR` 指定）

WSL2 環境では `/lib/modules/$(uname -r)/build` が存在しない場合があるため、`KDIR` にローカルのカーネルソースディレクトリを指定する。また、ソースファイル名に合わせて `obj-m` を指定する。

```makefile
obj-m := main.o

KDIR ?= /home/higor/kernel-src/WSL2-Linux-Kernel

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
```

### 2. ビルド・ロード・ログ確認・アンロード・クリーン

```bash
cd ~/linux-dd-study/05/ex1_linked_list
make
sudo insmod main.ko
sudo dmesg | tail -n 35
sudo rmmod main
make clean
```

## 実行結果の要点

- 初期状態（`no=0`）から `add_struct(1)`、`add_struct(2)`、`add_struct(3)` を呼ぶと、先頭（`&head.list` 直後）へ順に挿入される。
- `remove_struct(p)` で `no=2` を削除すると、リストから `no=2` が外れ、`no=3` と `no=1` が直接つながる。
- `free_struct()` で残りの要素（`no=3`、`no=1`）を解放し、`sample driver installed.` が出力される。
- モジュール解除時に `sample driver removed.` が出力され、`make clean` でビルド生成物を削除する。

## ハマりどころと修正メモ

- **`/lib/modules/.../build` が見つからないエラー**：`KDIR` に `/home/higor/kernel-src/WSL2-Linux-Kernel` を指定して解決。
- **ターゲットが見つからないエラー**：ソースファイル名が `main.c` だったため、Makefile の `obj-m` を `main.o` に合わせて解決。
- **文法ミス**：`show_struct()` の閉じ括弧 `}` 抜け、`&head->list`（→ `&head.list`）、`remove_struct()` のセミコロン `;` 抜けを修正（修正後はファイルを保存してから `make` すること）。
- **`kmalloc` / `kfree` の暗黙の関数宣言エラー**：`#include <linux/slab.h>` を追加してビルドを通過。
- **ポインタ表記の違い**：WSL2（Linux 6.6系・x86_64）では `printk("%p", ...)` の出力形式やアドレス値が書籍（32bit・2.6系）と異なる。

## 補足（発展メモ）

- 要素を走査しながら解放する処理では、安全性の観点から `list_for_each_safe` と `list_del()` の併用も検討できる（ループ中の解放によるポインタ参照不整合を防ぐため）。