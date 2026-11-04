#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <wctype.h>
#include "text_structures.h"
#include "text_processing.h"

#define MEM_STEP 5

struct Sentence* readSentence() {
    int size = MEM_STEP;
    wchar_t *buf = malloc(size * sizeof(wchar_t));
    int n = 0;
    wint_t temp;

    if (!buf) {
        wprintf(L"Error: Memory allocation error\n");
        return NULL;
    }

    while (1) {
        if (n >= size - 2) {
            wchar_t *t = realloc(buf, (size + MEM_STEP) * sizeof(wchar_t));
            if (!t) {
                wprintf(L"Error: Memory allocation error\n");
                free(buf); 
                return NULL;
            }
            size += MEM_STEP;
            buf = t;
        }
        temp = fgetwc(stdin);

        /*if (temp == L'\n') {
            if (n == 0) {
                break;
            } else {
                continue;
            }
        }*/
        
        if ((temp == L'\n' || temp == L'.') && n == 0) {
        	free(buf);
        	return NULL;
        }

        buf[n++] = (wchar_t)temp;

        if (temp == L'.' || temp == L'!') {
		if (temp == L'.' && n >= 3 && buf[n - 2] == L'г' && buf[n - 3] == L' ') {
        		wint_t next_char;
        		int has_next_char = 0;
        
        		do {
          			if (n >= size - 2) {
            			wchar_t *t = realloc(buf, (size + MEM_STEP) * sizeof(wchar_t));
            			if (!t) {
              				wprintf(L"Error: Memory allocation error\n");
                            		free(buf);
                            		return NULL;
                        	}
                        		size += MEM_STEP;
                        		buf = t;
          			}
          
          			next_char = fgetwc(stdin);
          
          			buf[n++] = (wchar_t)next_char;
        		} while (iswspace(next_char));
        
        		if (temp != '\n' && next_char != '\n') {
          			has_next_char = 1;
        		}
        
        		if (has_next_char && iswupper(next_char)) {
          			ungetwc(next_char, stdin);
          			n--;
          			buf[n] = L'\0';
          			break;
        		} else {
				continue;
        		}
		} else {
			break;
            	}
        }
    }
    buf[n] = L'\0';

    if (n == 0) {
        free(buf);
        return NULL;
    }

    struct Sentence *sentence = malloc(sizeof(struct Sentence));
    if (!sentence) {
        wprintf(L"Error: Memory allocation error\n");
        free(buf);
        return NULL;
    }
    sentence->str = buf;
    sentence->size = n;

    return sentence;
}

struct Text readText() {
    int size = MEM_STEP;
    struct Sentence **text = malloc(size * sizeof(struct Sentence*));
    int n = 0;
    struct Sentence* temp;
    int nlcount = 0;

    if (!text) {
        wprintf(L"Error: Memory allocation error\n");
        return (struct Text){NULL, 0, 0};
    }

    do {
        temp = readSentence();
        if (temp == NULL) {
            nlcount++;
        } else {
            nlcount = 0;
            if (n >= size) {
                struct Sentence **t = realloc(text, (size + MEM_STEP) * sizeof(struct Sentence*));
                if (!t) {
                    wprintf(L"Error: Memory allocation error\n");

                    for (int i = 0; i < n; i++) {
                        free(text[i]->str);
                        free(text[i]);
                    }
                    free(text);
                    return (struct Text){NULL, 0, 0};
                }
                size += MEM_STEP;
                text = t;
            }
            text[n] = temp;
            n++;
        }
    } while (nlcount < 2);

    struct Text txt;
    txt.size = size;
    txt.sents = text;
    txt.n = n;
    return txt;
}

void printText(struct Text t) {
    for(int i = 0; i < t.n; i++) {
        struct Sentence *cur = t.sents[i];
        wprintf(L"%ls\n", cur->str);
    }
}
