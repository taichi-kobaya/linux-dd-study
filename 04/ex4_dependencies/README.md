# 第4章：モジュール間依存関係と外部シンボルの確認（`mii_link_ok`）

`#include <linux/mii.h>` と `mii_link_ok(NULL)` を参照する `hello.c` をビルドし、外部シンボルの扱いと、WSL2 カーネルにおける `mii` モジュールの確認・ロード手順をまとめた記録。

## ディレクトリ構成

```text
ex4_dependencies/
├── hello.c     # mii_link_ok(NULL) を参照するテスト用モジュールソース
├── Makefile    # カーネルツリー外ビルド用 Makefile
└── README.md
```

## 手順と確認内容

### 1. モジュールのビルドと未定義シンボルの確認

`hello.c` で `mii_link_ok()` を参照してビルドすると、このモジュール単体では実体を持たないため、シンボルテーブル上では未定義（`U` / `UND`）として扱われる。

```bash
cd ~/linux-dd-study/04/ex4_dependencies
make
nm -u hello.ko | grep mii_link_ok || readelf -s hello.ko | grep mii_link_ok
```

### 2. `mii` モジュールの存在確認とロード

WSL2 環境では起動直後に `mii` モジュールがロードされていない場合があるため、カーネルモジュール配下から `mii.ko` を探して `modprobe` でロードする。

```bash
# 現在のロード状態を確認（未ロード時は何も表示されない）
lsmod | grep mii

# mii.ko の場所を確認
find /lib/modules/$(uname -r) -name "*mii*" 2>/dev/null

# mii モジュールをロードして状態を確認
sudo modprobe mii
lsmod | grep mii
```

出力例：

```text
mii                    16384  0
```

## メモ

- 書籍（Linux 2.6系ベース）では `pcnet32` 等に依存して `mii` がロード済みの例が出ることがあるが、WSL2（Linux 6.6系）では環境が異なるため、必要に応じて `find` と `sudo modprobe mii` でモジュールを確認・ロードする。
- 後片付けとして `mii` をアンロードしたい場合は `sudo modprobe -r mii`（依存元がない場合）、ビルド生成物を消す場合は `make clean` を使う。