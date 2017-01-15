#ifdef PROCESSENGINE_H
#include <iostream>

template <class Type>
ProcessEngine<Type>::ProcessEngine()
: m_numThreads(1), m_bStop(false),m_bStopInProgress(false), m_Threads(),
  m_TaskQ(),m_qMutex(),m_qCondition()
{
}
template <class Type>
ProcessEngine<Type>::~ProcessEngine()
{
    ClearQ();
    StopThreads();
}
template <class Type>
bool ProcessEngine<Type>::Init(size_t nNumThreads)
{
    m_numThreads = nNumThreads;
    m_Threads.reserve(nNumThreads);
    return true;
}
template <class Type>
bool ProcessEngine<Type>::Start()
{
    m_bStop = false;
    for(uint16 i = 0; i < m_numThreads; ++i) {
        std::thread* pThread = new std::thread(&ProcessEngine::ThreadFunc, this);
        if (pThread)
            m_Threads.push_back(pThread);
    }
    return (m_Threads.size() > 0) ? true : false;
}
template <class Type>
void ProcessEngine<Type>::ThreadFunc(void)
{
    ProcessQ();
}
template <class Type>
void ProcessEngine<Type>::ProcessQ()
{
    std::unique_lock<std::mutex> qmutex(m_qMutex);
    TaskQ& queue = m_TaskQ;

    while(!m_bStop) {
        if (queue.size() > 0) {
            Task* pTask = queue.front();
            queue.pop();
            if (pTask) {
                qmutex.unlock();
                OnNewTask(*pTask);
                delete pTask;
                qmutex.lock();
            }
        }
        else {
            auto now = std::chrono::steady_clock::now();
            m_qCondition.wait_until(qmutex, now + std::chrono::seconds(1),
                                    [&queue]() {return !queue.empty();});
        }
    }
}

template <class Type>
bool ProcessEngine<Type>::Stop(size_t waitInMilliSecs)
{
    m_bStopInProgress = true;   // stops accepting any new tasks
    if (waitInMilliSecs)
        std::this_thread::sleep_for(std::chrono::milliseconds(waitInMilliSecs));

    ClearQ();
    StopThreads();
    return true;
}
template <class Type>
void ProcessEngine<Type>::ClearQ()
{
    std::lock_guard<std::mutex> guard(m_qMutex);
    while (m_TaskQ.size() > 0) {
        Task* pTask = m_TaskQ.front();
        m_TaskQ.pop();
        if (pTask) delete pTask;
    }
}
template <class Type>
void ProcessEngine<Type>::StopThreads()
{
    m_bStop = true;
    for(size_t i = 0; i < m_Threads.size(); ++i) {
        std::thread* pThread = m_Threads[i];
        if (pThread->joinable()) {
            pThread->join();
            delete pThread;
        }
    }
    m_Threads.clear();
}
template <class Type>
bool ProcessEngine<Type>::PostTask(Task* pTask)
{
    if (m_bStopInProgress || m_bStop || pTask == nullptr) return false;
    std::unique_lock<std::mutex> qmutex(m_qMutex);
    m_TaskQ.push(pTask);
    qmutex.unlock();
    m_qCondition.notify_all();
    return true;
}
#endif
