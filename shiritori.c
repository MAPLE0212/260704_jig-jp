#include <stdio.h>
#include <string.h>

#define MAX_WORD_LENGTH 256

int main(void) {
    char input[MAX_WORD_LENGTH];

    printf("単語を入力してください >");
    
    if (fgets(input, sizeof(input), stdin) ==NULL) {
        // "fgetsは標準入力（キーボード）から1行読み込む(Enterの改行含む)”
        return 0;
    }

    // 末尾の改行を削除
    size_t len = strlen(input); // 文字数を取得
    if (len >0 && input[len - 1] == '\n') { // 末尾が改行なら
        input[len - 1] = '\0'; // '\0'（文字列の終わり）に置き換えて消去
    }

    printf("入力された単語: %s\n", input);
    return 0;
}