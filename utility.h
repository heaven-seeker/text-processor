#ifndef UTILITY_H
#define UTILITY_H

#include "text_structures.h"

struct Sentence* readSentence();
void freeText(struct Text t);
int calculateProduct(wchar_t *t);
int compareSentences(const void *a, const void *b);
int containsDigits(wchar_t *sent);
const wchar_t* get_month_number(const wchar_t *month);
void print_help();

#endif
