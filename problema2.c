#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE sizeof(dictionary_entry)
#define WORDSIZE 25
#define MAXWORD 5001

typedef struct {
    char *word;
    int priority;
} dictionary_entry;

// citeste cuvintele din dictionar
void readDictionary(dictionary_entry *dict, int n) {
    int i;
    for (i = 0; i < n; i++) {
        char aux[WORDSIZE];
        scanf("%s", aux);
        getchar();
        dict[i].word = (char *)malloc((strlen(aux) + 1));
        strcpy(dict[i].word, aux);
        dict[i].priority = 0;
    }
}

// citeste cuvintele ce trebuiesc completate
void readComplet(char complet[][WORDSIZE], int m) {
    int i;
    for (i = 0; i < m; i++) {
        scanf("%s", complet[i]);
        getchar();
    }
}

void freeDictionary(dictionary_entry *dict, int n) {
    int i;
    for (i = 0; i < n; i++) {
        free(dict[i].word);
    }

    free(dict);
}

// realoca dictionarul cu un element in plus
void reallocDictionary(dictionary_entry **dict, int *n) {
    (*n)++;
    (*dict) = (dictionary_entry *)realloc(*dict, (*n) * SIZE);
}

// adauga un cuvant in dictionar
void addDictionary(dictionary_entry *dict, int *n, char cuv[]) {
    int dictSize = *n;
    dict[dictSize - 1].word = (char *)malloc((strlen(cuv) + 1));
    strcpy(dict[dictSize - 1].word, cuv);
    dict[dictSize - 1].priority = 1;
}

// cauta un cuvant in dictionar, returneaza 1 daca il gaseste / 0 daca nu
int isInDictionary(dictionary_entry *dict, int *n, char cuv[]) {
    int i, length = strlen(cuv);
    for (i = 0; i < *n; i++) {
        if (strlen(dict[i].word) == length && strcmp(dict[i].word, cuv) == 0) {
            (dict[i].priority)++;
            return 1;
        }
    }

    return 0;
}

// verifica daca exista vreun match pentru cuvant
int matchDictionary(dictionary_entry *dict, int *n, char cuv[]) {
    int i, length = strlen(cuv);
    for (i = 0; i < *n; i++) {
        if (strncmp(dict[i].word, cuv, length) == 0) {
            return 1;
        }
    }

    return 0;
}

// cauta cel mai bun match pentru un cuvant in dictionar
void searchDictionary(dictionary_entry *dict, int *n, char cuv[]) {
    int position = 0, maxPriority = -1, i;
    int length = strlen(cuv);
    char lastMatch[WORDSIZE];
    strcpy(lastMatch, dict[0].word);
    for (i = 0; i < (*n); i++) {
        if (strncmp(dict[i].word, cuv, length) == 0) {
            if ((dict[i].priority > maxPriority) ||
                (dict[i].priority == maxPriority &&
                 strcmp(dict[i].word, lastMatch) < 0)) {
                maxPriority = dict[i].priority;
                strcpy(lastMatch, dict[i].word);
                position = i;
            }
        }
    }

    strcpy(cuv, dict[position].word);
    (dict[position].priority)++;
}

// autocompleteaza cuvintele
void start(dictionary_entry **dict, char complet[][WORDSIZE], int *n, int m) {
    int i;
    for (i = 0; i < m; i++) {
        int length = strlen(complet[i]);
        if (complet[i][length - 1] == '*') {
            complet[i][length - 1] = '\0';
            if (!isInDictionary(*dict, n, complet[i])) {
                reallocDictionary(dict, n);
                addDictionary(*dict, n, complet[i]);
            }
        } else if (!matchDictionary(*dict, n, complet[i])) {
            reallocDictionary(dict, n);
            addDictionary(*dict, n, complet[i]);
        } else {
            searchDictionary(*dict, n, complet[i]);
        }
    }
}

int main() {
    int n, m;

    // citire cuvine din dictionar
    scanf("%d", &n);
    getchar();
    dictionary_entry *dict = (dictionary_entry *)calloc(n, SIZE);
    readDictionary(dict, n);

    // citire cuvinte ce trebuie completate
    scanf("%d", &m);
    getchar();
    char complet[MAXWORD][WORDSIZE];
    readComplet(complet, m);

    start(&dict, complet, &n, m);

    // afisari
    int i;
    for (i = 0; i < m; i++) printf("%s ", complet[i]);

    freeDictionary(dict, n);

    return 0;
}