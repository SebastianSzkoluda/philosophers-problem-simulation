#ifndef VISUALISATION_H
#define VISUALISATION_H

#include "philosopher.h"
#include "fork.h"

class Visualisation
{
private:
    int rows, columns;

    void drawPhilosopher(int id, bool usingLeft, bool usingRight);
    void drawPhilosopherDetails(int id, int progress, PhilosopherState state);
    void drawFork(int id, bool inUse);

public:
    Visualisation();
    ~Visualisation();

    void start(Fork** forks, Philosopher** philosophers);

};

#endif // VISUALISATION_H
