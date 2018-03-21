#include "philosopher.h"
#include <iostream>
#include <thread>
#include <chrono>

Philosopher::Philosopher(int identifier, Fork* leftFork, Fork* rightFork) : identifier(identifier), leftFork(leftFork), rightFork(rightFork),
    isRunning(true), currentProgress(0)
{
}

int Philosopher::randomThinkingStepTime() {
    //return (300 + rand() % 400)/10;
    return (300 + rand() % 2000)/10;
}


int Philosopher::randomEatingStepTime() {
    //return (300 + rand() % 400)/10;
    return (300 + rand() % 2000)/10;
}


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

        this->state = Waiting;

        if(this->leftFork->getId() < this->rightFork->getId()) {
            // get leftFork first

            this->leftFork->take();
            this->state = LeftForkAquired;

            this->rightFork->take();
            this->state = Eating;

            // eateing
            int eatingStepTime = randomEatingStepTime();
            wait10Times(eatingStepTime);

            this->rightFork->release();
            this->state = LeftForkAquired;
            this->leftFork->release();
            this->state = Thinking;

        } else {
            // get rightFork first

            this->rightFork->take();
            this->state = RightForkAquired;

            this->leftFork->take();
            this->state = Eating;

            // eating
            int eatingStepTime = randomEatingStepTime();
            wait10Times(eatingStepTime);

            this->leftFork->release();
            this->state = RightForkAquired;
            this->rightFork->release();
            this->state = Thinking;
        }



    }

}

void Philosopher::stop() {
    this->isRunning = false;
}
