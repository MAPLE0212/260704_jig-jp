#include <stdio.h>
#include <string.h>

#define MAX_WORD_LENGTH 256

int main(void) {
    
    char previous_word[MAX_WORD_LENGTH] = "しりとり";
    char next_word[MAX_WORD_LENGTH];

    printf("しりとりスタート！\n");
    printf("(終了するには Ctrl+D を押してください)\n\n");

    while (1) {
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

    if (strncmp(previous_word + prev_len - 3, next_word, 3) == 0) {
        // 末尾と先頭一致→previous_wordを更新
        strcpy(previous_word, next_word);
        printf("\n");
    } else {
        printf("エラー：前の単語に続いていません\n\n");
    }
    }

    return 0;
}