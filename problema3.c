#include <stdio.h>

typedef void (*func)();

void swap(unsigned int *nr1, unsigned int nr2) {
    // extrag pozitiile bitilor ce trebuie interschimbati
    unsigned int poz1 = nr2 >> 2;
    unsigned int poz2 = (nr2 << 30) >> 30;
    // extrag valoarea bitilor ce trebuie interschimbati
    unsigned int bit1 = (*nr1 & (1 << (3 - poz1))) >> (3 - poz1);
    unsigned int bit2 = (*nr1 & (1 << (3 - poz2))) >> (3 - poz2);
    // daca sunt diferiti ii interschimb
    if (bit1 != bit2) {
        *nr1 ^= (1 << (3 - poz1));
        *nr1 ^= (1 << (3 - poz2));
    }
}

void add(unsigned int *nr1, unsigned int nr2) {
    // variabile pentru rezultatul adunarii si restul ramas
    unsigned int rez = 0;
    unsigned int rest = 0;
    int i;
    for (i = 0; i < 4; i++) {
        // a, b = bitii numerelor extrasi doi cate doi
        unsigned int a = (*nr1 << (31 - i)) >> 31;
        unsigned int b = (nr2 << (31 - i)) >> 31;

        // daca am de adaugat un rest de la adunarea anterioara
        if (rest) {
            /*
            Daca si restul si primul bit sunt 1, restul ramane
            pe 1 iar primul bit devine 0
            Altfel primul bit trece pe 1 iar restul devine 0
            */
            if (a & rest) {
                a &= 0;
                rest |= 1;
            } else {
                a |= 1;
                rest &= 0;
            }
        }

        /*
        Daca un bit este 1 iar celalalt 0 rezultatul adunarii
        va fi 1, altfel va fi 0
        In cazul in care ambii biti sunt 1 voi ramane cu un rest
        in minte
        */
        if (a ^ b) {
            rez |= (1 << i);
        } else if (a & 1) {
            rest |= 1;
        }
    }

    // limitam rezultatul operatiei la 4 biti (ex 11001 devine 1001)
    rez = (rez << 28) >> 28;

    *nr1 = rez;
}

void rotate(unsigned int *nr1, unsigned int nr2) {
    int i;
    for (i = 0; i < nr2; i++) {
        // in c retin bitul ce se pierde
        unsigned int c = *nr1 >> 3;
        // fac ultimul bit 0 si fac o permutare la stanga
        *nr1 &= ~8;
        *nr1 <<= 1;
        // ii dau primului bit valoare bitului pierdut
        *nr1 ^= c;
    }
}

void xor(unsigned int *nr1, unsigned int nr2) {
    unsigned int rez = 0;
    int i;
    for (i = 0; i < 4; i++) {
        // a, b = bitii numerelor extrasi doi cate doi
        unsigned int a = (*nr1 << (31 - i)) >> 31;
        unsigned int b = (nr2 << (31 - i)) >> 31;
        // verific daca ambii biti sunt 1
        unsigned int unu = a & b;
        // verific daca ambii biti sunt 0
        unsigned int zero = (~a) & (~b);
        /*
        daca bitii nu sunt ambii 0 sau ambii 1 rezultatul
        pe bitul respectiv va fi 1
        */
        rez |= (~(unu | zero) << i);
    }

        *nr1 = rez;
}

unsigned int calculator(unsigned int n, short int op) {
    func f[] = {add, swap, rotate, xor};
    // extrag primul numar
    // ulterior in acesta va ramane rezultatul operatiilor efectuate
    unsigned int nr1 = (n << (28 - 6 * op)) >> 28;
    int i;
    for (i = 0; i < op; i++) {
        // extrag pe rand fiecare operatie ce trebuie efectuata
        unsigned int operatie = n << (32 - 6 * (op - i));
        // extrag al doilea numar din operatie
        unsigned int nr2 = (operatie << 2) >> 28;
        operatie >>= 30;
        f[operatie](&nr1, nr2);
    }

    return nr1;
}

int main() {
    // citire date
    short int op;
    unsigned int n;
    scanf("%hd", &op);
    getchar();
    scanf("%u", &n);

    // afisare rezultat
    printf("%u", calculator(n, op));

    return 0;
}