#include "ConfigReader.h"
#include "Utils.h"
#include <iostream>
#include <fstream>

const char CFG_DELIM = '=';

ConfigReader::ConfigReader() : m_Configs()
{
}
std::string ConfigReader::GetValue(const std::string& key, const std::string& strDefault)
{
    std::string strVal = strDefault;
    KeyValues::iterator iter = m_Configs.find(key);
    if (iter != m_Configs.end()) {
        strVal = iter->first;
    }
    return strVal;
}

bool ConfigReader::Init(const std::string& strFile)
{
    if (strFile.empty()) {
        std::cout << "Invalid config filename" << std::endl;
        return false;
    }
    std::ifstream configFile(strFile);
    if (!configFile.is_open()) {
        std::cout << "failed to open(" << strFile << ")" << std::endl;
        return false;
    }
    std::string strLine;
    int         lineNum = 0;

    while(std::getline(configFile, strLine)) {
        std::string strKey;
        std::string strValue;
        ++lineNum;
        if (strLine.empty())   continue;
        if (strLine[0] == '#') continue;

        if (GetTokens(strLine, strKey, strValue)) {
            m_Configs.insert(std::pair<std::string, std::string>(strKey,strValue));
        }
    }
    if (m_Configs.size()) return true;
    return false;
}

bool ConfigReader::GetTokens(const std::string& strConfig, std::string& strkey, std::string& strvalue)
{
    size_t pos = strConfig.find(CFG_DELIM);
    if (pos == std::string::npos) {
        std::cout << "No delimiter (" << CFG_DELIM << ") found in line : "
                  << strConfig << std::endl;
        return false;
    }
    if (pos != strConfig.rfind(CFG_DELIM)) {
        std::cout << "More than one delimter (" << CFG_DELIM << ") found in the line : "
             << strConfig << std::endl;
        return false; 
    }
    // Split up key and value and trim 
    Utils::StrSplit(strConfig, CFG_DELIM, strkey, strvalue);
    Utils::StrTrim(strkey);
    Utils::StrTrim(strvalue);
    if (strkey.empty() || strvalue.empty()) return false;
    return true;
}

