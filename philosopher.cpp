#include "philosopher.h"
#include <iostream>
#include <thread>
#include <chrono>

int Philosopher::randomnessRange = 2000;
int Philosopher::randomnessBase = 2000;

Philosopher::Philosopher(int identifier, Fork* leftFork, Fork* rightFork) : identifier(identifier),
    leftFork(leftFork), rightFork(rightFork), isRunning(true), currentProgress(0)
{
}

int Philosopher::randomThinkingStepTime() {
    return (randomnessBase + rand() % randomnessRange)/10;
}


int Philosopher::randomEatingStepTime() {
    return (randomnessBase + rand() % randomnessRange)/10;
}


/**
 * @brief Philosopher::wait10Times sleeps for 10x specified time while incrementing current progress after each step
 * @param stepTime time of each step in miliseconds
 */
void Philosopher::wait10Times(int stepTime) {
    for(currentProgress = 0; currentProgress < 10 && this->isRunning ; currentProgress++) {
        std::this_thread::sleep_for (std::chrono::milliseconds(stepTime));
    }
    currentProgress = 0;
}


void Philosopher::lifeCycle() {

    while(this->isRunning) {

        // thinking
        int thinkingStepTime = randomThinkingStepTime();
        wait10Times(thinkingStepTime);

        // finished thinking - will wait for forks
        this->state = Waiting;

        takeForks();

        // forks aquired, eataing
        this->state = Eating;

        int eatingStepTime = randomEatingStepTime();
        wait10Times(eatingStepTime);

        releaseForks();

        this->state = Thinking;

    }

}

/**
 * @brief Take fork of lower id first to prevent deadlock.
 */
void Philosopher::takeForks() {
    if(this->leftFork->getId() < this->rightFork->getId()) {
        // get leftFork first
        this->leftFork->take();
        this->state = LeftForkAquired;

        this->rightFork->take();

    } else {
        // get rightFork first
        this->rightFork->take();
        this->state = RightForkAquired;

        this->leftFork->take();
    }
}

/**
 * @brief Release fork of lower id first to prevent deadlock.
 */
void Philosopher::releaseForks() {
    if(this->leftFork->getId() < this->rightFork->getId()) {
        // release right fork first
        this->rightFork->release();
        this->state = LeftForkAquired;
        this->leftFork->release();

    } else {
        // release left fork first
        this->leftFork->release();
        this->state = RightForkAquired;
        this->rightFork->release();
    }
}

void Philosopher::stop() {
    this->isRunning = false;
}
