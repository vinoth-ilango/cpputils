#ifndef _CONFIG_READER_H
#define _CONFIG_READER_H

#include <string>
#include <map>

typedef std::map<std::string, std::string> KeyValues;

class ConfigReader
{
public:
    ConfigReader();
    bool Init(const std::string& strFile);
    std::string GetValue(const std::string& key, const std::string& strDefault = "");
private:
    KeyValues m_Configs;
    bool GetTokens(const std::string& configLine, std::string& key, std::string& value);
};

#endif // _CONFIG_READER_H
