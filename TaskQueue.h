#pragma once
#include <mutex>
#include <queue>
#include <thread>
using namespace std;

using callback = void (*)(void* arg);

struct Task {
    Task()
        : function(nullptr), arg(nullptr) {}

    Task(callback f, void* arg)
        : function(f), arg(arg) {}

    callback function;
    void* arg;
};

class TaskQueue {
private:
    std::mutex m_mutex;
    std::queue<Task> m_taskQ;

public:
    TaskQueue();
    ~TaskQueue();

    // 添加任务
    void addTask(Task task);
    void addTask(callback f, void* arg);
    // 取出一个任务
    Task takeTask();

    // 获取当前任务个数
    inline size_t taskNumber() {
        return m_taskQ.size();
    }
};
