#ifdef _MSC_VER
#include <Windows.h>
#include <string>
#include <io.h>
#elif defined(__unix__)
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include "inifileopt.h"
#define WritePrivateProfileStringA WritePrivateProfileString
#include "inifile.h"
#endif

//
#include "tinyxml.h"
#include <assert.h>
#include <map>
#include <list>
#include "core/jframe_facade.h"

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

// export from jframeworkdir library
J_EXTERN_C J_EXTERN const char* appName();
J_EXTERN_C J_EXTERN const char* appDirPath();
J_EXTERN_C J_EXTERN const char* thisDirPath();
J_EXTERN_C J_EXTERN const char* frameDirPath();
J_EXTERN_C J_EXTERN const char* nativeSeparator();
J_EXTERN_C J_EXTERN const char* envSeparator();
J_EXTERN_C J_EXTERN bool fileExists(const char* filePath);
J_EXTERN_C J_EXTERN const char* frameVersion();
J_EXTERN_C J_EXTERN const char* libraryPrefix();
J_EXTERN_C J_EXTERN const char* librarySuffix();

/**
 * @brief replaceString
 * @param input
 * @param src
 * @param dst
 * @return
 */
std::string &replaceString(std::string &input, const std::string &src, const std::string dst)
{
    std::string::size_type pos = 0;
    std::string::size_type srclen = src.size();
    std::string::size_type dstlen = dst.size();
    while ((pos = input.find(src, pos)) != std::string::npos) {
        input.replace(pos, srclen, dst);
        pos += dstlen;
    }

    return input;
}

/**
 * @brief generateQtConf
 * @param values
 * @return
 */
bool generateQtConf(const std::map<std::string, std::string> &values)
{
    const std::string filePath = std::string(thisDirPath()).append("/bin/qt.conf");
    std::map<std::string, std::string>::const_iterator citer(values.begin());
    for (; citer != values.end(); ++citer) {
#ifdef _MSC_VER
        WritePrivateProfileStringA("Paths", citer->first.c_str(),
                                   citer->second.c_str(), filePath.c_str());
#elif defined(__unix__)
        write_profile_string("Paths", citer->first.c_str(),
                             citer->second.c_str(), filePath.c_str());
#else
#pragma message("not supported!")
#endif
    }

    return true;
}

#if defined(__unix__)
bool generatorLdConfig(const std::list<std::string> &paths)
{
    //
    (void)paths;

    return true;
}
#endif

/**
 * @brief trimmedString
 * @param input
 * @return
 */
std::string &trimmedString(std::string &input)
{
    input.erase(0, input.find_first_not_of(" \t\n\r"));
    input.erase(input.find_last_not_of(" \t\n\r") + 1);
    return input;
}

/**
 * @brief frameVersion
 * @return
 */
bool loadConfig()
{
    // 生成框架全局配置文件路径
    std::string filePath = std::string(thisDirPath()).append("/config/")
            .append(std::string(::appName())).append("/jframe_global.xml");

    // 检测文件是否存在
    if (!fileExists(filePath.c_str())) {
        return false;   // not exists
    }

    // 加载框架全局配置文件
    TiXmlDocument document(filePath);
    if (!document.LoadFile(TIXML_ENCODING_UTF8)) {
        return false;   // load failure
    }

    // 获取根节点
    TiXmlElement *emRoot = document.RootElement();
    if (!emRoot) {
        assert(false);
        return false;   // no root node
    }

    // 查找指定软件名称节点
    std::string sVal;
    TiXmlElement *emApp = 0;
    for (emApp = emRoot->FirstChildElement("app");
         emApp != 0;
         emApp = emApp->NextSiblingElement("app")) {
        if (emApp->QueryStringAttribute("name", &sVal) == TIXML_SUCCESS
                && sVal == std::string(::appName())) {
            break;
        }
    }

    // 未找到，则默认使用第一个没有指定软件名称的节点
    if (emApp == 0) {
        emApp = emRoot->FirstChildElement("app");
        if (emApp == 0
                || emApp->QueryStringAttribute("name", &sVal) == TIXML_SUCCESS
                && !sVal.empty()) {
            return false;   // 未找到
        }
    }

    // 获取环境变量配置项
    TiXmlElement *emEnvval = emApp->FirstChildElement("envval");
    if (!emEnvval) {
        return false;
    }

    // 获取全局环境变量配置节点
    TiXmlElement *emGlobal = emEnvval->FirstChildElement("global");
    if (!emGlobal) {
        return false;
    }

    // 设置环境变量
    std::string paths;
#ifdef _MSC_VER
    const char* envPath = getenv("PATH");
    if (envPath) {
        paths = std::string(envPath);
        paths = trimmedString(paths);
        if (paths.find_last_of(";") != -1) {
            paths.append(";");
        }
    }
#elif defined(__unix__)
    const char* envPath = getenv("PATH");
    if (envPath) {
        paths = std::string(envPath);
    }
#else
#pragma message("Not supported!")
#endif

    //
    paths.append(std::string(frameDirPath()).append("/bin"));

    // 寻找全局变量配置->path节点
    for (TiXmlElement *emPath = emGlobal->FirstChildElement("path");
         emPath != 0;
         emPath = emPath->NextSiblingElement("path")) {
        // 获取base属性值
        if (emPath->QueryStringAttribute("base", &sVal) != TIXML_SUCCESS) {
            continue;
        }

        // 替换特殊字段
        std::string pathBase = sVal;
        replaceString(pathBase, "@FrameDir@", frameDirPath());
        replaceString(pathBase, "@AppDir@", appDirPath());
        replaceString(pathBase, "@ThisDir@", thisDirPath());
        replaceString(pathBase, "@ConfigDir@", std::string(thisDirPath()).append(::appName()));

        // 路径有效性检测
        if (!fileExists(pathBase.c_str())) {
            continue;   // 路径无效
        }

        // 获取type属性值
        std::string pathType;
        if (emPath->QueryStringAttribute("type", &pathType) != TIXML_SUCCESS
                || pathType.empty()) {
            continue;
        }

        //
        std::string newPaths;
        for (TiXmlElement *emItem = emPath->FirstChildElement("item");
             emItem != 0;
             emItem = emItem->NextSiblingElement("item")) {
            // 获取path属性值
            std::string path;
            if (emItem->QueryStringAttribute("path", &path) != TIXML_SUCCESS
                    || path.empty()) {
                continue;
            }

            //
            path = pathBase + '/' + path;
            if (newPaths.empty()) {
                newPaths = path;
            } else {
                newPaths += envSeparator() + path;
            }
        }

        //
        if (pathType == "append") {
            paths += envSeparator() + newPaths;
        } else if (pathType == "prefix") {
            paths = newPaths + envSeparator() + paths;
        } else if (pathType == "replace") {
            paths = newPaths;
        } else {
            //
        }
    }

    // 设置环境变量
#ifdef _MSC_VER
    paths = "PATH=" + paths;
    putenv(paths.c_str());
#elif defined(__unix__)
    paths = "PATH:" + paths;
    putenv(const_cast<char *>(paths.c_str()));
#else
#pragma message("Not supported!")
#endif

    /// for qt.conf

    // 获取qtconf节点
    TiXmlElement *emQtConf = emApp->FirstChildElement("qtconf");
    if (!emQtConf) {
        return false;
    }

    // 获取paths节点
    TiXmlElement *emPaths = emQtConf->FirstChildElement("paths");
    if (!emPaths) {
        return false;
    }

    //
    std::map<std::string, std::string> values;

    // 获取item节点
    for (TiXmlElement *emItem = emPaths->FirstChildElement("item");
         emItem != 0;
         emItem = emItem->NextSiblingElement("item")) {
        // key
        std::string sKey;
        if (emItem->QueryStringAttribute("key", &sKey) != TIXML_SUCCESS
                || sKey.empty()) {
            continue;
        }

        // value
        if (emItem->QueryStringAttribute("value", &sVal) != TIXML_SUCCESS) {
            continue;
        }

        // 替换特殊字段
        std::string pathValue = sVal;
        replaceString(pathValue, "@FrameDir@", frameDirPath());
        replaceString(pathValue, "@AppDir@", appDirPath());
        replaceString(pathValue, "@ThisDir@", thisDirPath());
        replaceString(pathValue, "@ConfigDir@", std::string(thisDirPath()).append(::appName()));

        // 路径有效性检测
        if (!fileExists(pathValue.c_str())) {
            continue;   // 路径无效
        }

        //
        if (sKey == "Plugins") {
            values[sKey] = pathValue;
        } else if (sKey == "Imports") {
            values[sKey] = pathValue;
        } else if (sKey == "Qml2Imports") {
            values[sKey] = pathValue;
        }
    }

    //
    if (!values.empty()) {
        generateQtConf(values);
    }

    // 解析系统节点信息

    return true;
}

/**
 * @brief LoadFrameworkEnv : ...
 * @param config : 0/1, release; 2, release. invalid for unix system
 * @return
 */
J_EXTERN_C J_ATTR_EXPORT bool LoadFrameworkEnv(int config)
{
    (void)config;

    // 加载框架全局配置
    if (!loadConfig()) {
        return false;
    }

    return true;
}
