#include "threadpool.h"
#include <iostream>
#include <string.h>
#include <string>

using namespace std;

ThreadPool::ThreadPool(int min, int max) {
    //ʵ�����������
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
        //���������߳�
        threadManager = thread(manager, this);
       
        //���������߳�
        threadIDs.resize(max);
        for (int i = 0; i < min; ++i) {
            threadIDs[i] = thread(worker, this);
        }
        return;
    } while (0);

    //����ʧ�ܣ�δ��31���˳��ͷ���Դ
    if (taskQ) {
        delete taskQ;
        taskQ = nullptr;
    }
}

ThreadPool::~ThreadPool() {
    //�ر��̳߳�
    shutdown = true;

    //�������չ������߳�
    if (threadManager.joinable()) {
        threadManager.join();
    }
    //�����������������߳�
    condPool.notify_all();
    for (int i = 0; i < maxNum; ++i) {
        if(threadIDs[i].joinable()){
            threadIDs[i].join();
        }
    }
    //�ͷ��ڴ�
    if (taskQ) {
        delete taskQ;
        taskQ = nullptr;
    }
}

void ThreadPool::worker(void* arg) {
    //�˴������߳��д���worker������thisָ�룬����void*arg �βδ���
    //��thisָ������ת��Ϊthreadpool���� Ҳ��argתΪthreadpool
    ThreadPool* pool = static_cast<ThreadPool*>(arg);

    while (true) {
        unique_lock<mutex> unlk(pool->mutexPool);
        while (pool->taskQ->taskNumber() == 0 && !pool->shutdown) {
            //���������߳�
            pool->condPool.wait(unlk);
            //�ж��Ƿ�Ҫ�����߳�
            if (pool->exitNum > 0) {
                pool->exitNum--;
                if (pool->liveNum > pool->minNum) {
                    pool->liveNum--;
                    unlk.unlock();
                    return;
                }
            }
        }

        //�ж��̳߳��Ƿ񱻹ر�
        if (pool->shutdown) {
            return;
        }

        //���������ȡ����
        Task task = pool->taskQ->takeTask();
        //�ƶ�ͷ�ڵ� ����
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
        //ÿ3����һ��
        int queueSize = 0;
        int liveNum = 0;
        int busyNum = 0;
        this_thread::sleep_for(chrono::seconds(3));

        //ȡ���̳߳��� ���������͵�ǰ�̵߳�����
        unique_lock<mutex> unlk(pool->mutexPool);
        queueSize = pool->taskQ->taskNumber();
        liveNum = pool->liveNum;
        busyNum = pool->busyNum;
        unlk.unlock();

        //����߳�
        //����ĸ���>����̸߳��� && �����߳���<����߳���
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

        //�����߳�
        //æ���߳�*2 < �����߳� && �����߳�>��С�߳���s
        if (pool->busyNum * 2 < liveNum && liveNum > pool->minNum) {
            unlk.lock();
            //ÿ�� ��Ҫ����NUMBER=2���߳�
            pool->exitNum = NUMBER;
            unlk.unlock();

            //�ù������߳���ɱ
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
    //�������
    taskQ->addTask(task);
    //�����������߳�
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
