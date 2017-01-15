#ifndef PROCESSENGINE_H
#define PROCESSENGINE_H

#include "Datatypes.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

class Task
{
    template <class Type>
    friend class ProcessEngine;
private:
    size_t m_Type;
    void*  m_PayLoad1;
    void*  m_PayLoad2;
    ~Task() {}  // To make sure, Task obj can be created only in heap.
public:
    explicit Task(size_t type, void* payLoad1 = 0, void* payLoad2 = 0)
            : m_Type(type), m_PayLoad1(payLoad1), m_PayLoad2(payLoad2){}
    size_t GetType()     const {return m_Type;}
    void*  GetPayLoad1() const {return m_PayLoad1;}
    void*  GetPayLoad2() const {return m_PayLoad2;}
};

typedef std::queue<Task*> TaskQ;
typedef std::vector<std::thread*> Threads;

template <class Type>
class ProcessEngine
{
public:
    ProcessEngine();
    ~ProcessEngine();

    bool Init(size_t nNumThreads = 1);
    bool Start();
    bool Stop(size_t waitInMilliSecs = 1000);

    bool PostTask(Task* pTask);
    void OnNewTask(const Task& task) {
        static_cast<Type*>(this)->ProcessTask(task);
    }

private:
    void ThreadFunc(void);
    void ProcessQ();
    void ClearQ();
    void StopThreads();

    size_t     m_numThreads;
    bool       m_bStop;
    bool       m_bStopInProgress;
    Threads    m_Threads;
    TaskQ      m_TaskQ;
    std::mutex m_qMutex;
    std::condition_variable m_qCondition;
};

#include "ProcessEngine.cpp"
#endif // PROCESSENGINE_H
