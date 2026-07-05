#include <stdio.h>
#include <string.h>

#include <stdlib.h>
#include <time.h>

#define MAX_WORD_LENGTH 256
#define MAX_HISTORY 100

// 3バイトのかな文字を番号(コードポイント)に変換。
// カタカナならひらがなに寄せて返す（あ と ア を同じ値にする）
int kana_code(const char *p) {
    unsigned char b0 = (unsigned char)p[0];
    unsigned char b1 = (unsigned char)p[1];
    unsigned char b2 = (unsigned char)p[2];
    // 3バイトから元の番号を組み立てる
    int cp = ((b0 & 0x0F) << 12) | ((b1 & 0x3F) << 6) | (b2 & 0x3F);
    // カタカナ(ァ〜ヶ)なら 0x60 引いてひらがなにする
    if (cp >= 0x30A1 && cp <= 0x30F6) cp -= 0x60;
    return cp;
}


// ひらがな，カタカナ以外の入力を弾く
int is_hiragana_katakana(const char *word, size_t len) {
    if (len % 3 != 0) return 0;
    // ↑3バイトずつ区切る
    for (size_t i =0; i < len; i += 3) {
        unsigned char b0 = (unsigned char)word[i];
        unsigned char b1 = (unsigned char)word[i + 1];
        if (b0 != 0xE3) return 0;         // 先頭は必ず 0xE3
        if (b1 < 0x81 || b1 > 0x83) return 0;
    }
    return 1;
}

int main(void) {
    int game_over = 0; // 0：ゲーム続行、1：ゲームオーバー

    // 最初の単語を追加
    const char *initial_word[] = {"しりとり", "しりとりのし", "しりとりのと"};
    srand((unsigned int)time(NULL));
    

    while (game_over == 0) {
    // 単語履歴を保持
    char history[MAX_HISTORY][MAX_WORD_LENGTH];
    int history_count = 0;
    char next_word[MAX_WORD_LENGTH];

    // 最初の単語をランダムに選択
    int idx = rand() % 3;
    strcpy(history[0], initial_word[idx]);
    history_count++;

    printf("しりとりスタート！\n");
    printf("(終了するには Ctrl+D を押してください)\n\n");

    while (1) {
        const char *previous_word = history[history_count - 1];
        printf("前の単語: %s\n", previous_word);
        printf("次の単語を入力してください >");

        if (fgets(next_word, sizeof(next_word), stdin) == NULL) {
            printf("\nしりとりを終了します\n");
            game_over = 1;
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

    // リセットコマンド
    if (strcmp(next_word, "reset") == 0) {
        strcpy(history[0], "しりとり");
        history_count = 1;
        printf("\nリセットしました. 最初からプレイ\n\n");
        continue;
    }

    if (!is_hiragana_katakana(next_word, len)) {
        printf("ひらがな，カタカナを入力してください\n\n");
        continue;
    }
    // 二文字以上の単語を入力してください(バイト数を3→6に変更)
    size_t prev_len = strlen(previous_word);
    if (prev_len <6 || len <6) {
        printf("二文字以上の単語を入力してください\n\n");
        continue;
    }

    // if (strncmp(previous_word + prev_len - 3, next_word, 3) != 0) {
    //     printf("エラー：前の単語に続いていません\n\n");
    //     continue;
    // }

    // バイト比較から番号比較に変更
    if (kana_code(previous_word + prev_len - 3) != kana_code(next_word)) {
        printf("エラー：前の単語に続いていません\n\n");
        continue;
    }

//     // んで終わるかチェック
//     if (kana_code(next_word + len - 3) == 0x3093) {   // 0x3093 = ん
//     printf("\n「%s」は「ん」で終わる単語です，ゲーム終了\n\n", next_word);
//     game_over = 1;
//     break;
// }
    if (kana_code(next_word + len - 3) == 0x3093) {   // 0x3093 = ん
        printf("\n「%s」は「ん」で終わる単語です，ゲーム終了\n\n", next_word);
        game_over = 1;
        break;
    }

    // 重複チェック
    int duplicated = 0;
    for (int i = 0; i <history_count; i++) {
        if (strcmp(history[i], next_word) == 0) {
            duplicated = 1;
            continue;
        }
    }
    if (duplicated) {
        printf("\n「%s」はすでに使用した単語です．ゲーム終了\n\n", next_word);
        game_over = 1;
        break;
    }


    // 単語履歴に追加
    if (history_count >= MAX_HISTORY) {
        printf("\n履歴が最大数に達しました，ゲーム終了\n\n");
        game_over = 1;
        break;
    }
    strcpy(history[history_count], next_word);
    history_count++;
    printf("\n");
    }


    if (game_over == 1) {
        printf("ゲームを続けますか？(y/n)");
        char answer[MAX_WORD_LENGTH];
        if (fgets(answer, sizeof(answer), stdin) == NULL) {
            printf("ゲームを終了します\n");
            break;
        }
        answer[strcspn(answer, "\n")] = '\0';
        if (strcmp(answer, "y") == 0) {
            game_over = 0;
        history_count = 0;
        strcpy(history[0], "しりとり");
        continue;
        }
        if (strcmp(answer, "n") == 0) {
            printf("ゲームを終了します\n");
            break;
        }
    }
    return 0;
    }
}
