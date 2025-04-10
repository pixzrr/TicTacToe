#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

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

int main(void) {
    initscr();
    refresh();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);

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
        for (int i = COLS / 2 - 10; i <= COLS / 2 + 10; i++) {
            mvprintw(LINES / 2 - 2, i, "-");
            mvprintw(LINES / 2 + 2, i, "-");
        }
        for (int i = LINES / 2 - 5; i <= LINES / 2 + 5; i++) {
            mvprintw(i, COLS / 2 - 4, "|");
            mvprintw(i, COLS / 2 + 4, "|");
        }

        while (1) {
            choix = getch();
            int l = 0;
            int h = 0;
            switch (choix) {
                case KEY_UP:
                    if (l == 0){
                        break;
                    } else {
                        l += 1;
                        break;
                    }
                case KEY_DOWN:
                    if (l == 2){
                        break;
                    } else {
                        l -= 1;
                        break;
                    }
                case KEY_LEFT:
                    if (h == 0){
                        break;
                    } else {
                        attron(A_UNDERLINE);
                        mvprintw(LINES / 2 - 4, COLS / 2 - 8, " ");
                        attroff(A_UNDERLINE);
                        h -= 1;
                        break;
                    }
                case KEY_RIGHT:
                    if (h == 2){
                        break;
                    } else {
                        attron(A_UNDERLINE);
                        mvprintw(LINES / 2, COLS / 2 - 8, " ");
                        attroff(A_UNDERLINE);
                        h += 1;
                        break;
                    }
            }
            if (choix == ' '){
                if (l == 0 && h == 0){
                    mvprintw(LINES / 2 - 4, COLS / 2 - 8, "X");
                } else if (l == 1 && h == 0){
                    mvprintw(LINES / 2, COLS / 2 - 8, "X");
                }
            }

            if (choix == 'q' || choix == 'Q') {
                main();
            }
        }

        // case l=1 ; h=1
        mvprintw(LINES / 2 - 4, COLS / 2 - 8, "X");
        // case l=2 ; h=1
        mvprintw(LINES / 2, COLS / 2 - 8, "X");
        // case l=3 ; h=1
        mvprintw(LINES / 2 + 4, COLS / 2 - 8, "X");


        // case l=1 ; h=2
        mvprintw(LINES / 2 - 4, COLS / 2, "X");
        // case l=2 ; h=2
        mvprintw(LINES / 2, COLS / 2, "X");
        // case l=3 ; h=2
        mvprintw(LINES / 2 + 4, COLS / 2, "X");


        // case l=1 ; h=3
        mvprintw(LINES / 2 - 4, COLS / 2 + 8, "X");
        // case l=2 ; h=3
        mvprintw(LINES / 2, COLS / 2 + 8, "X");
        // case l=3 ; h=3
        mvprintw(LINES / 2 + 4, COLS / 2 + 8, "X");
    }

    if (highlight == 5) {
        endwin();
    }

    getch();
    endwin();
return 0;
}
