#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

#include "fork.h"


enum PhilosopherState {
    Thinking, Eating, Waiting, LeftForkAquired, RightForkAquired
};

class Philosopher
{
private:
    int identifier;
    volatile bool isRunning;

    Fork* leftFork;
    Fork* rightFork;

    PhilosopherState state;
    int currentProgress; // out of 10

    // actual time times 10
    int randomThinkingStepTime();

    // actual time times 10
    int randomEatingStepTime();

    void wait10Times(int stepTime);
public:
    Philosopher(int identifier, Fork* leftFork, Fork* rightFork);

    PhilosopherState getState() { return this->state; }

    int getCurrentProgress() { return this->currentProgress; }

    void lifeCycle();

    void stop();
};

#endif // PHILOSOPHER_H
