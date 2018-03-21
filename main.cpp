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


int lifetimeInMeals = 3;

void philosopher_routine(int referenceId, Fork* leftFork, Fork* rightFork) {
    int mealsEaten = 0;

    while(mealsEaten < lifetimeInMeals) {
        int millisecondsOfThinking = 300 + rand() % 400;
        std::cout << "Philosopher " << referenceId << ": I will be thinking for " << millisecondsOfThinking << "ms" << std::endl;
        std::this_thread::sleep_for (std::chrono::milliseconds(millisecondsOfThinking));

        leftFork->take();
        std::cout << "Philosopher " << referenceId << ": I have aquired my left fork" << std::endl;
        rightFork->take();

        int milisecondsOfEating = 300 + rand() % 400;
        std::cout << "Philosopher " << referenceId << ": I have aquired both forks, will be eating for " << milisecondsOfEating << "ms" << std::endl;
        std::this_thread::sleep_for (std::chrono::milliseconds(milisecondsOfEating));
        mealsEaten++;
        std::cout << "Philosopher " << referenceId << ": I have eaten " << mealsEaten << " times already" << std::endl;

        leftFork->release();
        rightFork->release();
    }
}

const int numberOfPhilosophers = 5;
const int numberOfForks = 5;

Fork** createForks() {
    Fork** forks = new Fork*[numberOfForks];
    for(int i = 0 ; i < numberOfForks ; i++) {
      forks[i] = new Fork(i);
    }
    return forks;
}

// (born)
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

    for(int i = 0 ; i < numberOfPhilosophers ; i++) {
        philosophers[i]->stop();
    }

    for(int i = 0 ; i < numberOfPhilosophers; i++) {
        threads[i].join();
    }



    //Philosopher philo = Philosopher(0, nullptr, nullptr);
    //std::thread thread = std::thread(philo);
//    thread.join();


//    Visualisation *visualisation = new Visualisation();






//    std::thread* threads = new std::thread[numberOfPhilosophers - 1];
//    for(int i = 0 ; i < numberOfPhilosophers - 1 ; i++) {
//        threads[i] = std::thread(philosopher_routine, i, forks[i], forks[numberOfPhilosophers+i]);
//    }

//    philosopher_routine(numberOfPhilosophers - 1, forks[numberOfPhilosophers-1], forks[2*numberOfPhilosophers-1]);



//    delete[] threads;

//    for(int i = 0 ; i < numberOfPhilosophers * 2 ; i++) {
//      delete forks[i];
//    }
//    delete[] forks;

    return 0;
}
