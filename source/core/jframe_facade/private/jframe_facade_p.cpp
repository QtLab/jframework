#include "precomp.h"
#include "jframe_facade_p.h"
#include <QCoreApplication>
#include "tinyxml.h"
#if defined(__unix__)
#include <unistd.h>
#endif

/**
 * @brief appDirPath : 获取 application 所在路径（不包含文件名称）
 * @return : application 所在路径（不包含文件名称）
 */
const char* _static_appDirPath()
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

// export from jframeworkdir library
typedef const char* (J_ATTR_CDECL*ThisDirPath)();
typedef const char* (J_ATTR_CDECL*FrameDirPath)();
typedef const char* (J_ATTR_CDECL*FrameVersion)();

// struct JFrameFacadeData

struct JFrameFacadeData
{
    bool frameLoaded;           // 框架加载状态

    IJUnknown* frameFactory;    // 框架工厂系统部件
    IJUnknown* frameKernel;     // 框架内核系统部件

    std::string appDirPath;     // 软件实体路径
    std::string configDirPath;  // 框架配置文件夹路径
    std::string thisDirPath;    // 软件实体部署路径（application可执行文件上一级路径）
    std::string frameDirPath;   // 框架部署路径

    std::string frameVersion;   // 框架版本

    //
    std::string frameGlobalPath;    // 框架全局配置文件路径
    std::string frameLayoutPath;    // 框架布局配置文件路径

    //
    std::string language;           // 软件系统语言（如："zh_CN"）

    //
    std::list<std::string> arguments;   // 重启框架命令行参数临时存放变量

    // export from jframeworkdir library
    ThisDirPath fThisDirPath;       //
    FrameDirPath fFrameDirPath;     //
    FrameVersion fFrameVersion;     //

    JFrameFacadeData()
        : frameLoaded(false)
        , frameFactory(0)
        , frameKernel(0)
        //
        , fThisDirPath(0)
        , fFrameDirPath(0)
        , fFrameVersion(0)
    {

    }
};

// class JFrameFacade

//
QMutex JFrameFacade::_instance_mutex;
JFrameFacade* JFrameFacade::_instance = 0;

JFrameFacade* JFrameFacade::getInstance()
{
    // 双重检测（防止多线程竞争）
    if (JFrameFacade::_instance == 0) {
        JFrameFacade::_instance_mutex.lock();
        if (JFrameFacade::_instance == 0) {
            JFrameFacade* tempInstance = new JFrameFacade;
            JFrameFacade::_instance = tempInstance;
        }
        JFrameFacade::_instance_mutex.unlock();
    }

    return JFrameFacade::_instance;
}

void JFrameFacade::releaseInstance()
{
    // 双重检测（防止多线程竞争）
    if (JFrameFacade::_instance != 0) {
        JFrameFacade::_instance_mutex.lock();
        if (JFrameFacade::_instance != 0) {
            JFrameFacade::_instance_mutex.unlock();
            delete JFrameFacade::_instance;
            JFrameFacade::_instance_mutex.lock();
            JFrameFacade::_instance = 0;
        }
        JFrameFacade::_instance_mutex.unlock();
    }
}

std::string JFrameFacade::interfaceIdentity() const
{
    return IID_IJFrameFacade;
}

unsigned int JFrameFacade::interfaceVersion() const
{
    return VER_IJFrameFacade;
}

void *JFrameFacade::queryInterface(const std::string &iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJUnknown, iid, ver);
    J_QUERY_MEMBER_OBJECT(IJUnknown, iid, ver, data->frameFactory);
    J_QUERY_MEMBER_OBJECT(IJUnknown, iid, ver, data->frameKernel);

    return 0;
}

bool JFrameFacade::loadInterface()
{
    return true;
}

void JFrameFacade::releaseInterface()
{
    // 释放框架内核部件
    if (data->frameKernel) {
        data->frameKernel->releaseInterface();
    }

    // 释放框架工厂部件
    if (data->frameFactory) {
        data->frameFactory->releaseInterface();
    }
}

std::list<std::string> JFrameFacade::queryMethod() const
{
    std::list<std::string> methods;

    // log
    methods.push_back(std::string("log").append("..."));

    return methods;
}

bool JFrameFacade::invokeMethod(const std::string &method, int argc, ...)
{
    bool result = false;
    va_list ap;
    va_start(ap, argc);

    // 输出一条日志
    if (method == "log") {
        result = invokeLog(method, argc, ap);
    }
    // 退出框架（同步方式）
    else if (method == "frame_exit") {
        result = invokeExitFrame();
    }
    // 重启框架（同步方式）
    else if (method == "frame_restart") {
        result = invokeRestartFrame();
    }
    // 查询库文件是否存在
    else if (method == "library_query_exists") {
        result = invokeLibraryQueryExists(argc, ap);
    }
    // 获取动态库导出接口
    else if (method == "library_resolve") {
        result = invokeLibraryResolve(argc, ap);
    }

    va_end(ap);

    return result;
}

std::string JFrameFacade::appDirPath() const
{
    return _static_appDirPath();
}

std::string JFrameFacade::configDirPath() const
{
    return data->configDirPath;
}

std::string JFrameFacade::thisDirPath() const
{
    return data->thisDirPath;
}

std::string JFrameFacade::frameDirPath() const
{
    return data->frameDirPath;
}

std::string JFrameFacade::frameGlobalPath() const
{
    return data->frameGlobalPath;
}

std::string JFrameFacade::frameLayoutPath() const
{
    return data->frameLayoutPath;
}

std::string JFrameFacade::frameVersion() const
{
    return data->frameVersion;
}

bool JFrameFacade::frameVersion(int &major, int &minor, int &patch) const
{
#if defined(_MSC_VER) && (_MSC_VER >= 140)
    scanf_s
        #else
    scanf
        #endif
            (data->frameVersion.c_str(), "%d.%d.%d", &major, &minor, &patch);

    return true;
}

bool JFrameFacade::loadFrame(int argc, char** argv, void *app)
{
    // 检测框架是否已加载
    if (data->frameLoaded) {
        return true;    // 已加载
    }

    // 加载框架全局配置信息
    if (!loadConfig()) {
        return false;   // 加载失败
    }

    // 加载框架
    return loadFramePrivate(argc, argv, app);
}

void JFrameFacade::showFrame(bool show, bool maximized)
{
    if (data->frameKernel) {
        // 转到框架内核系统
        data->frameKernel->invokeMethod("frame_show", 2, show, maximized);
    }
}

void JFrameFacade::tryExitFrame()
{
    if (data->frameKernel) {
        // 转到框架内核系统
        data->frameKernel->invokeMethod("frame_try_exit");
    } else {
        invokeExitFrame();
    }
}

void JFrameFacade::exitFrame()
{
    if (data->frameKernel) {
        // 转到框架内核系统
        //（通过在内核系统中异步中转回facade，来实现框架的退出，防止部件自身拥有时的释放出错）
        data->frameKernel->invokeMethod("frame_exit");
    } else {
        invokeExitFrame();
    }
}

void JFrameFacade::restartFrame(const std::list<std::string> &arguments)
{
    // 暂存参数
    data->arguments = arguments;

    if (data->frameKernel) {
        // 转到框架内核系统
        //（通过在内核系统中异步中转回facade，来实现框架的退出，防止部件自身拥有时的释放出错）
        data->frameKernel->invokeMethod("frame_restart");
    } else {
        invokeRestartFrame();
    }
}

bool JFrameFacade::loginFrame()
{
    // 检测框架是否已加载
    if (!data->frameLoaded) {
        return false;       // 未加载
    }

    // 转到框架内核系统
    return data->frameKernel->invokeMethod("frame_login");
}

bool JFrameFacade::logoutFrame()
{
    // 检测框架是否已加载
    if (!data->frameLoaded) {
        return false;       // 未加载
    }

    // 转到框架内核系统
    return data->frameKernel->invokeMethod("frame_logout");
}

std::string JFrameFacade::getEnvValue(const std::string &name) const
{
    QByteArray env = qgetenv(name.c_str());
    if (env.isNull()) {
        return std::string();
    } else {
        return std::string(env.data());
    }
}

int JFrameFacade::runQApp(void *mfcApp)
{
    // 检测框架是否已加载
    if (!data->frameLoaded) {
        return -1;       // 未加载
    }

    // 转到框架内核系统
    int ret = 0;
    if (!data->frameKernel->invokeMethod("run_q_app", 2, mfcApp, &ret)) {
        return -1;
    }

    return ret;
}

long JFrameFacade::windowHandle(void *window, const std::string &winType)
{
    // 检测框架是否已加载
    if (!data->frameLoaded) {
        return 0;   // 未加载
    }

    // 转到框架内核系统
    long handle = 0;
    if (!data->frameKernel->invokeMethod(
                "window_handle", 3, window, winType.c_str(), &handle)) {
        return -1;  // 调用失败
    }

    return handle;
}

std::string JFrameFacade::language() const
{
    return data->language;
}

std::string JFrameFacade::parsePath(const std::string &path) const
{
    QString temp = QString::fromStdString(path)
            .replace("@AppDir@", QString::fromStdString(appDirPath()))
            .replace("@ConfigDir@", QString::fromStdString(configDirPath()))
            .replace("@ThisDir@", QString::fromStdString(thisDirPath()))
            .replace("@FrameDir@", QString::fromStdString(frameDirPath()));
    return temp.toStdString();
}

bool JFrameFacade::loadFramePrivate(int argc, char **argv, void *app)
{
    //
#ifdef __unix__
    (void)new QApplication(argc, argv);
#endif // __unix__

    // 加载门面接口
    loadInterface();

    // 复制框架动态库到应用实例响应位置，进行框架动态更新
    copyFrameFile();

    bool result = true;

    // 加载框架工厂系统库
    result = result && loadFrameFactory();

    // 加载框架内核系统库
    result = result && loadFrameKernel();

    //
    data->frameLoaded = result;

    // 创建Qt应用实体
    result = result && data->frameKernel->invokeMethod("create_qapp", 3, argc, argv, app);

    // 加载框架工厂系统接口
    result = result && data->frameFactory->loadInterface();

    // 加载框架内核系统接口
    result = result && data->frameKernel->loadInterface();

    return result;
}

IJUnknown *JFrameFacade::loadFrameInterface(const std::string &moduleName)
{
    // 定义导出接口
    typedef IJUnknown* (*FuncCreateInstance)();

    // 获取导出接口
    const QString filePath = QString::fromStdString(frameDirPath() + "/bin/core/"
                                                    + dynamicPrefix() + moduleName + dynamicSuffix());
    FuncCreateInstance fCreateInstance =
            (FuncCreateInstance)QLibrary::resolve(filePath, "CreateInstance");

    if (!fCreateInstance) {
        jframeLogWarning(QStringLiteral("模块%1无法加载！（找不到CreateInstance接口或缺少依赖项）")
                         .arg(filePath).toLocal8Bit().data());
        return 0;       // 获取失败
    }

    // 获取模块实例
    return fCreateInstance();
}

void JFrameFacade::releaseFrameInterface(const std::string &moduleName)
{
    // 定义导出接口
    typedef void (*FuncReleaseInstance)();

    // 获取导出接口
    const QString filePath = QString::fromStdString(frameDirPath() + "/bin/core/"
                                                    + dynamicPrefix() + moduleName + dynamicSuffix());
    FuncReleaseInstance fReleaseInstance =
            (FuncReleaseInstance)QLibrary::resolve(filePath, "ReleaseInstance");

    if (!fReleaseInstance) {
        jframeLogWarning(QStringLiteral("模块%1无法加载！（找不到ReleaseInstance接口或缺少依赖项）")
                         .arg(filePath).toLocal8Bit().data());
        return;         // 获取失败
    }

    // 释放模块实例
    fReleaseInstance();
}

bool JFrameFacade::loadFrameFactory()
{
    // 检测框架工厂系统是否已加载
    if (data->frameFactory) {
        return true;    // 已加载
    }

    // 获取接口实例
    data->frameFactory = loadFrameInterface("jframe_factory");
    if (!data->frameFactory) {
        return false;   // 加载失败
    }

    return true;
}

bool JFrameFacade::loadFrameKernel()
{
    // 检测框架内核系统是否已加载
    if (data->frameKernel) {
        return true;    // 已加载
    }

    // 获取接口实例
    data->frameKernel = loadFrameInterface("jframe_kernel");
    if (!data->frameKernel) {
        return false;   // 加载失败
    }

    return true;
}

void JFrameFacade::copyFrameFile()
{
    QString srcdir = QString::fromStdString(data->frameDirPath).append("/bin").toLower();
    QString dstdir = QString::fromStdString(data->thisDirPath).toLower();
    if (srcdir == dstdir) {
        return;     // 框架位置与应用实例相同，不需要更新
    }

    /// 拷贝框架组件

    //
    QDir componentSrcDir(srcdir + "/Component/jframe");
    if (componentSrcDir.exists()) {
        // 删除软件实体中的框架组件文件夹
        QString componentNewDir(dstdir + "/Component/jframe");
        //deleteDir(componentNewDir);
        copyDir(componentSrcDir.absolutePath(), componentNewDir);
    }
}

bool JFrameFacade::loadConfig()
{
    //
    if (!QFileInfo(QString::fromStdString(data->frameGlobalPath)).isReadable()) {
        Q_ASSERT_X(false, "Warning", QStringLiteral("配置文件不存在，即将退出软件！")
                   .toUtf8().data());
        return false;
    }

    // 读取配置文件
    TiXmlDocument document(data->frameGlobalPath);
    if (!document.LoadFile(TIXML_ENCODING_UTF8)) {
        Q_ASSERT_X(false, "Warning", QStringLiteral("配置文件加载错误，即将推出软件！")
                   .toUtf8().data());
        return false;
    }

    // 获取根节点
    TiXmlElement *emRoot = document.RootElement();
    if (!emRoot) {
        return false;
    }

    // 解析系统节点信息

    return true;
}

std::string JFrameFacade::dynamicPrefix() const
{
    static std::string prefix = "";
    if (prefix.empty()) {
#ifdef _MSC_VER
        prefix = "";
#elif defined(__unix__)
        prefix = "lib";
#else
#pragma message("not supported!")
        prefix = "";
#endif
    }

    return prefix;
}

std::string JFrameFacade::dynamicSuffix(bool debug) const
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

std::string JFrameFacade::envSeparator() const
{
    static std::string sep = "";
    if (sep.empty()) {
#ifdef _MSC_VER
        sep = ";";
#elif defined(__unix__)
        sep = ":";
#else
#pragma message("not supported!")
        sep = ":";
#endif
    }

    return sep;
}

bool JFrameFacade::terminateProcess()
{
    // 释放框架门面系统部件单实例
    releaseInstance();

    // 退出Qt系统
    QCoreApplication::quit();

#ifdef _MSC_VER
    // 强制退出当前进程
    return !!::TerminateProcess(::GetCurrentProcess(), 0);
#else
    return true;
#endif
}

bool JFrameFacade::loadDefaultSystem()
{
    // 参数有效性检测
    if (!data->frameKernel) {
        return false;   // 无效
    }

    // 转到框架内核系统
    return data->frameKernel->invokeMethod("load_default_system");
}

bool JFrameFacade::copyDir(const QString &fromDir, const QString &toDir)
{
    //
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);

    // 目标文件夹有效性检测
    if (!targetDir.exists()) {
        if (!targetDir.mkpath(targetDir.absolutePath())) {
            return false;       // 创建失败
        }
    }

    // 递归复制整个文件夹
    sourceDir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QListIterator<QFileInfo> citerFileInfo(sourceDir.entryInfoList());
    while (citerFileInfo.hasNext()) {
        const QFileInfo &fileInfo = citerFileInfo.next();
        const QString targetPath = targetDir.filePath(fileInfo.fileName());
        if (fileInfo.isFile()) {
            //
            if (targetDir.exists(fileInfo.fileName())) {
                targetDir.remove(fileInfo.fileName());
            }
            //
            if (QFile::copy(fileInfo.filePath(), targetPath)) {
                return false;       // error
            }
        } else {
            if (!copyDir(fileInfo.filePath(), targetPath)) {
                return false;       // errot

            }
        }
    }

    return true;
}

bool JFrameFacade::deleteDir(const QString &path)
{
    // 检测参数有效性
    if (path.isEmpty()) {
        return false;
    }

    // 路径有效性检测
    QDir dir(path);
    if (!dir.exists()) {
        return true;    // 文件夹不存在
    }

    // 递归删除真个文件夹
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QListIterator<QFileInfo> citerFileInfo(dir.entryInfoList());
    while (citerFileInfo.hasNext()) {
        const QFileInfo &fileInfo = citerFileInfo.next();
        if (fileInfo.isFile()) {
            fileInfo.dir().remove(fileInfo.fileName());
        } else {
            deleteDir(fileInfo.absoluteFilePath());
        }
    }

    //
    return dir.rmdir(dir.absolutePath());
}

bool JFrameFacade::testAnotherApp()
{
#ifdef _MSC_VER
    static QSharedMemory sharedMemory("C2.exe");
    if (sharedMemory.create(100)) {
        return false;
    } else {
        return true;
    }
#else
    //
    return false;
#endif
}

bool JFrameFacade::loadTextCodecConfig()
{
    if (!QFileInfo(QString::fromStdString(data->frameGlobalPath)).isReadable()) {
        Q_ASSERT_X(false, "Warning", QStringLiteral("配置文件不存在，即将退出软件！")
                   .toUtf8().data());
        return false;
    }

    // 读取配置文件
    TiXmlDocument document(data->frameGlobalPath);
    if (!document.LoadFile(TIXML_ENCODING_UTF8)) {
        Q_ASSERT_X(false, "Warning",
                   QStringLiteral("配置文件打\"%1\"开失败，即将退出软件！\n错误标识：%2\n错误描述：%3\n错误位置：[%4, %5]")
                   .arg(QString::fromStdString(data->frameGlobalPath))
                   .arg(document.ErrorId()).arg(QString::fromLatin1(document.ErrorDesc()))
                   .arg(document.ErrorRow()).arg(document.ErrorCol())
                   .toUtf8().constData());
        return false;
    }

    // 获取根节点
    TiXmlElement *emRoot = document.RootElement();
    if (!emRoot) {
        return false;
    }

    // 获取框架文本编码节点
    TiXmlElement *emTextCodec = emRoot->FirstChildElement("textCodec");
    if (!emTextCodec) {
        return false;
    }

    std::string sVal;

    // framework encoding
    if (emTextCodec->QueryStringAttribute("encoding", &sVal) != TIXML_SUCCESS) {
        //
    }

    //
    QString codeForName = QString::fromUtf8(emTextCodec->Attribute("encoding"));
    if (codeForName.isEmpty()) {
        codeForName = "utf-8";    // default
    }

    // text codec
    QTextCodec::setCodecForLocale(QTextCodec::codecForName(codeForName.toUtf8()));
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
#endif

    return true;
}

bool JFrameFacade::loadFrameworkDirMethods()
{
    bool result = true;

    //
    QLibrary lib(QString::fromStdString(std::string(_static_appDirPath()).append("/")
                                        .append(dynamicPrefix()).append("jframeworkdir").append(dynamicSuffix(false))));
    if (!lib.load()) {
        Q_ASSERT(false);
        return false;
    }

    // thisDirPath
    if (result) {
        data->fThisDirPath = (ThisDirPath)lib.resolve("thisDirPath");
        if (!data->fThisDirPath) {
            result = false;
        }
    }
    // frameDirPath
    if (result) {
        data->fFrameDirPath = (FrameDirPath)lib.resolve("frameDirPath");
        if (!data->fFrameDirPath) {
            result = false;
        }
    }
    // frameVersion
    if (result) {
        data->fFrameVersion = (FrameVersion)lib.resolve("frameVersion");
        if (!data->fFrameVersion) {
            result = false;
        }
    }

    return result;
}

bool JFrameFacade::invokeLog(const std::string &method, int argc, va_list ap)
{
    // 检测框架内核系统部件有效性
    if (!data->frameKernel) {
        return false;       // 框架内核系统部件无效
    }

    // 参数个数检测
    if (argc < 3) {
        return false;   // 无效 (type, msg, where, ...)
    }

    // 解析各参数值
    const char *type = va_arg(ap, char*);
    const char *msg = va_arg(ap, char *);
    const char *where = va_arg(ap, char *);
    const char *file = 0;
    int line = 0;
    const char *func = 0;
    if (argc > 3) {
        file = va_arg(ap, char *);
    }
    if (argc > 4) {
        line = va_arg(ap, int);
    }
    if (argc > 5) {
        func = va_arg(ap, char *);
    }

    // 转到框架内核系统
    return data->frameKernel->invokeMethod(
                method, argc, type, msg, where, file, line, func);
}

bool JFrameFacade::invokeExitFrame()
{
    // 释放框架系统部件
    releaseInterface();

    // 销毁框架内核系统部件
    if (data->frameKernel) {
        releaseFrameInterface("jframe_kernel");
        data->frameKernel = 0;
    }

    // 销毁框架工厂系统部件
    if (data->frameFactory) {
        releaseFrameInterface("jframe_factory");
        data->frameFactory = 0;
    }

    // 响应Qt事务
    QCoreApplication::processEvents();

    // 强制退出当前进程
    terminateProcess();

    return true;
}

bool JFrameFacade::invokeRestartFrame()
{
    // 转换参数
    QStringList args;
    std::list<std::string>::const_iterator citer = data->arguments.begin();
    for (; citer != data->arguments.end(); ++citer) {
        args.append(QString::fromStdString(*citer));
    }

    // 清空参数列表
    data->arguments.clear();

    // 重启框架
    QProcess::startDetached(QCoreApplication::applicationFilePath(), args);

    // 退出当前框架
    return invokeExitFrame();
}

bool JFrameFacade::invokeLibraryQueryExists(int argc, va_list ap)
{
    // 参数有效性检测
    if (argc < 2) {
        return false;   // 参数无效
    }

    // 获取库文件路径
    const char* dir = va_arg(ap, char*);
    if (!dir) {
        return false;   // 参数无效
    }

    // 获取库名称
    const char* fileName = va_arg(ap, char*);
    if (!fileName) {
        return false;   // 参数无效
    }

    // 生成文件路径
    const QString filePath = QString("%1/%2%3%4")
            .arg(dir).arg(dynamicPrefix().c_str()).arg(fileName)
            .arg(dynamicSuffix().c_str());

    // 检测文件是否存在
    if (!QFile::exists(filePath)) {
        return false;   // 不存在
    }

    // 检测文件是否是库文件
    return QLibrary::isLibrary(filePath);
}

bool JFrameFacade::invokeLibraryResolve(int argc, va_list ap)
{
    // 参数有效性检测
    if (argc < 4) {
        return false;   // 参数无效
    }

    // 获取库文件路径
    const char* dir = va_arg(ap, char*);
    if (!dir) {
        return false;   // 参数无效
    }

    // 获取库名称
    const char* fileName = va_arg(ap, char*);
    if (!fileName) {
        return false;   // 参数无效
    }

    // 获取导出函数名称
    const char* funcName = va_arg(ap, char*);
    if (!funcName) {
        return false;   // 参数无效
    }

    // 获取回传参数
    void **pFunc = va_arg(ap, void**);
    if (!pFunc) {
        return false;   // 参数无效
    }

    // 获取导出函数地址
    void* tmpFunc = (void *)QLibrary::resolve(
                QString("%1/%2%3").arg(dir).arg(fileName).arg(dynamicSuffix().c_str()),
                funcName);
    if (!tmpFunc) {
        return false;   // 获取失败
    }

    *pFunc = tmpFunc;

    return true;
}

JFrameFacade::JFrameFacade()
{
    data = new JFrameFacadeData;

    // 加载 jframeworkdir 库接口
    if (!loadFrameworkDirMethods()) {
        Q_ASSERT(false);
    }

    // 初始化框架版本
    if (data->fFrameVersion) {
        data->frameVersion = data->fFrameVersion();
    }

    // 初始化各路径
    data->appDirPath = _static_appDirPath();
    if (data->fThisDirPath) {
        data->thisDirPath = data->fThisDirPath();
    }
    if (data->fFrameDirPath) {
        data->frameDirPath = data->fFrameDirPath();
    }

    // 初始化框架各配置文件路径
    data->configDirPath = data->thisDirPath + "/config";
    data->frameGlobalPath = data->configDirPath + "/frame/jframe_global.xml";
    data->frameLayoutPath = data->configDirPath + "/frame/jframe_layout.xml";

    // 加载Qt系统文本编码配置信息
    if (!loadTextCodecConfig()) {
        // 加载失败
    }
}

JFrameFacade::~JFrameFacade()
{
    // 销毁框架内核系统部件
    if (data->frameKernel) {
        releaseFrameInterface("jframe_kernel");
        data->frameKernel = 0;
    }

    // 销毁框架工厂系统部件
    if (data->frameFactory) {
        releaseFrameInterface("jframe_factory");
        data->frameFactory = 0;
    }

    delete data;
}
