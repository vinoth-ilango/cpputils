#include "Utils.h"
#include "ProcessEngine.h"
#include "ObjectPool.h"
#include "TimeUtil.h"
#include <iostream>
#include <boost/lexical_cast.hpp>

class ProcessEngineTest : public ProcessEngine<ProcessEngineTest>
{
public:
    void ProcessTask(const Task& task) {
        std::cout << task.GetType() << std::endl;
    }
};

void TestProcessEngine() {
    ProcessEngineTest test;
    test.Init(2);
    test.Start();

    for(size_t i = 0; i < 10; ++i) {
        Task* pTask = new Task(i);
        test.PostTask(pTask);
    }
    test.Stop();
}

class TestObject : public Poolable<TestObject>{
public:
    void Init() {
        std::cout << "Inside Init(" << GetIndex() << ")" << std::endl;
    }
    void Cleanup() {
        std::cout << "Inside cleanup(" << GetIndex() << ")" << std::endl;
    }
    static void PoolLoggerFunc(const std::string& strLog) {
        std::cout << strLog << std::endl;
    }
};

typedef ObjectPool<TestObject> TestObjectPool;


void MyTestObjectPool() {
    TestObjectPool testObjectPool(TestObject::PoolLoggerFunc);
    testObjectPool.CreateObjects(10);
    std::vector<TestObject*> vecTestObjs;

    for(int i = 0; i < 100; ++i) {
        TestObject* obj = testObjectPool.Acquire();
        vecTestObjs.push_back(obj);
    }
    TestObject* obj = testObjectPool.Acquire();
    testObjectPool.Release(*obj);

    for(size_t i = 0; i < vecTestObjs.size(); ++i) {
        testObjectPool.Release(*vecTestObjs[i]);
    }

    testObjectPool.FreeObjects();
}


int main(int argc, char *argv[])
{
    // MyTestObjectPool();
    // TestProcessEngine();
    /*
    profiletime startTime = TimeUtil::GetProfileTime();
    std::chrono::high_resolution_clock::duration dur = startTime.time_since_epoch();
    int64 secs = std::chrono::duration_cast<std::chrono::seconds>(dur).count();
    int64 nMilliSecs = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    int64 milliSecs = nMilliSecs % secs;

    std::cout << "secs " << secs << " milliSecs" << nMilliSecs << std::endl;
    std::cout << "msecs" << milliSecs << std::endl;

    std::cout << "log time " << TimeUtil::GetLogTime() << std::endl;
    */

    std::string str = "12234x";
    try {
        std::cout << boost::lexical_cast<int>(str) << std::endl;
    }
    catch(const boost::bad_lexical_cast& e ) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
