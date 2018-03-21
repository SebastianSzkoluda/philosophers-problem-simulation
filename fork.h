#ifndef FORK_H
#define FORK_H

#include <mutex>
#include <atomic>
#include <condition_variable>

class Fork
{
private:
    std::atomic<bool> inUse;
    int id;

    std::mutex _lock;
    std::condition_variable _cvInUse;
public:
    Fork(int id);
    void take();
    void release();
    bool isInUse();

    int getId();

};

#endif // FORK_H
