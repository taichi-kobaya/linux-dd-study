# 第4章：カーネルツリーへの組み込み（`drivers/char/hello`・`Kconfig` 連携）

複数ソースファイル（`main.c` + `sub.c`）で構成した `hello` ドライバを、Linux カーネルツリー（`drivers/char/`）に組み込み、`make menuconfig` から選択してビルド・ロード・アンロードを確認した記録。

## ディレクトリ構成

```text
ex3_kconfig_char/
├── hello/
│   ├── Makefile       # hello-objs := main.o sub.o で複合モジュール化
│   ├── main.c         # module_init / module_exit と sub() 呼び出し
│   └── sub.c          # printk で呼び出し元関数名を出力
├── kconfig_char.patch # drivers/char/Kconfig と drivers/char/Makefile の差分
└── README.md
```

## カーネル側への配置・変更内容

1. `~/kernel-src/WSL2-Linux-Kernel/drivers/char/hello/` に `Makefile`、`main.c`、`sub.c` を配置
2. `drivers/char/Kconfig` に `config HELLO`（`tristate "hello driver"`）を追加
3. `drivers/char/Makefile` の末尾に `obj-$(CONFIG_HELLO) += hello/` を追加

## 手順

### 1. コンフィグ設定（`menuconfig`）

カーネルソースのルートディレクトリで実行する（`drivers/char` や `hello` 配下ではなくルートで実行するのがポイント）。

```bash
cd ~/kernel-src/WSL2-Linux-Kernel
make menuconfig
```

- `Device Drivers` → `Character devices` → `hello driver` で `<M>` を選択し、Save して終了。

### 2. モジュールビルド

```bash
cd ~/kernel-src/WSL2-Linux-Kernel
make -j$(nproc) M=drivers/char/hello modules
```

### 3. 動作確認（`insmod` / `dmesg` / `lsmod` / `rmmod`）

```bash
sudo insmod drivers/char/hello/hello.ko
dmesg | tail -n 10
lsmod | grep hello
sudo rmmod hello
dmesg | tail -n 10
```

## ハマりどころと解決メモ

- **`make menuconfig` の実行場所**：サブディレクトリではなくカーネルルート（`~/kernel-src/WSL2-Linux-Kernel`）で実行する。
- **`ncurses` 不足エラー**：`libncurses-dev`、`pkg-config`（必要に応じて `bison`、`flex`）をインストールして解決。
- **`menuconfig` での表示場所**：起動直後のトップメニューではなく、`Device Drivers` → `Character devices` の階層内に表示される。
- **`drivers/char/hello/Makefile` の記述ミス**：複合オブジェクト指定はドット（`hello.objs`）ではなくハイフン（`hello-objs`）にする。
- **`sub.c` の引数区切り**：`printk` のフォーマット文字列と `__func__` の間はピリオド（`.`）ではなくカンマ（`,`）にする。