#ifndef TEXT_OPERATIONS_H
#define TEXT_OPERATIONS_H

#include "text_structures.h"
#include "utility.h"


struct Sentence* readSentence();
void removeDuplicateSentences(struct Text *text);
void deleteLeadingSpace(struct Text *t);
void printUniqueWords(struct Text *t);
void sortSentencesByProduct(struct Text *t);
void RemoveSentencesWithSymbolsWithoutDigits(struct Text *t);
void replace_dates(struct Text *t);
void changeSentence(struct Text* t, wchar_t* word);


#endif // TEXT_OPERATIONS_H
