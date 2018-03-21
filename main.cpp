#include <iostream>
#include <thread>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <mutex>
#include <ncurses.h>

#include "visualisation.h"
#include "philosopher.h"
#include "fork.h"

const int numberOfPhilosophers = 5;
const int numberOfForks = 5;

Fork** createForks() {
    Fork** forks = new Fork*[numberOfForks];
    for(int i = 0 ; i < numberOfForks ; i++) {
      forks[i] = new Fork(i);
    }
    return forks;
}

Philosopher** createPhilosophers(Fork** forks) {
    Philosopher** philosophers = new Philosopher*[numberOfForks];
    for(int i = 0 ; i < numberOfForks ; i++) {
      Fork* rightFork;
      if(i == numberOfPhilosophers - 1) {
          rightFork = forks[0];
      } else {
          rightFork = forks[i+1];
      }

      philosophers[i] = new Philosopher(i, forks[i], rightFork);
    }
    return philosophers;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    Fork** forks = createForks();
    Philosopher** philosophers = createPhilosophers(forks);

    std::thread* threads = new std::thread[numberOfPhilosophers];
    for(int i = 0 ; i < numberOfPhilosophers ; i++) {
        threads[i] = std::thread(&Philosopher::lifeCycle, philosophers[i]);
    }

    Visualisation visualisation = Visualisation();
    visualisation.start(forks, philosophers);

    // after ESC was pressed
    for(int i = 0 ; i < numberOfPhilosophers ; i++) {
        philosophers[i]->stop();
    }

    for(int i = 0 ; i < numberOfPhilosophers; i++) {
        threads[i].join();
    }


    // clean up
    for(int i = 0 ; i < numberOfPhilosophers ; i++) {
        delete philosophers[i];
    }
    delete[] philosophers;

    delete[] threads;

    for(int i = 0 ; i < numberOfPhilosophers ; i++) {
      delete forks[i];
    }
    delete[] forks;

    return 0;
}
