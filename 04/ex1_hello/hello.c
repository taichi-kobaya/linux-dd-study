// カーネルコードは標準ライブラリに依存できないため、
// カーネル自身が提供するAPIやマクロ定義を使う
#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("Dual BSD/GPL"); // ライセンス宣言

// モジュールがカーネルに呼ばれた時に最初に一回だけ呼ばれる関数
初期化が成功したら0、失敗したら負のエラーコード（-ENOMEM, -ENODEVなど）を返す
static int hello_init(void) 
{
        printk(KERN_ALERT "driver loaded\n"); // KERN_ALERTはログレベル（重大度）を指定するプレフィックスマクロ
        return 0;
}

// rmmodでモジュールがカーネルからアンロードされる時に呼ばれる後片づけ用の関数
static void hello_exit(void)
{
        printk(KERN_ALERT "driver unloaded\n");
}

// エントリーポイントの登録
module_init(hello_init);
module_exit(hello_exit);