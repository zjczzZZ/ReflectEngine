#pragma once
#include "TaskQueue.h"
#include <chrono>
#include <condition_variable>
#include <thread>
#include <vector>

class ThreadPool {
public:
    ThreadPool(int min, int max);
    ~ThreadPool();

    //给线程池添加任务
    void CommitTask(Task task);

    int getBusyNum();

    int getAliveNum();

private:
    //任务队列
    TaskQueue* taskQ;

    std::thread threadManager; //管理者线程(管理线程池中的线程 可扩可缩
                               //来任务唤醒阻塞线程)
    std::vector<thread> threadIDs; //工作线程 （线程池中工作的线程）
    int minNum;                    //最小线程数
    int maxNum;                    //最大线程数
    int busyNum;                   //忙的线程数
    int liveNum;                   //存活的线程数
    int exitNum;                   //要销毁的线程数

    std::mutex mutexPool;             //锁整个线程池
    std::condition_variable condPool; //任务队列是不是空

    bool shutdown; //是不是要销毁线程池 1销毁 0不销毁
    static const int NUMBER = 0;

private:
    //工作线程的任务函数
    static void worker(void* arg);
    //管理者线程的任务函数
    static void manager(void* arg);
};
