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
    std::string interfaceIdentity() const;
    unsigned int interfaceVersion() const;
    void *queryInterface(const std::string &iid, unsigned int ver);
    bool loadInterface();
    void releaseInterface();
    std::list<std::string> queryMethod() const;
    bool invokeMethod(const std::string &method, int argc, ...);

    // IJFrameFacade interface
public:
    std::string appDirPath() const;
    std::string configDirPath() const;
    std::string thisDirPath() const;
    std::string frameDirPath() const;
    std::string frameGlobalPath() const;
    std::string frameLayoutPath() const;

    std::string frameVersion() const;
    bool frameVersion(int &major, int &minor, int &patch) const;

    bool loadFrame(int argc = 0, char** argv = 0, void *app = 0);

    void showFrame(bool show = true, bool maximized = true);

    void tryExitFrame();
    void exitFrame();
    void restartFrame(const std::list<std::string> &arguments);

    bool loginFrame();
    bool logoutFrame();

    std::string getEnvValue(const std::string &name) const;
    int runQApp(void *mfcApp);
    long windowHandle(void *window, const std::string &winType);

    std::string language() const;

private:
     //
    bool loadFramePrivate(int argc, char** argv, void *app);

     //
    IJUnknown *loadFrameInterface(const std::string &moduleName);
    void releaseFrameInterface(const std::string &moduleName);

    //
    bool loadFrameFactory();
    bool loadFrameKernel();

    //
    void copyFrameFile();

    // 加载程序配置文件
    bool loadConfig();

    // 获取模块前缀
    std::string dynamicPrefix() const;

    // 获取模块后缀
    std::string dynamicSuffix(bool debug = true) const;

    // 获取环境变量的分割符
    std::string envSeparator() const;

    // 强制退出当前进程
    bool terminateProcess();

    // 启动默认框架系统
    bool loadDefaultSystem();

    // 赋值文件夹
    bool copyDir(const QString &fromDir, const QString &toDir);

    // 删除文件夹
    bool deleteDir(const QString &path);

    //
    bool testAnotherApp();

    //
    bool loadTextCodecConfig();

    //
    bool loadFrameworkDirMethod();

private:
    bool invokeLog(const std::string &method, int argc, va_list ap);
    bool invokeExitFrame();
    bool invokeRestartFrame();
    bool invokeLibraryQueryExists(int argc, va_list ap);
    bool invokeLibraryResolve(int argc, va_list ap);

private:
    JFrameFacade();
    ~JFrameFacade();

    JFrameFacadeData* data;

    //
    static QMutex _instance_mutex;      // 单实例保护锁
    static JFrameFacade* _instance;     // 单实例指针
};

#endif // JFRAME_FACADE_P_H
