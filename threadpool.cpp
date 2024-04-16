#include "threadpool.h"
#include <iostream>
#include <string.h>
#include <string>

using namespace std;

ThreadPool::ThreadPool(int min, int max) {
    //实例化任务队列
    do {
        minNum = min;
        maxNum = max;
        busyNum = 0;
        liveNum = min;
        exitNum = 0;
        shutdown = false;

        taskQ = new TaskQueue;
        if (taskQ == nullptr) {
            cout << "malloc taskQ false" << endl;
            break;
        }
        //创建管理线程
        threadManager = thread(manager, this);
       
        //创建工作线程
        threadIDs.resize(max);
        for (int i = 0; i < min; ++i) {
            threadIDs[i] = thread(worker, this);
        }
        return;
    } while (0);

    //分配失败，未从31行退出释放资源
    if (taskQ) {
        delete taskQ;
        taskQ = nullptr;
    }
}

ThreadPool::~ThreadPool() {
    //关闭线程池
    shutdown = true;

    //阻塞回收管理者线程
    if (threadManager.joinable()) {
        threadManager.join();
    }
    //唤醒阻塞的消费者线程
    condPool.notify_all();
    for (int i = 0; i < maxNum; ++i) {
        if(threadIDs[i].joinable()){
            threadIDs[i].join();
        }
    }
    //释放内存
    if (taskQ) {
        delete taskQ;
        taskQ = nullptr;
    }
}

void ThreadPool::worker(void* arg) {
    //此处，将线程中传入worker函数的this指针，即由void*arg 形参代换
    //将this指针类型转换为threadpool类型 也即arg转为threadpool
    ThreadPool* pool = static_cast<ThreadPool*>(arg);

    while (true) {
        unique_lock<mutex> unlk(pool->mutexPool);
        while (pool->taskQ->taskNumber() == 0 && !pool->shutdown) {
            //阻塞工作线程
            pool->condPool.wait(unlk);
            //判断是否要销毁线程
            if (pool->exitNum > 0) {
                pool->exitNum--;
                if (pool->liveNum > pool->minNum) {
                    pool->liveNum--;
                    unlk.unlock();
                    return;
                }
            }
        }

        //判断线程池是否被关闭
        if (pool->shutdown) {
            return;
        }

        //任务队列中取任务
        Task task = pool->taskQ->takeTask();
        //移动头节点 解锁
        pool->busyNum++;
        unlk.unlock();

        task.function(task.arg);

        unlk.lock();
        pool->busyNum--;
        unlk.unlock();
    }
}


void ThreadPool::manager(void* arg) {
    ThreadPool* pool = static_cast<ThreadPool*>(arg);

    while (!pool->shutdown) {
        //每3秒检测一次
        int queueSize = 0;
        int liveNum = 0;
        int busyNum = 0;
        this_thread::sleep_for(chrono::seconds(3));

        //取出线程池中 任务数量和当前线程的数量
        unique_lock<mutex> unlk(pool->mutexPool);
        queueSize = pool->taskQ->taskNumber();
        liveNum = pool->liveNum;
        busyNum = pool->busyNum;
        unlk.unlock();

        //添加线程
        //任务的个数>存活线程个数 && 存活的线程数<最大线程数
        if (queueSize > liveNum && liveNum < pool->maxNum) {
            unlk.lock();
            int counter = 0;
            for (int i = 0; i < pool->maxNum && counter < NUMBER &&
                pool->liveNum < pool->maxNum;
                ++i) {
                if (pool->threadIDs[i].get_id() == thread::id()) {
                    pool->threadIDs[i] = thread(worker, pool);
                    ++counter;
                    pool->liveNum++;
                }
            }
            unlk.unlock();
        }

        //销毁线程
        //忙的线程*2 < 存活的线程 && 存活的线程>最小线程数s
        if (pool->busyNum * 2 < liveNum && liveNum > pool->minNum) {
            unlk.lock();
            //每次 想要销毁NUMBER=2个线程
            pool->exitNum = NUMBER;
            unlk.unlock();

            //让工作的线程自杀
            for (int i = 0; i < NUMBER; ++i) {
                pool->condPool.notify_one();
            }
        }
    }
}

void ThreadPool::CommitTask(Task task) {
    if (shutdown) {
        return;
    }
    //添加任务
    taskQ->addTask(task);
    //唤醒阻塞的线程
    condPool.notify_one();
    
}

int ThreadPool::getAliveNum() {
    int AliveNum = 0;
    mutexPool.lock();
    AliveNum = this->liveNum;
    mutexPool.unlock();
    return AliveNum;
}

int ThreadPool::getBusyNum() {
    int nusyNum = 0;
    mutexPool.lock();
    busyNum = this->busyNum;
    mutexPool.unlock();
    return busyNum;
}
