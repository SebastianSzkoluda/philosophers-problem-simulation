#include "visualisation.h"
#include "ncurses.h"
#include <iostream>
#include <string>

Visualisation::Visualisation()
{
    initscr();
    noecho();
    curs_set(0);
    start_color();

    init_pair(1, COLOR_WHITE, COLOR_BLACK);

    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);

    init_pair(4, COLOR_BLUE, COLOR_BLUE);
    init_pair(5, COLOR_YELLOW, COLOR_YELLOW);

    bkgd(COLOR_PAIR(1));

    getmaxyx( stdscr, rows, columns);
}

Visualisation::~Visualisation(){
    endwin();
}

void Visualisation::start(Fork** forks, Philosopher** philosophers) {

    PhilosopherState currentDrawStates[5];
    bool currentForkStates[5];


    // static draw
    attron(COLOR_PAIR(4));
    move(1,1);
    addch(' ');
    attron(COLOR_PAIR(1));
    printw(" Thinking progress");

    attron(COLOR_PAIR(5));
    move(2,1);
    addch(' ');
    attron(COLOR_PAIR(1));
    printw(" Eating progress");

    attron(COLOR_PAIR(3));
    move(3,1);
    addch('Y');
    attron(COLOR_PAIR(1));
    printw(" Fork in use");

    attron(COLOR_PAIR(2));
    move(4,1);
    addch('Y');
    attron(COLOR_PAIR(1));
    printw(" Fork free");

    move(5,1);
    attron(COLOR_PAIR(1));
    printw("\\o/ Philo with hands");


    // main loop
    int ch;
    nodelay(stdscr, TRUE);
    for (;;) {
        if ((ch = getch()) == ERR) {
            // redraw routine

            // temp

            // get data
            for(int i = 0; i < 5; i++) {
                currentDrawStates[i] = philosophers[i]->getState();
                currentForkStates[i] = forks[i]->isInUse();
            }

            for(int i = 0 ; i < 5; i++) {
                PhilosopherState state = currentDrawStates[i];
                if(state == LeftForkAquired) {
                    drawPhilosopher(i, true, false);
                } else if(state == RightForkAquired) {
                    drawPhilosopher(i, false, true);

                } else if(state == Eating) {
                    drawPhilosopher(i, true, true);

                } else {
                    drawPhilosopher(i, false, false);
                }

                // temp
                drawPhilosopherDetails(i, philosophers[i]->getCurrentProgress(), philosophers[i]->getState());
            }

            for(int i = 0 ; i < 5; i++) {
                bool inUse = currentForkStates[i];

                drawFork(i, inUse);
            }

            //timeout(1000);
            timeout(250);
        }
        else {
            return;
        }

    }
}

void Visualisation::drawFork(int id, bool inUse) {
    int forksY[] = {7, 13, 16, 13, 7};
    int forksX[] = {43, 41, 46, 51, 49};
    char forkSymbol = 'Y';

    move(forksY[id], forksX[id]);

    if(inUse) {
        attron(COLOR_PAIR(3));
    } else {
        attron(COLOR_PAIR(2));
    }

    addch(forkSymbol);


}

void Visualisation::drawPhilosopher(int id, bool usingLeft, bool usingRight) {
    attron(COLOR_PAIR(1));

    char noHand = ' ';

    char leftHands[] = {'/', '\\','_','|','\\'};
    char rightHands[] = {'|', '_', '/', '\\', '/'};

    int leftHandsY[] = {9, 15, 16, 11, 6};
    int leftHandsX[] = {41, 42, 48, 51, 47};

    int rightHandsY[] = {11, 16, 15, 9, 6};
    int rightHandsX[] = {41, 44, 50, 51, 45};

    int headsY[] = {10, 16, 16, 10, 6};
    int headsX[] = {41, 43, 49, 51, 46};

    move(leftHandsY[id], leftHandsX[id]);
    if(usingLeft) {
        addch(leftHands[id]);
    } else {
        addch(noHand);
    }

    move(rightHandsY[id], rightHandsX[id]);
    if(usingRight) {
        addch(rightHands[id]);
    } else {
        addch(noHand);
    }

    move(headsY[id], headsX[id]);
   // addch((char)0x2588);
    addch('O');


}

void Visualisation::drawPhilosopherDetails(int id, int progress, PhilosopherState state) {
    int topLeftY[] = {9, 18, 18, 9, 1};
    int topLeftX[] = {25, 30, 50, 55, 40};


    move(topLeftY[id] + 0, topLeftX[id]);
    printw("Philo: ");
    move(topLeftY[id] + 0, topLeftX[id] + 8);
    printw(std::to_string(id).c_str());
    move(topLeftY[id] + 1, topLeftX[id]);
    printw("+----------+");
    move(topLeftY[id] + 2, topLeftX[id]);
    printw("|          |");
    move(topLeftY[id] + 3, topLeftX[id]);
    printw("+----------+");

    // progress is cleared so

    bool waitingForForks = false;
    if(state == Thinking) {
        attron(COLOR_PAIR(4));

    } else if(state == Eating) {
        attron(COLOR_PAIR(5));
    } else {
        waitingForForks = true;
    }

    if(waitingForForks) {
        move(topLeftY[id] + 2, topLeftX[id] + 1);
        printw("Waiting..");

    } else {

        for(int i = 0; i < progress ; i++) {
            move(topLeftY[id] + 2, topLeftX[id] + 1 + i);
            addch('x');
        }
    }

}


