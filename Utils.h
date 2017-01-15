#ifndef UTILS_H
#define UTILS_H


#include "Datatypes.h"
#include <vector>
#include <string>
#include <sstream>
#include <cmath>    // required for fabs and isnan functions

//! vector of strings
typedef std::vector <std::string> stringvector;
typedef std::vector <int32>       intvector;

class Utils
{
public:

    static void StrTokenize(const std::string& str, char delim, stringvector& tokens);
    static void StrTokenize(const std::string& str, const std::string& delims, stringvector& tokens);    
    static void StrSplit   (const std::string& str, char delimiter, std::string& first, std::string& second);
    static void StrTrim    (std::string& toTrim, const std::string& trimChars = " \t\n");
    static void StrToUpper (std::string& str);

    template <typename Type>
    static bool IsNan(Type x);
    template <typename Type>
    static std::string GetString(const Type& data);
    static std::string GetPricePrecisionString(double data, int precision = 2);

    static void StrFindAndReplace(std::string& str, const std::string& find, const std::string& replace);
    static void StrFindAndReplace(std::string& str, char find, char replace);
    static bool StrEndsWith(const std::string& str, const std::string& strEnd);
    static bool StrContains(const std::string& str, const std::string& strSubString);

    static int32  Str2Int     (const std::string& strInt);
    static double Str2Double  (const std::string& strDbl);
    static int64  Str2Int64   (const std::string& strInt);
    static bool   StrIsNumeric(const std::string& str, uint16 nNumberBase = 10);
    static bool   IsDigit     (const char c);
    static size_t StrHash     (const std::string& str);

    static bool   DoubleEquals (double left, double right, double epsilon = 0.0001);
    static int    DoubleCompare(double left, double right, double epsilon = 0.0001);
    static int64  Double2Int64 (double dbl);
    static double RoundDouble  (double dbl, uint16 numDecimalPlaces = 2); // number of decimal places

    static int64  Price2Int64  (double dblPrice, double dblTickSize, int32 nPriceMultiplier);
    static double Price2Double (int64  lPrice,   double dblTickSize, int32 nPriceMultiplier);
    static double RoundPrice   (double dblPrice, double tickSize); // round to ticksize

    static bool FileExists(const std::string& filePath);
    static bool DeleteFile(const std::string& strFile);
    static bool CopyFile  (const std::string& src, const std::string& dest);
    static bool MakeDir   (const std::string& path);
    static bool MakePath  (const std::string& path);
    static bool DirExists (const std::string& dir);
    static bool GetAllFilesInDir(const std::string& strDir,stringvector& vecfiles,std::string strFileFilter);

    static void WriteToSysLog(const std::string& prefix, int logLevel, const std::string& log);
    static bool IsPrimeNumber(int n);
    static bool IsCharPresent(char c, const std::string& str);
    static void StrTokenizeWithDelims(const std::string& str,stringvector& vec, const std::string& strDelims = "+-*/ ()");
    static void RemoveString(stringvector& vec, const std::string& = " ");

};
// inline functions
inline
int32 Utils::Str2Int(const std::string& strInt)
{
    return std::atoi(strInt.c_str());
}
inline
double Utils::Str2Double(const std::string& strDouble)
{
    return atof(strDouble.c_str());
}
inline
int64 Utils::Str2Int64(const std::string& strInt)
{
    return atoll(strInt.c_str());
}
inline
bool Utils::StrIsNumeric(const std::string& str, uint16 numberBase)
{
    // copied from http://rosettacode.org/wiki/Determine_if_a_string_is_numeric#C.2B.2B
    std::istringstream ss(str);
    if (10 == numberBase) {
        double dValue; ss >> dValue;
    } else if ((8 == numberBase) || (16 == numberBase)) {
        int nValue; ss >> ((8 == numberBase) ? std::oct : std::hex) >> nValue;
    } else {
        return false; // invalid number base
    }
    if (!ss) return false;
    return (ss.rdbuf()->in_avail() == 0);
}
inline
void Utils::StrSplit(const std::string& str, char delimiter, std::string& first, std::string& second)
{
    size_t delimPos = str.find(delimiter);
    if (delimPos   != std::string::npos) {
        first  = str.substr(0, delimPos);
        second = str.substr(delimPos+1);
    }
}
inline
void Utils::StrTrim(std::string& toTrim, const std::string& trimChars)
{
    size_t lstrip = toTrim.find_first_not_of(trimChars);
    size_t rstrip = toTrim.find_last_not_of(trimChars);
    if (lstrip == std::string::npos || rstrip == std::string::npos)
        toTrim = "";
    else
        toTrim = toTrim.substr(lstrip, (rstrip - lstrip) + 1);
}
inline
bool Utils::StrEndsWith(const std::string& str, const std::string& strEnd)
{
    if (strEnd.size() > str.size()) return false;
    return std::equal(str.begin() + str.size() - strEnd.size(), str.end(), strEnd.begin());
}
inline
bool Utils::StrContains(const std::string& str, const std::string& strFind)
{
    return (str.find(strFind) != std::string::npos) ? true : false;
}

template <typename Type>
inline std::string Utils::GetString(const Type& data)
{
    std::stringstream ss;
    ss << data;
    return ss.str();
}
template <typename Type>
inline bool Utils::IsNan(Type x)
{
    return std::isnan(x);
    // Nan - not a number, for more info : www.parashift.com/c++-faq-lite/newbie.html#faq-29.15
    // return (x != x);
}
inline
bool Utils::DoubleEquals (double left, double right, double epsilon)
{
    return (fabs(left - right) < epsilon);
    // http://www.learncpp.com/cpp-tutorial/35-relational-operators-comparisons/
    // const double dpEpsilon = 0.000001; // or some other small number
    // return fabs(dX - dY) <= dEpsilon * fabs(dX);
    // qt way
    // return qFuzzyCompare(1 + left, 1 + right);
}
inline
int Utils::DoubleCompare(double left, double right, double epsilon)
{
    if (DoubleEquals(left, right, epsilon)) return 0;
    return ((left < right) ? -1 : 1);
}
inline
int64 Utils::Double2Int64 (double dbl)
{
    return static_cast<int64>(floor(dbl + 0.5));
}
inline
int64 Utils::Price2Int64(double dblPrice, double dblTick, int32 nPriceMultiplier)
{
    double dblRound = RoundPrice(dblPrice, dblTick);
    return static_cast<int64>(floor(dblRound * nPriceMultiplier + 0.5));
}
inline
double Utils::Price2Double(int64  lPrice, double dblTickSize, int32 nPriceMultiplier)
{
    return RoundPrice((lPrice/double(nPriceMultiplier)), dblTickSize);
}
inline
double Utils::RoundPrice(double dblPrice, double tick)
{
    // (e.g) if tick is 0.005, price is 1.0024 - return is 1.0
    //       if tick is 0.005, price is 1.0025 - return 1.005
    return (floor((dblPrice + (tick/2)) / tick) * tick);
}
inline
double Utils::RoundDouble(double dbl, uint16 numDecimalPlaces)
{
    double places = pow(10.0, numDecimalPlaces);
    return round(dbl * places) / places;
}
inline
bool Utils::IsPrimeNumber(int n)
{
    if (n == 1) return false;
    if (n % 2 == 0) return false;
    for (int i = 3; i <= sqrt(n); i+=2){
        if((n % i) == 0) return false;
    }
    return true;
}
inline
bool Utils::IsDigit(const char c)
{
    return (::isdigit(c)) ? true : false;
}
inline
bool Utils::IsCharPresent(char c, const std::string& str)
{
    return (str.find(c) == str.npos) ? false : true;
}

#endif // UTILS_H
