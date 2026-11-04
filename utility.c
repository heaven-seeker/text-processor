#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <wctype.h>
#include "text_structures.h"

void freeText(struct Text t) {
    for(int i = 0; i < t.n; i++) {
        free(t.sents[i]->str);
        free(t.sents[i]);
    }
    free(t.sents);
}

int calculateProduct(wchar_t* t) {
    wchar_t* sent = wcsdup(t);
    int lensProduct = 1;
    wchar_t *token;
    wchar_t *state;

    sent[wcslen(sent) - 1] = L'\0';

    token = wcstok(sent, L" ", &state);
    while (token != NULL) {
        size_t tokenLen = wcslen(token);
        if (tokenLen > 0 && token[tokenLen - 1] == L',') {
            token[tokenLen - 1] = L'\0';
        }

        lensProduct *= wcslen(token);

        token = wcstok(NULL, L" ", &state);
    }

    free(sent);
    
    return lensProduct;
}

int compareSentences(const void* a, const void* b) {
    struct Sentence* sentenceA = *(struct Sentence**)a;
    struct Sentence* sentenceB = *(struct Sentence**)b;

    int productA = calculateProduct(sentenceA->str);
    int productB = calculateProduct(sentenceB->str);

    return productA - productB;
}

int containsDigits(const wchar_t* sent) {
    for (int i = 0; i < wcslen(sent); i++) {
        if (iswdigit(sent[i])) {
            return 1;
        }
    }
    return 0;
}

void print_help() {
    wprintf(L"0: Print the processed text.\n");
    wprintf(L"1: Print words that appear at most once.\n");
    wprintf(L"2: Replace dates in the format '<day> <month> <year> г.' with 'DD/MM/YYYY'.\n");
    wprintf(L"3: Sort sentences by the product of word lengths.\n");
    wprintf(L"4: Remove sentences containing '#' or '№', but no digits.\n");
    wprintf(L"5: Display this help message.\n");
}
