#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include "io.h"
#include "text_processing.h"
#include "utility.h"

int main() {
    setlocale(LC_CTYPE, "");
    int option = -1;

    wscanf(L"%d", &option);

    switch (option) {
        case 0: {
            struct Text t = readText();
            deleteLeadingSpace(&t);
            removeDuplicateSentences(&t);
            printText(t);
            freeText(t);
            break;
        }
        case 1: {
            struct Text t = readText();
            deleteLeadingSpace(&t);
            removeDuplicateSentences(&t);
            printUniqueWords(&t);
            freeText(t);
            break;
        }
        case 2: {
            struct Text t = readText();
            deleteLeadingSpace(&t);
            removeDuplicateSentences(&t);
            replace_dates(&t);
            printText(t);
            freeText(t);
            break;
        }
        case 3: {
            struct Text t = readText();
            deleteLeadingSpace(&t);
            removeDuplicateSentences(&t);
            sortSentencesByProduct(&t);
            printText(t);
            freeText(t);
            break;
        }
        case 4: {
            struct Text t = readText();
            deleteLeadingSpace(&t);
            removeDuplicateSentences(&t);
            RemoveSentencesWithSymbolsWithoutDigits(&t);
            printText(t);
            freeText(t);
            break;
        }
        case 5:
            print_help();
            break;
        case 9: {
			wchar_t* key = malloc(100 * sizeof(wchar_t));
			fgetwc(stdin);
			fgetws(key, 100, stdin);
			key[wcslen(key) - 1] = L'\0'; 
        	struct Text t = readText();
            deleteLeadingSpace(&t);
            removeDuplicateSentences(&t);
            changeSentence(&t, key);
            freeText(t);
            break;
        }
        default:
            wprintf(L"Error: Invalid option\n");
            break;
    }

    return 0;
}
