#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <vector>
#include <mutex>
#include <iostream>
#include <sstream>

template <typename Type>
class Poolable
{
public:
    size_t GetIndex() const       {return m_Index; }
    void   SetIndex(size_t index) {m_Index = index;}
    void InitObj()    {static_cast<Type*>(this)->Init();}
    void CleanupObj() {static_cast<Type*>(this)->Cleanup();}
private:
    size_t m_Index = 0;
};

typedef void (*LogFunc) (const std::string&);

template <typename Type>
class ObjectPool
{
public:
    ObjectPool (LogFunc loggerFunc = nullptr);
    ~ObjectPool();
    bool CreateObjects(size_t numObjects);
    void FreeObjects(void);
    Type* Acquire();
    void  Release(Type& relObj);

private:
    size_t             m_firstUnusedObjIndex;
    LogFunc            m_LogFunc;
    std::mutex         m_poolMutex;
    std::vector<Type*> m_vecObjects;

    void CreateObjectPriv(size_t numObjects);
    void OnError(const std::string& strMessage);
};


template <class Type> inline
ObjectPool<Type>::ObjectPool(LogFunc loggerFunc)
: m_firstUnusedObjIndex(0), m_LogFunc(loggerFunc), m_poolMutex(), m_vecObjects()
{}

template <class Type> inline
ObjectPool<Type>::~ObjectPool()
{
    FreeObjects();
}
template <class Type>
bool ObjectPool<Type>::CreateObjects(size_t numObjects)
{
    std::lock_guard<std::mutex> lock(m_poolMutex);
    if (m_vecObjects.empty()) CreateObjectPriv(numObjects);
    if (!m_vecObjects.empty()) {
        m_firstUnusedObjIndex = 0;
        return true;
    }
    return false;
}
template <class Type>
void ObjectPool<Type>::CreateObjectPriv(size_t numObjects)
{
    std::stringstream ss;
    ss << "Info: ObjectPool creating " << numObjects << " new objects";
    OnError(ss.str());

    size_t newNumObjects = m_vecObjects.size() + numObjects;
    try {
        for (size_t i = m_vecObjects.size(); i < newNumObjects; ++i) {
            Type* pType = new Type();
            pType->SetIndex(i);
            m_vecObjects.push_back(pType);
        }
    }
    catch(...) {
        OnError("Err: ObjectPool, exception when creating new objs...");
    }
}
template <class Type>
void ObjectPool<Type>::FreeObjects()
{
    std::lock_guard<std::mutex> lock(m_poolMutex);
    for(size_t i = 0; i < m_vecObjects.size(); ++i) {
        delete m_vecObjects[i];
    }
    m_vecObjects.clear();
}
template <class Type> inline
Type* ObjectPool<Type>::Acquire()
{
    Type* retObj = nullptr;
    {
        std::lock_guard<std::mutex> lock(m_poolMutex);
        size_t poolSize = m_vecObjects.size();
        if (m_firstUnusedObjIndex >= poolSize) {
            size_t newPoolSize = size_t(poolSize * 1.5);
            CreateObjectPriv(newPoolSize - poolSize);
            poolSize = m_vecObjects.size();
        }
        if ((poolSize != 0) && (m_firstUnusedObjIndex < poolSize)) {
            retObj = m_vecObjects[m_firstUnusedObjIndex];
            ++m_firstUnusedObjIndex;
        }
    }
    if (retObj) retObj->InitObj();
    else OnError("Err: ObjectPool, error full(no free objects in pool)");
    return retObj;
}

template <class Type> inline
void ObjectPool<Type>::Release(Type& relObj)
{
    size_t relIndex = relObj.GetIndex();
    relObj.CleanupObj();

    std::lock_guard<std::mutex> lock(m_poolMutex);
    if (m_firstUnusedObjIndex > 0) --m_firstUnusedObjIndex;
    else {
        OnError("Err: ObjectPool, Release for the same obj is called more than once");
        return;
    }
    // swap relObj and topUsedObj
    size_t topUsedObjIndex = m_firstUnusedObjIndex;
    Type* temp = m_vecObjects[topUsedObjIndex];
    m_vecObjects[topUsedObjIndex] = m_vecObjects[relIndex];
    m_vecObjects[relIndex]        = temp;

    // Interchange the index
    m_vecObjects[topUsedObjIndex]->SetIndex(topUsedObjIndex);
    m_vecObjects[relIndex]->SetIndex(relIndex);
}
template <class Type> inline
void ObjectPool<Type>::OnError(const std::string& strMessage)
{
    if (m_LogFunc) m_LogFunc(strMessage);
    else std::cout << strMessage << std::endl;
}

#endif // OBJECTPOOL_H

