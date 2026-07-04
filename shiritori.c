#include <stdio.h>
#include <string.h>

#define MAX_WORD_LENGTH 256
#define MAX_HISTORY 100

int main(void) {
    
    // 単語履歴を保持
    char history[MAX_HISTORY][MAX_WORD_LENGTH];
    int history_count = 0;
    char next_word[MAX_WORD_LENGTH];

    // 最初の単語を追加
    strcpy(history[0], "しりとり");
    history_count++;

    printf("しりとりスタート！\n");
    printf("(終了するには Ctrl+D を押してください)\n\n");

    while (1) {
        const char *previous_word = history[history_count - 1];
        printf("前の単語: %s\n", previous_word);
        printf("次の単語を入力してください >");

        if (fgets(next_word, sizeof(next_word), stdin) == NULL) {
            printf("\nしりとりを終了します\n");
            break;
    }

    size_t len = strlen(next_word);
    if (len > 0 && next_word[len - 1] == '\n') {
        next_word[len - 1] = '\0';
        len--;
    }

    // 空入力は無視
    if (len == 0) {
        continue;
    }

    size_t prev_len = strlen(previous_word);
    if (prev_len <3 || len <3) {
        printf("ひらがなを入力してください\n\n");
        continue;
    }

    if (strncmp(previous_word + prev_len - 3, next_word, 3) != 0) {
        printf("エラー：前の単語に続いていません\n\n");
        continue;
    }

    // んで終わるかチェック
    if ((unsigned char)next_word[len - 3] == 0xE3 &&
        (unsigned char)next_word[len - 2] == 0x83 &&
        (unsigned char)next_word[len - 1] == 0x93) {
        printf("\n「%s」は「ん」で終わる単語です，ゲーム終了\n\n", next_word);
        break;
    }

    // 重複チェック
    int duplicated = 0;
    for (int i = 0; i <history_count; i++) {
        if (strcmp(history[i], next_word) == 0) {
            duplicated = 1;
            break;
        }
    }
    if (duplicated) {
        printf("\n「%s」はすでに使用した単語です．ゲーム終了\n\n", next_word);
        break;
    }


    // 単語履歴に追加
    if (history_count >= MAX_HISTORY) {
        printf("\n履歴が最大数に達しました，ゲーム終了\n\n");
        break;
    }
    strcpy(history[history_count], next_word);
    history_count++;
    printf("\n");
    }

    return 0;
}