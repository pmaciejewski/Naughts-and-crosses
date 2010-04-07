#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<ncurses/ncurses.h>

/* rysowanie wykresu */

void rysuj(int *p)
{
    int i;
    char znak[9];

    for (i = 0; i < 9; i++) {

	switch (*(p + i)) {
	case 1:
	    znak[i] = 'x';
	    break;
	case 2:
	    znak[i] = 'o';
	    break;
	default:
	    znak[i] = ' ';
	    break;
	}
    }

    mvprintw(0,0,"%c|%c|%c\n", znak[0], znak[1], znak[2]);
    mvprintw(1,0,"-+-+-\n");
    mvprintw(2,0,"%c|%c|%c\n", znak[3], znak[4], znak[5]);
    mvprintw(3,0,"-+-+-\n");
    mvprintw(4,0,"%c|%c|%c\n", znak[6], znak[7], znak[8]);
}

/* wyczytywanie ruchu */

void wczytaj(int gracz, int pole, int *p)
{
    if (*(p + pole - 1) != 1 && *(p + pole - 1) != 2) {
	if (gracz == 1) {
	    *(p + pole - 1) = 1;
	}
	if (gracz == 2) {
	    *(p + pole - 1) = 2;
	}
    } else {
	mvprintw(5,0,"Gracz %d! Złe pole, podaj jeszcze raz:", gracz);
	scanw("%d", &pole);
	wczytaj(gracz, pole, p);
    }
}

 /*sprawdzanie wygranej */

int test(int gracz, int *p)
{
    int i, znak[9], win;
    //wygrana gracza 1; win = 1
    // wygrana gracza 2; win =2
    for (i = 0; i < 9; i++) {

	switch (*(p + i)) {
	case 1:
	    znak[i] = 1;
	    break;
	case 2:
	    znak[i] = 2;
	    break;
	default:
	    znak[i] = 0;
	    break;
	}
    }

    //sprawdzanie wierszy
    for (i = 0; i < 7; i = i + 3) {
	win = ((win == 1) || ((znak[i] == gracz) && (znak[i + 1] == gracz)
			      && (znak[i + 2] == gracz)));

    }
    // sprawdzanie kolumn 
    for (i = 0; i < 3; i = i + 3) {
	win = ((win == 1) || ((znak[i] == gracz) && (znak[i + 3] == gracz)
			      && (znak[i + 6] == gracz)));
    }
    //sprawdzenie przekatnych
    win = ((win == 1)
	   || ((znak[0] == gracz) && (znak[4] == gracz)
	       && (znak[8] == gracz)));
    win = ((win == 1)
	   || ((znak[2] == gracz) && (znak[4] == gracz)
	       && (znak[6] == gracz)));

    if (win == 1 && gracz == 1) {
	win = 1;
    }
    if (win == 1 && gracz == 2) {
	win = 2;
    }
    return win;
}

int menu(void)
{
    int tryb, i, exit=0;
    int x = 10, y = 2, ch;	//s terowanie kursorem
    initscr();
    WINDOW *menu;
    menu = newwin(7, 35, 10, 10);
    keypad(menu,true);
    init_pair(1, COLOR_BLUE, COLOR_RED);
    wattrset(menu, COLOR_PAIR(1));
    /* kolorowanko menu, inaczej nie umiem ;p */
    for (i = 0; i < 7 * 35; i++) {
	wprintw(menu, " ");
    }

    mvwprintw(menu, 1, 2, "Wesola gra w kolko i krzyzyk.");
    mvwprintw(menu, 2, 3, "single player ");
    mvwprintw(menu, 3, 3, "multiplayer ");
    mvwprintw(menu, 6, 3, "sterowanie: strzalki + enter");
    wmove(menu, y, x);
    //wybór z menu
    while (exit < 1) {		
        wrefresh(menu);
	ch = wgetch(menu);
	switch (ch) {
	case KEY_UP:
	    y--;
	    wmove(menu, y, x);
	    break;
	case KEY_DOWN:
	    y++;
	    wmove(menu, y, x);
	    break;
	case 0xa:
	    getyx(menu, y, x);
	    if (y == 2 || y == 3) {
		exit = 1; //jak klikniemy enter w 3 lub 4 koumnie (single, multi) to zmienia flagę wyjścia na 1 i wychodzi z while
	    }
	    break;



	}
    }
    if (y == 2) {		//single player
	tryb = 1;
    }
    if (y == 3) {
	tryb = 2;
    }

    return tryb;
}
void pc1(int *p)		//zrobilem z tego voida, bo nie musi zwracac wyniku, moze go od razu w funkcji zapisac
{
    int pole;
    srand(time(NULL));
    while (1) {			//jak dajesz while(0), to w ogole nie wchodzi do petli
	pole = rand() % 10;
	if (*(p + pole - 1) == 0) {
	    *(p + pole - 1) = 2;	//p+pole-1, bo numerujemy od zera
	    break;
	}
    }
}
int main()
{
    int p1[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int win = 0;
    int pole, remis, tryb;
    int ch, x = 2, y = 2, exit = 0;	//do operacji na kursorze

    initscr();
    cbreak();
    noecho();// ja jeeeeeeeeeeeeeeeeeeeeebie, działa! ;d
    keypad(stdscr, true);	//do zabawy kursorem
    start_color();
    beep();

    tryb = menu();


    while (remis < 9) {
	rysuj(p1);

	exit = 0;
	// todo: przeniesc to do funkcji wczytaj i zrobic sprawdzanie czy nie jest juz zajete pole
	while (exit<1) {
		move(y,x);
		refresh();
		ch = getch();
	    switch (ch) {
	    case KEY_LEFT:
		x--;
		move(y, x);
		refresh();
		break;
	    case KEY_RIGHT:
		x++;
		move(y, x);
		refresh();
		break;
	    case KEY_UP:
		y--;
		move(y, x);
		refresh();
		break;
	    case KEY_DOWN:
		y++;
		move(y, x);
		refresh();
		break;
	    case 0xa:
		getyx(stdscr, y, x);	// y = wiersz kursora y = kolumna kursora
		if (x == 0 && y == 0) {
			pole = 1;
			exit = 1;
		}
		else if ((x == 2) && (y == 0)) {
			pole = 2;
			exit = 1;
		}
		else if (x == 4 && y == 0) {
			pole = 3;
			exit = 1;
		}
		else if (x == 0 && y == 2) {
			pole = 4;
			exit = 1;
		}
		else if (x == 2 && y == 2) {
			pole = 5;
			exit = 1;
		}
		else if (x == 4 && y == 2) {
			pole = 6;
			exit = 1;
		}
		else if (x == 0 && y == 4) {
			pole = 7;
			exit = 1;
		}
		else if (x == 2 && y == 4) {
			pole = 8;
			exit = 1;
		}
		else if (x == 4 && y == 4) {
			pole = 9;
			exit = 1;
		}
				break;
			  }

	}
	

	wczytaj(1, pole, p1);
	remis++;
	if (remis == 9) {

	    printw("Remis\n");
	    break;
	}

	win = test(1, p1);
	if (win > 0) {
	    break;
	}
	/* wybór trybu single/multi */

	if (tryb == 2) {
	    rysuj(p1);		//rysuje planszę po ruchu pierwszego gracza tylko jak jest multiplayer
	    printw("Gracz 2, podaj pole [1-9]:");
	    scanw("%d", &pole);
	    wczytaj(2, pole, p1);
	}
	if (tryb == 1) {
	    pc1(p1);
	}
	win = test(2, p1);
	remis++;
	if (remis == 9) {
	    printw("Remis\n");
	    break;
	}
	if (win > 0) {
	    rysuj(p1);
	    break;
	}
	refresh();
    }
    if (win == 1) {
	mvprintw(5,0,"Gratulacje! Gracz 1, wygrałeś.\n");
    }
    if (win == 2) {
	mvprintw(5,0,"Gratulacje! Gracz 2 wygrywa!\n");
    }
    getch();
    endwin();
    return 0;
}
