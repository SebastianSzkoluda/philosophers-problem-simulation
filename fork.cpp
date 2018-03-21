#include "fork.h"
#include <thread>

Fork::Fork(int id) : id(id), inUse(false) { }

void Fork::take() {
    std::unique_lock<std::mutex> l(this->_lock);

    this->_cvInUse.wait(l, [this](){return this->inUse == false;});

    this->inUse = true;

    l.unlock();
}

void Fork::release() {
    std::unique_lock<std::mutex> l(this->_lock);

    this->inUse = false;

    l.unlock();
    this->_cvInUse.notify_one();
}


bool Fork::isInUse() {
    return inUse;
}

int Fork::getId() {
    return this->id;
}
