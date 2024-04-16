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

    //���̳߳��������
    void CommitTask(Task task);

    int getBusyNum();

    int getAliveNum();

private:
    //�������
    TaskQueue* taskQ;

    std::thread threadManager; //�������߳�(�����̳߳��е��߳� ��������
                               //�������������߳�)
    std::vector<thread> threadIDs; //�����߳� ���̳߳��й������̣߳�
    int minNum;                    //��С�߳���
    int maxNum;                    //����߳���
    int busyNum;                   //æ���߳���
    int liveNum;                   //�����߳���
    int exitNum;                   //Ҫ���ٵ��߳���

    std::mutex mutexPool;             //�������̳߳�
    std::condition_variable condPool; //��������ǲ��ǿ�

    bool shutdown; //�ǲ���Ҫ�����̳߳� 1���� 0������
    static const int NUMBER = 0;

private:
    //�����̵߳�������
    static void worker(void* arg);
    //�������̵߳�������
    static void manager(void* arg);
};
