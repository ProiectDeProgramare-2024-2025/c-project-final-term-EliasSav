#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>


#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"

typedef struct {
    char nume[50];
    int scor_total;
} Jucator;

char* alege_cuvant() {
    static char cuvinte[100][50];
    int nr_cuvinte = 0;
    FILE *f = fopen("cuvinte.txt", "r");
    if (!f) {
        printf(RED "Eroare: nu s-a putut deschide cuvinte.txt\n" RESET);
        return NULL;
    }
    while (fscanf(f, "%49s", cuvinte[nr_cuvinte]) == 1 && nr_cuvinte < 100) {
        nr_cuvinte++;
    }
    fclose(f);
    if (nr_cuvinte == 0) return NULL;
    srand(time(NULL));
    int index = rand() % nr_cuvinte;
    return cuvinte[index];
}

void salveaza_scorul(const char* nume_jucator, int scor) {
    Jucator jucatori[100];
    int nr_jucatori = 0;
    FILE *f = fopen("scoruri.txt", "r");
    if (f != NULL) {
        char name[50];
        int sc;
        while (fscanf(f, "%49s %d", name, &sc) == 2) {
            strcpy(jucatori[nr_jucatori].nume, name);
            jucatori[nr_jucatori].scor_total = sc;
            nr_jucatori++;
        }
        fclose(f);
    }
    int gasit = 0;
    for (int i = 0; i < nr_jucatori; i++) {
        if (strcmp(jucatori[i].nume, nume_jucator) == 0) {
            jucatori[i].scor_total += scor;
            gasit = 1;
            break;
        }
    }
    if (!gasit) {
        strcpy(jucatori[nr_jucatori].nume, nume_jucator);
        jucatori[nr_jucatori].scor_total = scor;
        nr_jucatori++;
    }
    f = fopen("scoruri.txt", "w");
    if (f == NULL) {
        printf(RED "Eroare la scrierea in scoruri.txt\n" RESET);
        return;
    }
    for (int i = 0; i < nr_jucatori; i++) {
        fprintf(f, "%s %d\n", jucatori[i].nume, jucatori[i].scor_total);
    }
    fclose(f);
    f = fopen("istoric.txt", "a");
    if (f == NULL) {
        printf(RED "Eroare la scrierea in istoric.txt\n" RESET);
        return;
    }
    fprintf(f, "%s %d\n", nume_jucator, scor);
    fclose(f);
}

void afiseaza_meniu() {
    printf(BLUE "--Joc de tip spanzuratoare--\n" RESET);
    printf("1. %sJoc nou%s\n", YELLOW, RESET);
    printf("2. %sClasament%s\n", GREEN, RESET);
    printf("3. %sIstoric jocuri%s\n", CYAN, RESET);
    printf("4. %sIesire%s\n", RED, RESET);

}
int citeste_optiune() {
    int optiune;
    printf("Introduceti o optiune (1-4): ");
    while (scanf("%d", &optiune) != 1 || optiune < 1 || optiune > 4) {
        while (getchar() != '\n');
        printf(RED "Optiune invalida! Introduceti un numar intre 1 si 4: " RESET);
    }
    return optiune;
}

void citeste_nume(char* nume) {
    int valid = 0;
    while (!valid) {
        printf("Introduceti numele jucatorului (fara spatii, doar litere, max 49 caractere): ");
        scanf("%49s", nume);

        valid = 1;
        if (strlen(nume) == 0) {
            valid = 0;
        } else {
            for (int i = 0; nume[i] != '\0'; i++) {
                if (!isalpha(nume[i])) {
                    valid = 0;
                    break;
                }
            }
        }

        if (!valid) {
            printf(RED "Nume invalid. Folositi doar litere si fara spatii!\n" RESET);
        }
    }
}


// [Nicio modificare în #include sau typedef]

// Adăugare/înlocuire funcții modificate sau complet noi

void afiseaza_progres(const char* progres) {
    printf(YELLOW "Progres: " RESET);
    for (int i = 0; progres[i] != '\0'; i++) {
        if (progres[i] == '_') {
            printf(RED "_ " RESET);
        } else {
            printf(GREEN "%c " RESET, progres[i]);
        }
    }
    printf("\n");
}

void joc_nou(char Player_name[50]) {
    printf(BLUE "--Joc nou--\n" RESET);
    citeste_nume(Player_name);
    int scor = 0, gresite = 0;
    while (gresite < 5) {
        char* cuvant = alege_cuvant();
        if (cuvant == NULL) {
            printf(RED "Nu s-a putut alege un cuvant.\n" RESET);
            break;
        }
        int lungime = strlen(cuvant);
        char progres[50];
        int gasite[50] = {0};
        for (int i = 0; i < lungime; i++) {
            progres[i] = '_';
        }
        progres[lungime] = '\0';

        char prima = cuvant[0], ultima = cuvant[lungime - 1];
        for (int i = 0; i < lungime; i++) {
            if (cuvant[i] == prima || cuvant[i] == ultima) {
                progres[i] = cuvant[i];
                gasite[i] = 1;
            }
        }

        printf("\n" CYAN "Cuvant de ghicit: " RESET);
        for (int i = 0; i < lungime; i++) {
            if (progres[i] == '_') {
                printf(RED "_ " RESET);
            } else {
                printf(GREEN "%c " RESET, progres[i]);
            }
        }
        printf("\n");

        while (1) {
            char litera;
            printf("Introduceti o litera: ");
            scanf(" %c", &litera);
            litera = tolower(litera);
            int gasit = 0;
            for (int i = 0; i < lungime; i++) {
                if (cuvant[i] == litera && !gasite[i]) {
                    progres[i] = litera;
                    gasite[i] = 1;
                    gasit = 1;
                }
            }

            if (!gasit) {
                gresite++;
                printf(RED "Litera incorecta! Greseli: %d/5\n" RESET, gresite);
            } else {
                afiseaza_progres(progres);
            }

            if (strcmp(progres, cuvant) == 0) {
                printf(GREEN "Ai ghicit cuvantul! +%d puncte\n" RESET, lungime);
                sleep(3);  // asteapta 3 secunde
                scor += lungime;
                break;
            }


            if (gresite >= 5) {
                break;
            }
        }

        if (gresite >= 5) {
            printf(RED "Ai pierdut! Ai depasit limita de 5 greseli.\n" RESET);
            printf(YELLOW "Cuvantul era: %s%s%s\n" RESET, CYAN, cuvant, RESET);
            Sleep(3000);  // Așteaptă 3 secunde înainte de a curăța ecranul
        }
        system("cls"); // sau "clear" pentru Linux/Mac
    }

    salveaza_scorul(Player_name, scor);
    printf(GREEN "Scorul tau a fost salvat.\n" RESET);
    printf("Apasa Enter pentru a continua...");
    while (getchar() != '\n');
    getchar();
    system("cls");
}

void clasament() {
    FILE *file = fopen("scoruri.txt", "r");
    if (file == NULL) {
        printf(RED "Nu s-a putut deschide fisierul scoruri.txt\n" RESET);
        return;
    }
    Jucator jucatori[100];
    int nr_jucatori = 0;
    char name[50];
    int score;
    while (fscanf(file, "%49s %d", name, &score) == 2) {
        int gasit = 0;
        for (int i = 0; i < nr_jucatori; i++) {
            if (strcmp(jucatori[i].nume, name) == 0) {
                jucatori[i].scor_total += score;
                gasit = 1;
                break;
            }
        }
        if (!gasit) {
            strcpy(jucatori[nr_jucatori].nume, name);
            jucatori[nr_jucatori].scor_total = score;
            nr_jucatori++;
        }
    }
    fclose(file);
    for (int i = 0; i < nr_jucatori - 1; i++) {
        for (int j = i + 1; j < nr_jucatori; j++) {
            if (jucatori[j].scor_total > jucatori[i].scor_total) {
                Jucator temp = jucatori[i];
                jucatori[i] = jucatori[j];
                jucatori[j] = temp;
            }
        }
    }
    printf(BLUE "--Clasament general--\n" RESET);
    for (int i = 0; i < nr_jucatori; i++) {
        printf("%d. %s%s%s - %s%d%s %spuncte%s\n",
           i + 1,
           CYAN, jucatori[i].nume, RESET,
           GREEN, jucatori[i].scor_total, RESET,
           YELLOW, RESET);
    }
    printf("\nApasati enter pentru a reveni\n");
    while (getchar() != '\n');
    getchar();
    system("cls");
}

void istoric_jocuri(char Player_name[50]) {
    if (Player_name[0] == '\0') {
        printf(RED "Pentru a vedea istoricul trebuie sa incepi un joc nou si sa introduci un nume!\n" RESET);
    } else {
        FILE *file = fopen("istoric.txt", "r");
        if (file == NULL) {
            printf(RED "Nu s-a putut deschide fisierul istoric.txt\n" RESET);
            return;
        }
        printf("Istoric pentru %s%s%s:\n", CYAN, Player_name, RESET);
        char name[50];
        int score, found = 0, joc_nr = 1;
        while (fscanf(file, "%49s %d", name, &score) == 2) {
            if (strcmp(name, Player_name) == 0) {
                printf("Joc %d: %s%d%s %s\n", joc_nr++, GREEN, score, RESET, CYAN "puncte" RESET);
                found = 1;
            }
        }
        if (!found) {
            printf(YELLOW "Nu exista scoruri pentru acest jucator.\n" RESET);
        }
        fclose(file);
    }
    printf("\nApasati enter pentru a reveni\n");
    while (getchar() != '\n');
    getchar();
    system("cls");
}


int main() {
    int choice = 0;
    char Player_name[50] = "";
    while (1) {
        afiseaza_meniu();
        choice = citeste_optiune();
        system("cls");
        switch (choice) {
            case 1:
                joc_nou(Player_name);
                break;
            case 2:
                clasament();
                break;
            case 3:
                istoric_jocuri(Player_name);
                break;
            case 4:
                printf(GREEN "Game over!\n" RESET);
                return 0;
        }
    }
    return 0;
}
