#ifndef IO_OPERATIONS_H
#define IO_OPERATIONS_H

#include "text_structures.h"

void printText(struct Text t);
struct Sentence* readSentence();
struct Text readText();

#endif // IO_OPERATIONS_H
