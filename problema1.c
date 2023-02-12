#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEYWORD1 11
#define KEYWORD2 4
#define ROWLENGTH 110
#define WORDLENGTH 7
#define MATCH2(p, pp) (strcmp(p, word2[j]) == 0 && strcmp(pp, word3[j]) == 0)
#define MATCH1(p) strcmp(p, word1[j]) == 0

// aloca memorie, citeste textul si initializeaza matricea de subliniere
void readText(char **text, char **undr, int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        char aux[ROWLENGTH];
        fgets(aux, ROWLENGTH, stdin);
        int length = strlen(aux);
        aux[length - 1] = '\0';
        length--;
        text[i] = (char *)malloc(length + 1);
        strcpy(text[i], aux);
        undr[i] = (char *)malloc(length + 1);
        for (j = 0; j < length; j++) undr[i][j] = ' ';
    }
}

// subliniaza cuvantul/secventa de cuvinte cheie gasit/e
void underlineWord(char *row, char *undrRow, int nrWord, int lengthSecv) {
    // lf si rg = inceputul si sfarsitul secventei ce trebuie subliniate
    // spaces = numarul de spatii intalnite
    int lf = 0, rg = 0, spaces = 0, i;
    while (row[lf] == ' ') lf++;

    // merg cu "lf" pana la inceputul sencventei ce trebuie subliniate
    while (spaces < nrWord) {
        while (1) {
            if (row[lf] == ' ') {
                // am gasit nu nou set de spatii
                spaces++;
                // trec peste tot setul de spatii
                while (row[lf] == ' ') lf++;
                break;
            }
            lf++;
        }
    }

    // merg cu "rg" pana la sfarsitul secventei ce trebuie subliniate
    rg = lf;
    while (row[rg + 1] != ' ' && row[rg + 1] != '\0') rg++;

    /* daca este cautata o secventa de 2 cuvinte repet procesul pentru
    inca un cuvant */
    if (lengthSecv == 2 && row[rg + 1] != '\0') {
        rg++;
        while (row[rg] == ' ') rg++;
        while (row[rg + 1] != ' ' && row[rg + 1] != '\0') rg++;
    }

    // subliniez secventa
    for (i = lf; i <= rg; i++) undrRow[i] = '_';
}

// cauta pe fiecare rand secventele de cuvinte cheie(cate doua cuvinte)
void underlineRowOne(char **text, char **undr, int n) {
    int i, j;
    char word2[KEYWORD2][WORDLENGTH] = {"first", "for", "is", "list"};
    char word3[KEYWORD2][WORDLENGTH] = {"of", "each", "a", "of"};

    // parcurg fiecare rang si extrag cate doua cuvinte consecutive
    for (i = 0; i < n; i++) {
        char *aux = (char *)malloc(strlen(text[i]));
        strcpy(aux, text[i]);
        char *p;
        int nrWord = -1;
        // primul cuvant
        p = strtok(aux, " ");
        while (p) {
            nrWord++;
            char *pp;
            // al doilea cuvant
            pp = strtok(NULL, " ");
            /* daca cele doua cuvinte fac match pe vreun set de
            cuvinte cheie le subliniez */
            if (pp) {
                for (j = 0; j < KEYWORD2; j++) {
                    if (MATCH2(p, pp)) {
                        underlineWord(text[i], undr[i], nrWord, 2);
                        break;
                    }
                }
            }
            // al doilea cuvant devine primul in urmatoarea cautare
            p = pp;
        }
    }
}

// cauta pe fiecare rand cuvintele cheie(cate un singur cuvant)
void underlineRowTwo(char **text, char **undr, int n) {
    int i, j;
    char word1[KEYWORD1][WORDLENGTH] = {"for",   "from",   "in",    "is",
                                        "unit",  "or",     "while", "int",
                                        "float", "double", "string"};

    // parcurg fiecare rand si extrag fiecare cuvant
    for (i = 0; i < n; i++) {
        char *aux = (char *)malloc(strlen(text[i]));
        strcpy(aux, text[i]);
        char *p;
        int nrWord = -1;
        // cuvantul
        p = strtok(aux, " ");
        // daca acest cuvant se afla printre cele cautate este subliniat
        while (p) {
            nrWord++;
            for (j = 0; j < KEYWORD1; j++) {
                if (MATCH1(p)) {
                    underlineWord(text[i], undr[i], nrWord, 1);
                    break;
                }
            }
            p = strtok(NULL, " ");
        }
    }
}

int main() {
    // citire date
    int n;
    scanf("%d", &n);
    getchar();
    char **text = (char **)malloc(n * sizeof(char *));
    char **undr = (char **)malloc(n * sizeof(char *));
    readText(text, undr, n);

    underlineRowOne(text, undr, n);
    underlineRowTwo(text, undr, n);

    // afisari
    int i;
    for (i = 0; i < n; i++) {
        puts(text[i]);
        puts(undr[i]);
    }

    return 0;
}