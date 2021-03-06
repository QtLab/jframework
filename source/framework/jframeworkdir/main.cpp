﻿#ifdef _MSC_VER
#include <Windows.h>
#include <string>
#include <io.h>
#elif defined(__unix__)
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <dlfcn.h>
#include <limits.h>
#endif

#include "tinyxml.h"
#include <assert.h>
#include "core/jframe_facade.h"

/**
 * @brief _g_app_name
 */
static std::string _g_app_name = "";

/**
 * @brief appDirPath : 获取软件名称
 * @return 软件名称
 */
J_EXTERN_C J_ATTR_EXPORT const char* appName()
{
    return _g_app_name.c_str();
}

/**
 * @brief appDirPath : 获取 application 所在路径（不包含文件名称）
 * @return application 所在路径（不包含文件名称）
 */
J_EXTERN_C J_ATTR_EXPORT const char* appDirPath()
{
    static std::string _path = "";
    if (_path.empty()) {
#ifdef __unix__
        std::stringstream ss;
        ss << "/proc/" << getpid() << "/exe";
#endif
        //
        char buffer[J_PATH_MAX + 2];
        unsigned int length = 0;
#ifdef _MSC_VER
        length = (unsigned int)GetModuleFileNameA(NULL, buffer, J_PATH_MAX + 1);
#elif defined(__unix__)
        length = (unsigned int)readlink(ss.str().c_str(), buffer, J_PATH_MAX);
#endif
        //
        if (length == 0) {
            // error
        } else if (length <= J_PATH_MAX) {
            buffer[J_PATH_MAX + 1] = '\0';
            _path = buffer;
        } else {
            // J_PATH_MAX sized buffer wasn't large enough to contain the full path, use heap
            char *bufferNew = 0;
            int i = 1;
            size_t size;
            do {
                ++i;
                size = J_PATH_MAX * i;
                bufferNew = reinterpret_cast<char *>(realloc(bufferNew, (size + 1) * sizeof(char)));
                if (bufferNew) {
#ifdef _MSC_VER
                    length = (unsigned int)GetModuleFileNameA(NULL, buffer, J_PATH_MAX + 1);
#elif defined(__unix__)
                    length = (unsigned int)readlink(ss.str().c_str(), buffer, J_PATH_MAX);
#endif
                }
            } while (bufferNew && length == size);

            if (bufferNew) {
                *(bufferNew + size) = '\0';
            }

            _path = bufferNew;
            free(bufferNew);
        }

        //
        if (!_path.empty()) {
            //
            int index = _path.find_last_of('/');
            if (index == -1) {
                index = _path.find_last_of('\\');
            }
            if (index != -1) {
                _path = _path.substr(0, index);
            }
            // replace '\\' with '/'
            std::string::iterator iter = _path.begin();
            for (; iter != _path.end(); ++iter) {
                if (*iter == '\\') {
                    *iter = '/';
                }
            }
        }
    }

    return _path.c_str();
}

/**
 * @brief thisDirPath
 * @return
 */
J_EXTERN_C J_ATTR_EXPORT const char* thisDirPath()
{
    static std::string _path = "";
    if (_path.empty()) {
        // 获取 application 所在路径（不包含文件名称）
        _path = appDirPath();
        // 去掉 /bin
        int index = _path.find_last_of('/');
        if (index != -1) {
            _path = _path.substr(0, index);
        }
    }

    return _path.c_str();
}

/**
 * @brief nativeSeparator
 * @return
 */
J_EXTERN_C J_ATTR_EXPORT const char* nativeSeparator()
{
    static std::string _v = "";
    if (_v.empty()) {
#ifdef _MSC_VER
        _v = "\\";
#elif defined(__unix__)
        _v = "/";
#else
#pragma message("not supported!")
        _v = "/";
#endif
    }

    return _v.c_str();
}

/**
 * @brief envSeparator
 * @return
 */
J_EXTERN_C J_ATTR_EXPORT const char* envSeparator()
{
    static std::string _v = "";
    if (_v.empty()) {
#ifdef _MSC_VER
        _v = ";";
#elif defined(__unix__)
        _v = ":";
#else
#pragma message("not supported!")
        _v = ":";
#endif
    }

    return _v.c_str();
}

/**
 * @brief fileExists
 * @return
 */
J_EXTERN_C J_ATTR_EXPORT bool fileExists(const char* filePath)
{
    if (!filePath) {
        return false;
    }

    // 检测文件是否存在
#ifdef _MSC_VER
    if (_access(filePath, 0) == -1) {
#elif defined(__unix__)
    if (access(filePath, F_OK) != 0) {
#else
#pragma message("not supported!")
    if (true) {
#endif
        return false;   // 不存在
    }

    return true;        // 存在
}

/**
 * @brief frameVersion
 * @return
 */
J_EXTERN_C J_ATTR_EXPORT const char* frameVersion()
{
    static std::string _version = "";
    if (!_version.empty()) {
        return _version.c_str();
    }

    // 生成框架全局配置文件路径
    std::string filePath = std::string(thisDirPath()).append("/config/")
            .append(_g_app_name).append("/jframe_global.xml");

    // 检测文件是否存在
    if (!fileExists(filePath.c_str())) {
        //assert(false);
        return _version.c_str();   // not exists
    }

    // 加载框架全局配置文件
    TiXmlDocument document(filePath);
    if (!document.LoadFile(TIXML_ENCODING_UTF8)) {
        assert(false);
        return _version.c_str();   // load failure
    }

    // 获取根节点
    TiXmlElement *emRoot = document.RootElement();
    if (!emRoot) {
        assert(false);
        return _version.c_str();   // no root node
    }

    // 查找指定软件名称节点
    std::string sVal;
    TiXmlElement *emApp = 0;
    for (emApp = emRoot->FirstChildElement("app");
         emApp != 0;
         emApp = emApp->NextSiblingElement("app")) {
        if (emApp->QueryStringAttribute("name", &sVal) == TIXML_SUCCESS
                && sVal == _g_app_name) {
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

    // 获取框架版本号
    if (emApp->QueryStringAttribute("frameVersion", &_version) != TIXML_SUCCESS) {
        assert(false);
        return _version.c_str();
    }

    return _version.c_str();
}

/**
 * @brief frameDirPath : 获取框架部署路径
 * @return : 框架部署路径
 */
J_EXTERN_C J_ATTR_EXPORT const char* frameDirPath()
{
    //
    static std::string _path = "";
    if (_path.empty()) {
        // 获取框架版本号
        std::string version = frameVersion();
        if (version.empty()) {
            version = "1.2.0";  // default version
        }

        // 获取框架部署路径
        std::stringstream key;
#ifdef _MSC_VER
        // 生成注册表键
        key << "Software\\Smartsoft\\JFramework\\" << version << "\\";

        // 查询键值 InstallPath
        HKEY hKey = NULL;
        if (RegOpenKeyA(HKEY_CURRENT_USER, key.str().c_str(), &hKey) == ERROR_SUCCESS) {
            if (hKey) {
                CHAR buffer[MAX_PATH + 2];
                DWORD length = MAX_PATH;
                if (RegQueryValueExA(hKey, "InstallPath", 0, NULL,
                                     (LPBYTE)buffer, &length) == ERROR_SUCCESS) {
                    buffer[MAX_PATH + 1] = '\0';
                    _path = buffer;
                    // replace '\\' with '/'
                    std::string::iterator iter = _path.begin();
                    for (; iter != _path.end(); ++iter) {
                        if (*iter == '\\') {
                            *iter = '/';
                        }
                    }
                }
                // close key
                RegCloseKey(hKey);
            }
        }
#elif defined(__unix__)
        const char* sTemp = getenv("JFRAME_DIR");
        if (sTemp) {
            _path = sTemp;
        }
#else
#pragma message("not supported!")
#endif
        // 如果找到框架部署路径，则使用 application 路径（上一级）
        if (_path.empty()) {
            _path = thisDirPath();
        }
    }

    return _path.c_str();
}

/**
 * @brief libraryPrefix
 * @return
 */
J_EXTERN_C J_ATTR_EXPORT const char* libraryPrefix()
{
    static std::string _v = "";
    if (_v.empty()) {
#ifdef _MSC_VER
        _v = "";
#elif defined(__unix__)
        _v = "lib";
#else
#pragma message("not supported!")
        _v = "";
#endif
    }

    return _v.c_str();
}

/**
 * @brief librarySuffix
 * @param debug
 * @return
 */
J_EXTERN_C J_ATTR_EXPORT const char* librarySuffix(bool debug)
{
#ifdef _MSC_VER
    return debug ? "d.dll" : ".dll";
#elif defined(__unix__)
    (void)debug;
    return ".so";
#else
#pragma message("not supported!")
    (void)debug;
    return "";
#endif
}

/**
 * @brief LoadFrameworkEnv
 * @param config
 * @return
 */
J_EXTERN_C J_ATTR_EXPORT bool LoadFrameworkEnv(int config)
{
    // 获取 jframework 部署路径
    const std::string _frameDirPath = frameDirPath();
    if (_frameDirPath.empty()) {
        return false;   // 路径无效
    }

    // 生成 jframeworkenv 全路径
    const std::string frameworkenvFilePath = _frameDirPath + "/bin/core/"
            + std::string(libraryPrefix()) + "jframeworkenv"
            + std::string(librarySuffix(false));

    // 获取 LoadFrameworkEnv 导出接口
    typedef bool (J_ATTR_CDECL*LoadFrameworkEnv)(int);
    LoadFrameworkEnv loadFrameworkEnv = (LoadFrameworkEnv)
            JLibrary::resolve(frameworkenvFilePath, "LoadFrameworkEnv");
    if (!loadFrameworkEnv) {
        return false;   // 获取失败
    }

    // 加载 jframeworkenv 模块
    if (!loadFrameworkEnv(config)) {
        return false;   // 加载失败
    }

    return true;
}

/**
 * @brief ...
 * @param config : 1, debug; other, release. valid for windows system
 * @param appName : 软件名称
 * @return 框架门面接口
 */
J_EXTERN_C J_ATTR_EXPORT void* FrameFacadeInstace(int config, const char *appName)
{
    // 参数检测
    if (!appName) {
        return 0;   // 无效
    }

    // 存储软件名称
    _g_app_name = std::string(appName);

    // 加载 jframeworkenv 模块
    if (!LoadFrameworkEnv(config)) {
        return 0;
    }

    // 获取 jframework 部署路径
    const std::string _frameDirPath = frameDirPath();
    if (_frameDirPath.empty()) {
        return 0;   // 路径无效
    }

    /// 加载 frame_facade 模块

    // 生成 frame_facade 全路径
    const std::string frameFacadeFilePath = _frameDirPath + "/bin/core/"
            + std::string(libraryPrefix()) + "jframe_facade"
            + std::string(librarySuffix(config == 1));

    // 获取 CreateInstance 导出接口
    typedef void *(J_ATTR_CDECL*CreateInstance)();
    CreateInstance createInstance = (CreateInstance)
            JLibrary::resolve(frameFacadeFilePath, "CreateInstance");
    if (!createInstance) {
        return 0;   // 加载失败
    }

    // 获取 JrameFacade 实例接口
    void *jframeFacade = createInstance();
    if (!jframeFacade) {
        return 0;   // 获取失败
    }

    return jframeFacade;
}
