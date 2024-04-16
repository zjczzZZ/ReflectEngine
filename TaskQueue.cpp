#include "TaskQueue.h"

TaskQueue::TaskQueue() {}

TaskQueue::~TaskQueue() {}

void TaskQueue::addTask(Task task) {
    std::lock_guard<mutex> lk(m_mutex);
    m_taskQ.push(task);
}

Task& TaskQueue::takeTask() {
    Task t;
    std::unique_lock<mutex> ulk(m_mutex);
    if (!m_taskQ.empty()) {
        t = m_taskQ.front();
        m_taskQ.pop();
    }
    ulk.unlock();
    return t;
}
void TaskQueue::addTask(callback f, void* arg) {
    std::lock_guard<mutex> lk(m_mutex);
    m_taskQ.push(Task(f, arg));
}