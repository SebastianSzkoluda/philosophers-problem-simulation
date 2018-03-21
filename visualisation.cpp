#include "visualisation.h"
#include "ncurses.h"
#include <iostream>
#include <string>

Visualisation::Visualisation()
{
    initscr();
    noecho();
    curs_set(0);

    initColors();
}

void Visualisation::initColors() {
    start_color();

    init_pair(1, COLOR_WHITE, COLOR_BLACK);

    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);

    init_pair(4, COLOR_BLUE, COLOR_BLUE);
    init_pair(5, COLOR_YELLOW, COLOR_YELLOW);

    init_pair(6, COLOR_CYAN, COLOR_BLACK);

    bkgd(COLOR_PAIR(1));
}

void Visualisation::drawLegend() {

    attron(COLOR_PAIR(6));
    move(0,1);
    printw("Legend: ");

    attron(COLOR_PAIR(4));
    move(2,3);
    addch(' ');
    attron(COLOR_PAIR(1));
    printw(" Thinking progress");

    attron(COLOR_PAIR(5));
    move(3,3);
    addch(' ');
    attron(COLOR_PAIR(1));
    printw(" Eating progress");

    attron(COLOR_PAIR(3));
    move(4,3);
    addch('Y');
    attron(COLOR_PAIR(1));
    printw(" Fork in use");

    attron(COLOR_PAIR(2));
    move(5,3);
    addch('Y');
    attron(COLOR_PAIR(1));
    printw(" Fork free");

    move(6,1);
    attron(COLOR_PAIR(1));
    printw("\\o/ Philo with hands");


    attron(COLOR_PAIR(6));
    move(0,30);
    printw("Visualisation: ");

}

void Visualisation::drawTimeRangePanel() {

    attron(COLOR_PAIR(6));
    move(16,1);
    printw("Configuration: ");

    attron(COLOR_PAIR(1));
    move(18,1);
    printw("Single operation time range: ");
    move(19,1);
    printw("[");
    printw(std::to_string(Philosopher::randomnessBase).c_str());
    printw(", ");
    printw(std::to_string(Philosopher::randomnessRange + Philosopher::randomnessBase).c_str());
    printw(") ms");
    move(20,1);
    printw("'z', 'x', 'c', 'v'");

}

Visualisation::~Visualisation(){
    endwin();
}

void Visualisation::start(Fork** forks, Philosopher** philosophers) {

    drawLegend();

    // main loop
    int ch;
    nodelay(stdscr, TRUE);
    for (;;) {
        if ((ch = getch()) != 27) {
            // handle configuration keys
            if(ch == 'z' && Philosopher::randomnessBase >= 310) {
                Philosopher::randomnessBase -= 10;
            } else if(ch == 'x') {
                Philosopher::randomnessBase += 10;
            } else if(ch == 'c' && Philosopher::randomnessRange >= 710) {
                Philosopher::randomnessRange -= 10;
            } else if( ch == 'v') {
                Philosopher::randomnessRange += 10;
            }

            drawTimeRangePanel();

            for(int i = 0 ; i < 5; i++) {
                PhilosopherState state = philosophers[i]->getState();
                if(state == LeftForkAquired) {
                    drawPhilosopher(i, true, false);
                } else if(state == RightForkAquired) {
                    drawPhilosopher(i, false, true);

                } else if(state == Eating) {
                    drawPhilosopher(i, true, true);

                } else {
                    drawPhilosopher(i, false, false);
                }

                drawPhilosopherDetails(i, philosophers[i]->getCurrentProgress(), state);
            }

            for(int i = 0 ; i < 5; i++) {
                bool inUse = forks[i]->isInUse();

                drawFork(i, inUse);
            }

            timeout(125);
        }
        else {
            return;
        }

    }
}

void Visualisation::drawFork(int id, bool inUse) {
    const int forksY[] = {7, 13, 16, 13, 7};
    const int forksX[] = {53, 51, 56, 61, 59};
    const char forkSymbol = 'Y';

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

    const char noHand = ' ';

    const char leftHands[] = {'/', '\\','_','|','\\'};
    const char rightHands[] = {'|', '_', '/', '\\', '/'};

    const int leftHandsY[] = {9, 15, 16, 11, 6};
    const int leftHandsX[] = {51, 52, 58, 61, 57};

    const int rightHandsY[] = {11, 16, 15, 9, 6};
    const int rightHandsX[] = {51, 54, 60, 61, 55};

    const int headsY[] = {10, 16, 16, 10, 6};
    const int headsX[] = {51, 53, 59, 61, 56};

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
    addch('O');


}

void Visualisation::drawPhilosopherDetails(int id, int progress, PhilosopherState state) {
    const int topLeftY[] = {9, 18, 18, 9, 1};
    const int topLeftX[] = {35, 40, 60, 65, 50};


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


