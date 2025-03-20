#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

void menu(int highlight) {
    const char *elements[] = {
        "Joueur contre Joueur",
        "Joeur contre IA",
        "IA contre IA",
        "Afficher le score",
        "Reinitialiser le score",
        "Quitter"
    };

    int nombre_items = sizeof(elements) / sizeof(elements[0]); // Utilisé pour savoir dynamiquement combien d'items sont dans le tableau (on divise la taille du tableau par la taille d'un pointeur

    for (int i = 0; i < nombre_items; i++) {
    }

    printw("Choisissez parmis les %d elements du menu, et appuyez sur entree lorsque votre choix est fait\n\n", nombre_items);

    for (int i = 0; i < nombre_items; i++) {
        if (highlight == i) {
            attron(A_UNDERLINE);
            printw("%s\n", elements[highlight]);
            attroff(A_UNDERLINE);
        } else {
            printw("%s\n",elements[i]);
        }
    }
}

int logo() {
    printw("----------------------------------------------------------------------- \n");
    printw(" TTTTT  III  CCCCC      TTTTT    AAAAA   CCCCC      TTTTT   OOO   EEEEE \n");
    printw("   T     I   C            T     A     A  C            T    O   O  E     \n");
    printw("   T     I   C            T     AAAAAAA  C            T    O   O  EEEE  \n");
    printw("   T     I   C            T     A     A  C            T    O   O  E     \n");
    printw("   T    III  CCCCC        T     A     A  CCCCC        T     OOO   EEEEE \n");
    printw("----------------------------------------------------------------------- \n");
    return 0;
}

int main(void) {
    initscr();
    refresh();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    int highlight = 0;
    int choix;
    while (1) {

        clear();
        logo();
        menu(highlight);
        refresh();

        choix = getch();
        switch (choix) {
            case KEY_UP:
                if (highlight == 0) {
                    break;
                } else {
                    highlight -= 1;
                    break;
                }
            case KEY_DOWN:
                if (highlight == 5) {
                    break;
                } else {
                    highlight += 1;
                    break;
                }
        }
        if (choix == ' ') {
            break;
        }
    }



    if (highlight == 0) {
        clear();
        refresh();
        mvprintw(10, 10, "-");
        mvprintw(10, 11, "-");
        mvprintw(10, 12, "-");
    }

    getch();
    endwin();
return 0;
}
