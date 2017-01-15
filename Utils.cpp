#include "Utils.h"
#include <fstream>
#include <algorithm>
// #include <dirent.h>     // To list director header
#include <sys/stat.h>   // to check if directory exists
#include <iomanip>      // std::setprecision
// #include <syslog.h>     // To write in system log
// #include <unistd.h>     // Required for unlink (delete file) function

void Utils::StrTokenize(const std::string& strIn, char delim, stringvector& tokens)
{
    const char* str = strIn.c_str();
    while(true) {
        const char* begin = str;
        while(*str != delim && *str) str++;
        tokens.push_back(std::string(begin, str));
        if (0 == *str++) break;
    }
}
void Utils::StrTokenize(const std::string& str, const std::string& delims, stringvector& tokens)
{
    std::string::size_type lastPos = str.find_first_not_of(delims, 0);   // Skip delims at begin
    std::string::size_type pos     = str.find_first_of(delims, lastPos); // Find "non-delimiter"
    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        tokens.push_back(str.substr(lastPos, pos - lastPos)); // Found a token
        lastPos = str.find_first_not_of(delims, pos);     // Skip delimiters. Note the "not_of"
        pos = str.find_first_of(delims, lastPos);         // Find next "non-delimiter"
    }
}
void Utils::StrTokenizeWithDelims(const std::string& str,stringvector& vec, const std::string& strDelims)
{
    // if str = "(1+200+ 20)" then vec contains {(,1,+,200,+, ,20,)}
    const char* sz = str.c_str();
    while (*sz) {
        if (IsCharPresent(*sz, strDelims)) {
            vec.push_back(std::string(1, *sz));
            ++sz; continue;
        }
        const char* begin = sz;
        while(*sz && !IsCharPresent(*sz, strDelims)) ++sz;
        vec.push_back(std::string(begin, sz));
    }
}
void Utils::RemoveString(stringvector& vec, const std::string& str)
{
    vec.erase(std::remove(vec.begin(), vec.end(), str), vec.end());
}
void Utils::StrFindAndReplace(std::string& str, char find, char replace)
{
    std::replace(str.begin(), str.end(), find, replace);
}
void Utils::StrFindAndReplace(std::string& str, const std::string& find, const std::string& replace)
{
    std::string::size_type pos = 0;
    while ( (pos = str.find(find, pos)) != std::string::npos ) {
        str.replace( pos, find.size(), replace);
        pos++;
    }
}
void Utils::StrToUpper(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

bool Utils::FileExists(const std::string& strPath)
{
    bool bExits = false;
    std::fstream fin;
    fin.open(strPath, std::ios::in);
    if (fin.is_open()) { bExits = true;}
    fin.close();
    return bExits;
}
bool Utils::DeleteFile(const std::string& /*strFile*/)
{
    // return (unlink(strFile.c_str()) == 0) ? true : false;
    return false;
}
bool Utils::MakeDir(const std::string& /*path*/)
{
//    TODO: Need to check in linux
//    struct stat st;
//    bool status = true;

//    if (stat(path, &st) != 0) { // Directory does not exist
//        if (mkdir(path, 0777) != 0) status = false;
//    }
//    else if (!S_ISDIR(st.st_mode)) {
//        errno = ENOTDIR;
//        status = false;
//    }
//    return(status);
    return false;
}
bool Utils::MakePath(const std::string& /*path*/)
{
//    TODO: Linux
//    char *pp;
//    char *sp;
//    bool status    = true;
//    char *copypath = strdup(path);

//    pp = copypath;
//    while (status && (sp = strchr(pp, '/')) != 0)
//    {
//        if (sp != pp)
//        {
//            /* Neither root nor double slash in path */
//            *sp = '\0';
//            status = MMUtils::MakeDir(copypath);
//            *sp = '/';
//        }
//        pp = sp + 1;
//    }
//    if (status)
//        status = MMUtils::MakeDir(path);
//    free(copypath);
//    return (status);
    return false;
}
bool Utils::DirExists (const std::string& /*dir*/)
{
//    struct stat st;
//    return (stat(dir,&st) == 0) ? true : false;
    return false;
}

bool Utils::GetAllFilesInDir(const std::string& /*strDir*/,stringvector& /*vecfiles*/,std::string /*strFilter*/)
{
//    DIR* dir = NULL;
//    dir      = opendir(strDir.c_str());
//    if (dir == NULL) {
//        std::cerr << "opendir api failed" << std::endl;
//        return false;
//    }
//    struct dirent* entry = NULL;
//    while((entry         = readdir(dir))) {
//        std::string strFileName = entry->d_name;
//        if (StringEndsWith(strFileName, strFilter))
//            vecfiles.push_back(strFileName);
//    }
//    closedir(dir);
    return true;
}

void Utils::WriteToSysLog(const std::string& /*prefix*/, int /*logLevel*/, const std::string& /*log*/)
{
//    openlog(prefix.c_str(), LOG_PID, LOG_LOCAL5);
//    syslog (logLevel, log.c_str());
//    closelog();
}

std::string Utils::GetPricePrecisionString(double data, int precision)
{
    std::stringstream ss;
    ss << std::setiosflags(std::ios::fixed) << std::setprecision(precision) << data;
    return ss.str();
}
size_t Utils::StrHash(const std::string& str)
{
    std::hash<std::string> hash_fn;
    return hash_fn(str);
//    const char* str = str2Hash.c_str();
//    size_t len      = str2Hash.size();
//    // ELF hash
//    uint32 hash = 0;
//    uint32 x    = 0;

//    for(size_t i = 0; i < len; str++, i++){
//        hash = (hash << 4) + (*str);
//        if((x = hash & 0xF0000000L) != 0) {
//            hash ^= (x >> 24);
//        }
//        hash &= ~x;
//    }
//    return hash;
}
bool Utils::CopyFile(const std::string& src, const std::string& dest)
{
    if (FileExists(src)) {
        std::ifstream in(src);
        std::ofstream out(dest);
        out << in.rdbuf();
        out.close();
        in.close();
        return true;
    }
    return false;
}
