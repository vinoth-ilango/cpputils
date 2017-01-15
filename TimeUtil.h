#ifndef TIMEUTIL
#define TIMEUTIL

#include <chrono>
#include <ctime>
#include <iomanip>
#include <stdio.h>
#include "Datatypes.h"

typedef std::chrono::high_resolution_clock::time_point profiletime;

class TimeUtil
{
public:
    static std::string GetCurrentDateTime(const std::string& strFormat);
    static std::string GetLogTime();

    static profiletime GetProfileTime();
    static int64       GetDiffInMicroSecs(profiletime start, profiletime end);
    static int64       GetDiffInNanoSecs (profiletime start, profiletime end);
    static int64       GetDiffInMilliSecs(profiletime start, profiletime end);

};
inline
profiletime TimeUtil::GetProfileTime()
{
    return std::chrono::high_resolution_clock::now();
}
inline
int64 TimeUtil::GetDiffInMicroSecs(profiletime start, profiletime end)
{
    std::chrono::high_resolution_clock::duration diff = (end - start);
    return std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
}
inline
int64 TimeUtil::GetDiffInNanoSecs (profiletime start, profiletime end)
{
    std::chrono::high_resolution_clock::duration diff = (end - start);
    return std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count();
}

inline
int64 TimeUtil::GetDiffInMilliSecs(profiletime start, profiletime end)
{
    std::chrono::high_resolution_clock::duration diff = (end - start);
    return std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
}
inline
std::string TimeUtil::GetCurrentDateTime(const std::string& strFormat)
{
    // %Y%m%d %H:%M:%S - 20150820 20:08:20
    // %Y%b%d %H:%M:%S - 2015Aug20 20:08:20
    // %y%b%d %H:%M:%S - 15Aug20 20:08:20

    std::time_t epochTime = std::time(nullptr);
    struct tm   timeInfo;
    char        timeString[64] = {0};
    if(0 == ::localtime_s(&timeInfo, &epochTime)) {
        std::strftime(timeString, sizeof(timeString), strFormat.c_str(), &timeInfo);
    }
    return timeString;
}
inline
std::string TimeUtil::GetLogTime()
{
    std::chrono::high_resolution_clock::duration dur = std::chrono::high_resolution_clock::now().time_since_epoch();
    int64 nsecs      = std::chrono::duration_cast<std::chrono::seconds>(dur).count();
    int64 nMilliSecs = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    struct tm   timeInfo;
    char timeString[32] = {'\0'};
    if (0 == ::localtime_s(&timeInfo, &nsecs)) {
//        sprintf(timeString, "%4d-%02d-%02d %02d:%02d:%02d.%03d",
//              timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
//              timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec, (nMilliSecs % nsecs));

        sprintf(timeString, "%02d:%02d:%02d.%03d",
               timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec, (nMilliSecs % nsecs));
    }
    return timeString;
}

//inline
//std::string TimeUtil::GetCurrentTime(const std::string& strFormat)
//{

////    using namespace std::chrono;
////    high_resolution_clock::time_point currTime = high_resolution_clock::now();
////    std::time_t t = std::time(nullptr);
////    std::tm currTime;
////    std::localtime(&t);
////    std::strftime()

//}



#endif // TIMEUTIL

