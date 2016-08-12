#ifndef JFRAME_FACADE_P_H
#define JFRAME_FACADE_P_H

#include "../jframe_facade.h"
#include <QMutex>

// class JFrameFacade

struct JFrameFacadeData;

class JFrameFacade : public IJFrameFacade
{
    // single instance
public:
    static JFrameFacade* getInstance();
    static void releaseInstance();

    // IJUnknown interface
public:
    void releaseInterface();
    void *queryInterface(const char *iid, unsigned int ver);

    // IJObject interface
public:
    std::string objectIdentity() const;
    unsigned int objectVersion() const;
    bool invoke(const char *method, int argc, ...);

    // IJFrameFacade interface
public:
    std::string frameDirPath() const;
    std::string appDirPath() const;

    std::string frameGlobalPath() const;
    std::string frameLayoutPath() const;
    std::string frameComponentPath() const;
    std::string frameFramViewPath() const;

    std::string frameVersion() const;
    bool frameVersion(int &major, int &minor, int &patch) const;

    bool loadFrame();
    bool loadFrame(const char *version);
    bool loadFrame(int major, int minor, int patch);

    void showFrame(bool show = true, bool maximumed = true);

    void tryExitFrame();
    void exitFrame();
    void restartFrame(const std::list<std::string> &arguments);

    bool loginFrame();
    bool logoutFrame();

    std::string getEnvValue(const char *name) const;
    int runQApp(void *mfcApp);
    long windowHandle(void *window, const char *winType);

private:
     //
    bool loadFramePrivate();

     //
    IJUnknown* loadFrameInterface(const std::string &moduleName);

    //
    bool loadFrameKernel();
    bool loadFrameFactory();
    bool loadFrameLayout();
    bool loadFrameLogin();

    // 加载程序配置文件
    bool loadGlobalConfig();
    bool loadGlobalConfig(const std::string &version);
    bool loadGlobalConfig(int major, int minor, int patch);

    //
    void copyFrameFile();

    // 从配置文件获取框架版本
    std::string frameVersionFromConfig() const;

    // 从注册表获取指定版本的框架安装路径
    std::string framePathFromRegistry(const std::string &version);

    // 加载程序配置文件
    bool loadConfig(const std::string &frameDirPath);

    //
    bool generateAppQtConf(const std::map<std::string, std::string> &values);

    // 获取模块后缀
    std::string dynamicSuffix() const;

    // 强制退出当前进程
    bool terminateProcess();

    // 启动默认框架系统
    bool loadDefaultSystem();

    // 赋值文件夹
    bool copyDir(const QString &fromDir, const QString &toDir);

    // 删除文件夹
    bool deleteDir(const QString &path);

private:
    bool invokeLog(const char *method, int argc, va_list ap);
    bool invokeExitFrame();
    bool invokeRestartFrame();

private:
    JFrameFacade();
    ~JFrameFacade();

    JFrameFacadeData* data;

    //
    static QMutex _instance_mutex;      // 单实例保护锁
    static JFrameFacade* _instance;     // 单实例指针
};

#endif // JFRAME_FACADE_P_H
