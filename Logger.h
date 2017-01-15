#ifndef LOGGER_H
#define LOGGER_H

#include "Datatypes.h"
#include "ProcessEngine.h"
#include <sstream>
#include <thread>
#include <fstream>

enum class LogLevel {        
    DEBUG = 0,
    INFO,
    WARN,
    ERROR,
    FATAL
};
class Logger : public ProcessEngine<Logger>
{
public:
    static Logger& GetInstance();
    void Init(const std::string& strLogFilePrefix, LogLevel level = LogLevel::DEBUG);
    void ProcessTask(const Task& task);
    bool IsLogLevelEnabled(const LogLevel level);
    void WriteLog(LogLevel level, const std::string& strMessage);
    static const int32 LOG_TASK = 0;
private:
    LogLevel      m_logLevel;
    std::ofstream m_logFile;
    Logger();
    ~Logger();
};

inline
Logger& Logger::GetInstance()
{
    static Logger s_instance; return s_instance;
}
inline
void Logger::WriteLog(LogLevel level, const std::string &strMessage)
{
    LogLevel* pLevel = new LogLevel();
    *pLevel          = level;
    std::string* pString = new std::string(strMessage);
    Task* pTask = new Task(LOG_TASK, pLevel, pString);
    ProcessEngine::PostTask(pTask);
}
inline
bool Logger::IsLogLevelEnabled(const LogLevel level) {
    return (level >= m_logLevel) ? true : false;
}

class LogRecord
{
public:
    LogRecord(LogLevel nLogLevel, const std::string& /*strFile*/, int /*nLine*/)
        : m_LogLevel(nLogLevel), m_oss() {
    }
    ~LogRecord() {
        Logger::GetInstance().WriteLog(m_LogLevel, m_oss.str());
    }
    template <typename T>
    LogRecord& operator<<(const T& val) {
        m_oss << val;
        return *this;
    }
    LogRecord& operator<<(const double& dbl){
        m_oss << std::fixed;        // gives 6 decimal places precision
        // m_oss.precision(10);     // gives 10 decimal places precision
        m_oss << dbl;
        return *this;
    }
private:
    LogLevel           m_LogLevel;
    std::ostringstream m_oss;
};

#define mylog(level, filename, linenum) \
    if (Logger::GetInstance().IsLogLevelEnabled(level)) LogRecord(level, filename, linenum)

#define ldebug mylog(LogLevel::DEBUG, __FILE__, __LINE__)
#define linfo  mylog(LogLevel::INFO,  __FILE__, __LINE__)
#define lwarn  mylog(LogLevel::WARN,  __FILE__, __LINE__)
#define lerror mylog(LogLevel::ERROR, __FILE__, __LINE__)

#endif // LOGGER_H
