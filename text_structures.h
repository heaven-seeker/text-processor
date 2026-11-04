#ifndef TEXT_STRUCT_H
#define TEXT_STRUCT_H

struct Sentence {
    wchar_t *str;
    int size;
};

struct Text {
    struct Sentence **sents;
    int size;
    int n;
};

#endif

