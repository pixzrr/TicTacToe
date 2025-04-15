#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

// Variables pour le score
int score_X = 0;
int score_O = 0;

// Variables pour le revisionnage
int historique_x[9];
int historique_y[9];
char historique_symbole[9];
int nb_coups = 0;

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

    printw("Choisissez parmis les %d elements du menu, et appuyez sur espace lorsque votre choix est fait\n\n", nombre_items);

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



char verif_victoire(char grille[3][3]) {
    // Pour les lignes et les colonnes (on vérifie toutes les coordonnées "gagnantes")
    for (int i = 0; i < 3; i++) {
        // Lignes
        if (grille[i][0] != ' ' && grille[i][0] == grille[i][1] && grille[i][1] == grille[i][2]) {
            return grille[i][0];
        }
        // Colonnes
        if (grille[0][i] != ' ' && grille[0][i] == grille[1][i] && grille[1][i] == grille[2][i]) {
            return grille[0][i];
        }
    }

    // Pour les diagonales
    if (grille[0][0] != ' ' && grille[0][0] == grille[1][1] && grille[1][1] == grille[2][2]) {
        return grille[0][0];
    }
    if (grille[0][2] != ' ' && grille[0][2] == grille[1][1] && grille[1][1] == grille[2][0]) {
        return grille[0][2];
    }

    // Quand personne gagne
    return ' ';
}

int mouvement_ia(char grille[3][3], char symbole, int *x, int *y) {
    char adversaire;
    if (symbole == 'X') {
        adversaire = 'O';
    } else {
        adversaire = 'X';
    }

// Cet algo est basé sur l'algo Minmax
    // 1) On regarde si on peut gagner en appelant la fonction qui verifie les coordonnées "gagnantes"
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grille[i][j] == ' ') {
                grille[i][j] = symbole;
                if (verif_victoire(grille) == symbole) {
                    
                    *x = j;
                    *y = i;
                    
                    grille[i][j] = ' ';
                    return 1;
                }
                grille[i][j] = ' ';
            }

    // 2) Pareil, on vérifie sil'adversaire peut gagner en appelant la même fonction que dans l'étape 1
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grille[i][j] == ' ') {
                grille[i][j] = adversaire;
                if (verif_victoire(grille) == adversaire) {
                    
                    *x = j;
                    *y = i;
                    
                    grille[i][j] = ' ';
                    return 1;
                }
                grille[i][j] = ' ';
            }

    // 3) Si on peut ni ganer ni bloquer, on prend le centre (c'est là où on a le plus de chances de gagner)
    if (grille[1][1] == ' ') {
        *x = 1;
        *y = 1;
        return 1;
    }

    // 4) Sinon on prend un coin
    int coins[4][2] = {{0,0}, {0,2}, {2,0}, {2,2}};
    for (int i = 0; i < 4; i++) {
        int cx = coins[i][1];
        int cy = coins[i][0];
        if (grille[cy][cx] == ' ') {
            
            *x = cx;
            *y = cy;
            
            return 1;
        }
    }

    // 5) Si les 4 autres conditions ne sont pas vérifiées, on prend un côté
    int sides[4][2] = {{0,1}, {1,0}, {1,2}, {2,1}};
    for (int i = 0; i < 4; i++) {
        int sx = sides[i][1];
        int sy = sides[i][0];
        if (grille[sy][sx] == ' ') {
            
            *x = sx;
            *y = sy;
            
            return 1;
        }
    }
    return 0;
}


void JvsJ(int IA) { // Si int IA == 1, ça veut dire qu'on est en Joueur vs IA
    char grille[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    int x = 0, y = 0;
    int choix;
    int compteur_tour = 0;

    while (1) {
        clear();
        // On dessine la grille
        for (int i = COLS / 2 - 10; i <= COLS / 2 + 10; i++) {
            mvprintw(LINES / 2 - 2, i, "-");
            mvprintw(LINES / 2 + 2, i, "-");
        }
        for (int i = LINES / 2 - 5; i <= LINES / 2 + 5; i++) {
            mvprintw(i, COLS / 2 - 4, "|");
            mvprintw(i, COLS / 2 + 4, "|");
        }
        
        for (int i = 0; i < 3; i++) {
            for (int j = 0 ; j < 3 ; j++) {

                if (i == y && j == x) {
                    attron(A_REVERSE); // On surligne la case sur laquelle on se trouve
                }

                mvprintw(LINES / 2 - 4 + i * 4, COLS / 2 - 8 + j * 7, " %c ", grille[i][j]);

                if (i == y && j == x) {
                    attroff(A_REVERSE);
                }
            }
        }

        refresh();
        choix = getch();

// Le joueur choisi où il veut mettre son pion
        switch (choix) {
            case KEY_UP:
                if (y > 0){
                    y--;
                }
                break;
            case KEY_DOWN:
                if (y < 2){
                    y++;
                }
                break;
            case KEY_LEFT:
                if (x > 0){
                    x--;
                }
                break;
            case KEY_RIGHT:
                if (x < 2){
                    x++;
                }
                break;
// Quand il apuye sur espace...
            case ' ':
                if (grille[y][x] == ' ') {
                    if (compteur_tour % 2 == 0) { // ...on vérifie à qui est le tour
                        grille[y][x] = 'X';
                        
                        // Pour le replay (on note les coordonnées du choix de ce tour)
                        historique_x[nb_coups] = x;
                        historique_y[nb_coups] = y;
                        historique_symbole[nb_coups] = 'X';
                        nb_coups++;
                        
                        // Au tour tour O, si le joueur a choisi Joueur vs IA, c'est l'IA qui joue
                        if (IA == 1) {
                            int ia_x, ia_y;
                            if (mouvement_ia(grille, 'O', &x, &y)) {
                                grille[y][x] = 'O';
                                compteur_tour++;
                                
                                // Pour le replay aussi
                                historique_x[nb_coups] = x;
                                historique_y[nb_coups] = y;
                                historique_symbole[nb_coups] = 'O';
                                nb_coups++;
                                }
                        }

                        
                    } else { // Sinon c'est au joueur 2 de jouer
                        grille[y][x] = 'O';
                        
                        // Replay
                        historique_x[nb_coups] = x;
                        historique_y[nb_coups] = y;
                        historique_symbole[nb_coups] = 'O';
                        nb_coups++;
                    }
                    compteur_tour++;
                    
                    // verif_victoire renvoie, O, x ou un espace blanc en fonction de qui a gagné
                    char victoire = verif_victoire(grille);
                    if (victoire != ' ') {
                        clear();
                        mvprintw(LINES / 2, COLS / 2, "Gagnant : %c", victoire);
                        mvprintw(LINES / 2 + 2, COLS / 2 - 5, "Appuyez sur une touche pour revenir au menu,");
                        mvprintw(LINES / 2 + 4, COLS / 2 - 7, "Ou appuyez sur r pour revisionner votre partie");

                        int choix;
                        choix = getch();
                            if (choix == 'r' || choix == 'R') {
                            revisionnage();
                        } else {
                            clear();
                            accueil();
                        }

                        // On ajoute 1 au score du joueur qui a gagné
                        if (victoire == 'X') {
                            score_X++;
                        }
                        else if (victoire == 'O') {
                            score_O++;
                        }
                        clear();
                        accueil();
                    }
                    
                    // Si les 9 cases sont remplies et que la fonction verif_victoire renvoie toujours un espace vide, c'est qu'il y a égalité
                    if (compteur_tour == 9) {
                        clear();
                        mvprintw(LINES / 2, COLS / 2, "Egalité");
                        mvprintw(LINES / 2 + 2, COLS / 2 - 5, "Appuyez sur une touche pour revenir au menu,");
                        mvprintw(LINES / 2 + 4, COLS / 2 - 7, "Ou appuyez sur r pour revisionner votre partie");
                        
                        // Note : il y a égalité donc on n'ajoute pas de score
                        
                        int choix;
                        choix = getch();
                            if (choix == 'r' || choix == 'R') {
                            revisionnage();
                        } else {
                            clear();
                            accueil();
                        }
                    }
                }
                break;

            case 'q':
                clear();
                accueil();
            case 'Q':
                clear();
                accueil();
        }
    }
}




void IAvsIA() { // Même principe que la fonction JvsJ. La seule différence majeure est qu'il n'y a pas besoin de switch vu que la fonction mouvement_ia fait déjà le boulot
    char grille[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    int compteur_tour = 0;
    char joueur_actuel = 'X';

    while (1) {
        clear();

        for (int i = COLS / 2 - 10; i <= COLS / 2 + 10; i++) {
            mvprintw(LINES / 2 - 2, i, "-");
            mvprintw(LINES / 2 + 2, i, "-");
        }
        for (int i = LINES / 2 - 5; i <= LINES / 2 + 5; i++) {
            mvprintw(i, COLS / 2 - 4, "|");
            mvprintw(i, COLS / 2 + 4, "|");
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0 ; j < 3 ; j++) { // Pas besoin de surligner la case sur laquelle on est vu que c'est l'IA qui joue
                mvprintw(LINES / 2 - 4 + i * 4, COLS / 2 - 8 + j * 7, " %c ", grille[i][j]);
            }
        }

        refresh();
        usleep(1000000);

        int x, y;
        if (mouvement_ia(grille, joueur_actuel, &x, &y)) {
            grille[y][x] = joueur_actuel;
            compteur_tour++;
            
            historique_x[nb_coups] = x;
            historique_y[nb_coups] = y;
            historique_symbole[nb_coups] = joueur_actuel;
            nb_coups++;
        }

        char victoire = verif_victoire(grille);
        if (victoire != ' ') {
            clear();
            mvprintw(LINES / 2, COLS / 2 - 10, "Gagnant : %c", victoire);
            mvprintw(LINES / 2 + 2, COLS / 2 - 15, "Appuyez sur une touche pour revenir au menu.");
            mvprintw(LINES / 2 + 4, COLS / 2 - 7, "Ou appuyez sur r pour revisionner votre partie");
            refresh();
            
            int choix;
            choix = getch();
            if (choix == 'r' || choix == 'R') {
                revisionnage();
            } else {
                clear();
                accueil();
            }
        }

        if (compteur_tour == 9) {
            clear();
            mvprintw(LINES / 2, COLS / 2, "Egalité");
            mvprintw(LINES / 2 + 2, COLS / 2 - 5, "Appuyez sur une touche pour revenir au menu.");
            mvprintw(LINES / 2 + 4, COLS / 2 - 7, "Ou appuyez sur r pour revisionner votre partie");
            refresh();
            
            int choix;
            choix = getch();
            if (choix == 'r' || choix == 'R') {
                revisionnage();
            } else {
                clear();
                accueil();
            }
        }
        
        if (joueur_actuel == 'X') {
            joueur_actuel = 'O';
        } else {
            joueur_actuel = 'X';
        }
    }
}




void score(){
    clear();
    refresh();
    mvprintw(LINES / 2, COLS / 2, "Score :");
    mvprintw(LINES / 2 + 3, COLS / 2 - 5, "X : %d", score_X);
    mvprintw(LINES / 2 + 4, COLS / 2 - 5, "O : %d", score_O);
    mvprintw(LINES / 2 + 7, COLS / 2 - 5, "Appuyez sur une touche pour revenir au menu.");
    getch();
    clear();
    accueil();
}

void score_reset(){
    clear();
    refresh();
    
    score_X = 0;
    score_O = 0;
    
    mvprintw(LINES / 2, COLS / 2, "Score réinitialisé avec succès");
    mvprintw(LINES / 2 + 2, COLS / 2 - 5, "Appuyez sur une touche pour revenir au menu.");
    getch();
    clear();
    accueil();
}









void revisionnage() { // Dans cette fonction, on "recrache" ce qui vient d'être joué (pour le coup 1, ces coordonnées ont été jouées, pour le coup 2, c'était ces coordonées, etc)
    char grille[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    for (int i = 0; i < nb_coups; i++) {
        grille[historique_y[i]][historique_x[i]] = historique_symbole[i];

        clear();

        for (int j = COLS / 2 - 10; j <= COLS / 2 + 10; j++) {
            mvprintw(LINES / 2 - 2, j, "-");
            mvprintw(LINES / 2 + 2, j, "-");
        }
        for (int j = LINES / 2 - 5; j <= LINES / 2 + 5; j++) {
            mvprintw(j, COLS / 2 - 4, "|");
            mvprintw(j, COLS / 2 + 4, "|");
        }

        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
                mvprintw(LINES / 2 - 4 + y * 4, COLS / 2 - 8 + x * 7, " %c ", grille[y][x]);
            }
        }

        refresh();
        usleep(1000000);
    }

    mvprintw(LINES / 2 + 6, 1, "Revisionnage terminé.\n Appuyez sur une touche pour revenir au menu.");
    getch();
    clear();
    accueil();
}


void accueil() {
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
                if (highlight > 0){
                    highlight--;
                }
                    break;
            case KEY_DOWN:
                if (highlight < 5){
                    highlight++;
                }
                    break;
            case ' ':
                switch (highlight) {
                    case 0:
                        clear();
                        JvsJ(0);
                        break;
                    case 1:
                        clear();
                        JvsJ(1);
                        break;
                    case 2:
                        clear();
                        IAvsIA();
                        break;
                    case 3:
                        clear();
                        score();
                        break;
                    case 4:
                        clear();
                        score_reset();
                        break;
                    case 5:
                        endwin();
                        exit(0);
                }
                break;
        }
    }
}

int main(void) {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    
    accueil();
    
    endwin();
    return 0;
}
