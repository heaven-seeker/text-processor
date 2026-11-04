#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include "utility.h"

#define MEM_STEP 5
#define MAX_BUFFER 512

void cleanWord(wchar_t *word) {
    int len = wcslen(word);
    if (len > 0 && (word[len - 1] == L'.' || word[len - 1] == L',')) {
        word[len - 1] = L'\0';
    }
}

void deleteLeadingSpace(struct Text *t) {
    for (int i = 0; i < t->n; i++) {
        wchar_t *currentStr = t->sents[i]->str;
        int j = 0;

        while (iswspace(currentStr[j])) {
            j++;
        }

        if (j > 0) {
            size_t newLen = wcslen(currentStr) - j + 1;
            wchar_t *newStr = malloc(newLen * sizeof(wchar_t));

            if (newStr == NULL) {
                wprintf(L"Error: Memory allocation error in function deleteLeadingSpace\n");
                return;  
            }

            wcscpy(newStr, currentStr + j);

            free(t->sents[i]->str);
            t->sents[i]->str = newStr;
        }
    }
}

void removeDuplicateSentences(struct Text *text) {
    for (int i = 0; i < text->n; i++) {
        for (int j = i + 1; j < text->n;) {
            if (wcscmp(text->sents[i]->str, text->sents[j]->str) == 0) {
                free(text->sents[j]->str);
                free(text->sents[j]);
                for (int k = j; k < text->n - 1; k++) {
                    text->sents[k] = text->sents[k + 1];
                }
                text->n--;
            } else {
                j++;
            }
        }
    }
}

void printUniqueWords(struct Text *t) {
    wchar_t **allWords = NULL; 
    int *wordCounts = NULL;
    int totalWords = 0;

    for (int i = 0; i < t->n; i++) {
        wchar_t *sentence = wcsdup(t->sents[i]->str);
        
        if (sentence == NULL) {
            wprintf(L"Error: Memory allocation error in function printUniqueWords\n");
            return;
        }

        wchar_t *state;
        wchar_t *token = wcstok(sentence, L" ", &state);
        
        while (token != NULL) {
            cleanWord(token);       

            int found = 0;
            for (int j = 0; j < totalWords; j++) {
                if (wcscmp(allWords[j], token) == 0) {
                    wordCounts[j]++;
                    found = 1;
                    break;
                }
            }

            if (!found) {
                wchar_t **tempWords = realloc(allWords, (totalWords + 1) * sizeof(wchar_t *));
                int *tempCounts = realloc(wordCounts, (totalWords + 1) * sizeof(int));

                if (tempWords == NULL || tempCounts == NULL) {
                    wprintf(L"Error: Memory allocation error in function printUniqueWords\n");
                    free(allWords);
                    free(wordCounts);
                    free(sentence);
                    return;
                }

                allWords = tempWords;
                wordCounts = tempCounts;
                
                allWords[totalWords] = wcsdup(token);
                wordCounts[totalWords] = 1;
                totalWords++;
            }

            token = wcstok(NULL, L" ", &state);
        }
        free(sentence);
    }

    for (int i = 0; i < totalWords; i++) {
        if (wordCounts[i] == 1) {
            wprintf(L"%ls\n", allWords[i]);
        }
        free(allWords[i]);
    }

    free(allWords);
    free(wordCounts);
}

void sortSentencesByProduct(struct Text *t) {
    qsort(t->sents, t->n, sizeof(struct Sentence*), compareSentences);
}

void RemoveSentencesWithSymbolsWithoutDigits(struct Text *t) {
    for (int i = 0; i < t->n;) {
        if ((wcschr(t->sents[i]->str, L'#') || wcschr(t->sents[i]->str, L'№')) && !containsDigits(t->sents[i]->str)) {
            free(t->sents[i]->str);
            free(t->sents[i]);
            for (int k = i; k < t->n - 1; k++) {
                t->sents[k] = t->sents[k + 1];
            }
            t->n--;
        } else {
            i++;
        }
    }
}

void replace_dates(struct Text *t) {
    wchar_t *months[] = { L"января", L"февраля", L"марта", L"апреля", L"мая",
                          L"июня", L"июля", L"августа", L"сентября", L"октября",
                          L"ноября", L"декабря" };
    const int num_months = 12;

    for (int s = 0; s < t->n; s++) {
        struct Sentence *sentence = t->sents[s];
        int len = sentence->size;
        int i = 0;

        while (i < len) {
            if (iswdigit(sentence->str[i])) {
                int day;
                int n_day = 0;
                if (swscanf(&sentence->str[i], L"%d%n", &day, &n_day) == 1) {
                    int j = i + n_day;
                    if (j < len && sentence->str[j] == L' ') {
                        j++;

                        int month_num = -1;
                        int month_len = 0;
                        for (int m = 0; m < num_months; m++) {
                            month_len = wcslen(months[m]);
                            if (j + month_len <= len && wcsncmp(&sentence->str[j], months[m], month_len) == 0) {
                                month_num = m + 1;
                                break;
                            }
                        }

                        if (month_num > 0) {
                            j += month_len;
                            if (j < len && sentence->str[j] == L' ') {
                                j++;
                                int year;
                                int n_year = 0;
                                if (swscanf(&sentence->str[j], L"%d%n", &year, &n_year) == 1) {
                                    j += n_year;
                                    int g_len = 0;
                                    if (j + 3 <= len && wcsncmp(&sentence->str[j], L" г.", 3) == 0) {
                                        g_len = 3;

                                    } else if (j + 2 <= len && wcsncmp(&sentence->str[j], L" г", 2) == 0) {
                                        g_len = 2;
                                    }

                                    if (g_len > 0) {
                                        j += g_len;
                                        while (j < len && (sentence->str[j] == L'.' || sentence->str[j] == L',')) {
                                            j++;
                                        }

                                        wchar_t new_date[11];
                                        swprintf(new_date, sizeof(new_date)/sizeof(wchar_t), L"%02d/%02d/%04d", day, month_num, year);

                                        int old_len = j - i;
                                        int new_len = wcslen(new_date);
                                        int diff = new_len - old_len;

                                        if (diff != 0) {
                                            int new_sentence_len = len + diff;
                                            wchar_t *new_data = (wchar_t *)malloc((new_sentence_len + 1) * sizeof(wchar_t));
                                            if (!new_data) {
                                                wprintf(L"Error: Memory allocation error in function replace_dates\n");
                                                exit(1);
                                            }

                                            wmemcpy(new_data, sentence->str, i);
                                            wmemcpy(new_data + i, new_date, new_len);
                                            wmemcpy(new_data + i + new_len, sentence->str + j, len - j);
                                            new_data[new_sentence_len] = L'\0';

                                            free(sentence->str);
                                            sentence->str = new_data;
                                            len += diff;
                                        } else {
                                            wmemcpy(sentence->str + i, new_date, new_len);
                                        }

                                        i += new_len;
                                        continue;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            i++;
        }
    }
}


void changeSentence(struct Text* t, wchar_t* word) {
	for (int i = 0; i < t -> n; i++) {
		wchar_t* sentcpy = wcsdup(t -> sents[i] -> str);
		
		wchar_t* state;
		wchar_t* token = wcstok(sentcpy, L" ", &state);
		
		for (int p = 0; p <= wcslen(word); p++) {
			word[p] = towlower(word[p]);
		}
		
		while (token != NULL) {
			int counter = 0;
			cleanWord(token);
			int len = wcslen(token);
			for (int l = 0; l <= wcslen(token); l++) {
				token[l] = towlower(token[l]);
			}
			
			for (int j = 0; j < len; j++) {
				for (int k = 0; k < wcslen(word); k++) {
					if (token[j] == word[k]) {
						counter += 1;
						break;
					}
				}
			}
			
			if (counter == len) {
				wprintf(L"%ls\n", token);
			}
			token = wcstok(NULL, L" ", &state);
		}
		free(sentcpy);
	}
}
					




