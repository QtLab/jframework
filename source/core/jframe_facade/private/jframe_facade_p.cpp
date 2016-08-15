#include "precomp.h"
#include "jframe_facade_p.h"
#include <QCoreApplication>
#include "tinyxml.h"

// struct JFrameFacadeData

struct JFrameFacadeData
{
    bool frameLoaded;           // 框架加载状态
    IJUnknown* frameFactory;    // 框架工厂系统部件
    IJUnknown* frameKernel;     // 框架内核系统部件

    std::string frameDirPath;   // 框架部署路径
    std::string frameVersion;   // 框架版本

    //
    std::string frameConfigPath;    // 框架配置文件夹路径
    std::string frameGlobalPath;    // 框架全局配置文件路径
    std::string frameLayoutPath;    // 框架布局配置文件路径

    //
    std::string language;           // 软件系统语言（如："zh_CN"）

    //
    std::list<std::string> arguments;   // 重启框架命令行参数临时存放变量

    JFrameFacadeData():
        frameLoaded(false),
        frameFactory(0),
        frameKernel(0)
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

std::string JFrameFacade::frameDirPath() const
{
    return data->frameDirPath;
}

//
Q_CORE_EXPORT QString qAppFileName();   // get application file name

std::string JFrameFacade::appDirPath() const
{
    static std::string _path("");
    if (_path.empty()) {
        // 获取软件实体绝对路径
        _path = QFileInfo(qAppFileName()).absolutePath().toLocal8Bit();
    }

    return _path;
}

std::string JFrameFacade::frameConfigPath() const
{
    return data->frameConfigPath;
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

bool JFrameFacade::loadFrame()
{
    // 检测框架是否已加载
    if (data->frameLoaded) {
        return true;    // 已加载
    }

    // 加载框架配置
    if (!loadGlobalConfig()) {
        return false;   // 加载框架全局配置失败
    }

    // 加载框架
    return loadFramePrivate();
}

bool JFrameFacade::loadFrame(const std::string &version)
{
    // 检测框架是否已加载
    if (data->frameLoaded) {
        return true;    // 已加载
    }

    // 加载框架配置
    if (!loadGlobalConfig(version)) {
        return false;   // 加载框架全局配置失败
    }

    // 加载框架
    return loadFramePrivate();
}

bool JFrameFacade::loadFrame(int major, int minor, int patch)
{
    // 检测框架是否已加载
    if (data->frameLoaded) {
        return true;    // 已加载
    }

    // 加载框架配置
    if (!loadGlobalConfig(major, minor, patch)) {
        return false;   // 加载框架全局配置失败
    }

    // 加载框架
    return loadFramePrivate();
}

void JFrameFacade::showFrame(bool show, bool maximized)
{
    // 转到框架内核系统
    data->frameKernel->invokeMethod("frame_show", 2, show, maximized);
}

void JFrameFacade::tryExitFrame()
{
    // 转到框架内核系统
    data->frameKernel->invokeMethod("frame_try_exit");
}

void JFrameFacade::exitFrame()
{
    // 转到框架内核系统
    //（通过在内核系统中异步中转回facade，来实现框架的退出，防止部件自身拥有时的释放出错）
    data->frameKernel->invokeMethod("frame_exit");
}

void JFrameFacade::restartFrame(const std::list<std::string> &arguments)
{
    // 暂存参数
    data->arguments = arguments;

    // 转到框架内核系统
    //（通过在内核系统中异步中转回facade，来实现框架的退出，防止部件自身拥有时的释放出错）
    data->frameKernel->invokeMethod("frame_restart");
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
                "window_handle", 3, window, winType, &handle)) {
        return -1;  // 调用失败
    }

    return handle;
}

std::string JFrameFacade::language() const
{
    return data->language;
}

bool JFrameFacade::loadFramePrivate()
{
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

    // 加载框架内核系统接口
    result = result && data->frameKernel->loadInterface();

    return result;
}

IJUnknown *JFrameFacade::loadFrameInterface(const std::string &moduleName)
{
    // 定义导出接口
    typedef IJUnknown* (*FuncCreateInstance)();

    // 获取导出接口
    const QString filePath = QString::fromStdString(frameDirPath() + "/bin/"
                                                    + moduleName + dynamicSuffix());
    FuncCreateInstance fCreateInstance =
            (FuncCreateInstance)QLibrary::resolve(filePath, "CreateInstance");

    if (!fCreateInstance) {
        jframeLogWarning(QStringLiteral("模块%1无法加载！（找不到CreateInstance接口或缺少依赖项）")
                         .arg(filePath));
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
    const QString filePath = QString::fromStdString(frameDirPath() + "/bin/"
                                                    + moduleName + dynamicSuffix());
    FuncReleaseInstance fReleaseInstance =
            (FuncReleaseInstance)QLibrary::resolve(filePath, "ReleaseInstance");

    if (!fReleaseInstance) {
        jframeLogWarning(QStringLiteral("模块%1无法加载！（找不到ReleaseInstance接口或缺少依赖项）")
                         .arg(filePath));
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

bool JFrameFacade::loadGlobalConfig()
{
    return loadGlobalConfig(frameVersionFromConfig());
}

bool JFrameFacade::loadGlobalConfig(const std::string &version)
{
    data->frameVersion = version;

    //
    std::string path = framePathFromRegistry(data->frameVersion);
    if (path.empty()) {
        std::string framePath = appDirPath();
        framePath = framePath.substr(0, framePath.size() - 4);
        path = framePath;
    }

    return loadConfig(path);
}

bool JFrameFacade::loadGlobalConfig(int major, int minor, int patch)
{
    return loadConfig(QString("%1.%2.%3")
                      .arg(major).arg(minor).arg(patch)
                      .toStdString());
}

void JFrameFacade::copyFrameFile()
{
    QString srcdir = QString::fromStdString(frameDirPath().append("/bin")).toLower();
    QString dstdir = QString::fromStdString(appDirPath()).toLower();
    if (srcdir == dstdir) {
        return;     // 框架位置与应用实例相同，不需要更新
    }

    /// 拷贝框架组件

    //
    QDir componentSrcDir(srcdir + "/../Component/jframe");
    if (componentSrcDir.exists()) {
        // 删除软件实体中的框架组件文件夹
        QString componentNewDir(dstdir + "/../Component/jframe");
        //deleteDir(componentNewDir);
        copyDir(componentSrcDir.absolutePath(), componentNewDir);
    }
}

std::string JFrameFacade::frameVersionFromConfig() const
{
    if (!QFileInfo(QString::fromStdString(frameGlobalPath())).isReadable()) {
        Q_ASSERT_X(false, "Warning", "配置文件不存在，即将退出软件！");
        return false;
    }

    // 读取配置文件
    TiXmlDocument document(frameGlobalPath());
    if (!document.LoadFile(TIXML_ENCODING_UTF8)) {
        Q_ASSERT_X(false, "Warning",
                   QStringLiteral("配置文件打\"%1\"开失败，即将退出软件！\n"
                                  "错误标识：%2\n"
                                  "错误描述：%3\n"
                                  "错误位置：[%4, %5]")
                   .arg(QString::fromStdString(frameGlobalPath()))
                   .arg(document.ErrorId()).arg(QString::fromLatin1(document.ErrorDesc()))
                   .arg(document.ErrorRow()).arg(document.ErrorCol())
                   .toLocal8Bit().data());
        return false;
    }

    // 获取根节点
    TiXmlElement *emRoot = document.RootElement();
    if (!emRoot) {
        return false;
    }

    std::string sVal;

    // framework config
    if (emRoot->QueryStringAttribute("version", &sVal) != TIXML_SUCCESS) {
        Q_ASSERT_X(false, "Warning", "未指定框架版本，即将退出软件");
        return false;
    }

    return sVal;
}

std::string JFrameFacade::framePathFromRegistry(const std::string &version)
{
#ifdef _MSC_VER
    QSettings settings("HKEY_CURRENT_USER\\Software\\Smartsoft\\JFrame\\"
                       + QString::fromStdString(version),
                       QSettings::NativeFormat);
    QString installPath = settings.value("InstallPath").toString();
    if (installPath.isEmpty()) {
        return std::string();
    } else {
        installPath.append("\\frame");
    }

    //
    installPath.replace('\\', '/');

    return installPath.toStdString();
#else
    Q_UNUSED(version);
#pragma message("在非Windows环境下还未实现此功能！");
    return std::string();
#endif
}

bool JFrameFacade::loadConfig(const std::string &frameDirPath)
{
    // 保存框架路径
    data->frameDirPath = frameDirPath;

    //
    if (!QFileInfo(QString::fromStdString(frameGlobalPath())).isReadable()) {
        Q_ASSERT_X(false, "Warning", "配置文件不存在，即将退出软件！");
        return false;
    }

    // 读取配置文件
    TiXmlDocument document(frameGlobalPath());
    if (!document.LoadFile(TIXML_ENCODING_UTF8)) {
        Q_ASSERT_X(false, "Warning", "配置文件加载错误，即将推出软件！");
        return false;
    }

    // 获取根节点
    TiXmlElement *emRoot = document.RootElement();
    if (!emRoot) {
        return false;
    }

    // 获取环境变量配置项
    TiXmlElement *emEnvval = emRoot->FirstChildElement("envval");
    if (!emEnvval) {
        return false;
    }

    // 获取全局环境变量配置节点
    TiXmlElement *emGlobal = emEnvval->FirstChildElement("global");
    if (!emGlobal) {
        return false;
    }

    //
    std::string envPath = getEnvValue("Path");
    std::string paths = this->frameDirPath() + "/bin";

    // 寻找全局变量配置->path节点
    for (TiXmlElement *emPath = emGlobal->FirstChildElement("path");
         emPath != 0;
         emPath = emPath->NextSiblingElement("path")) {
        // 获取base属性值
        std::string pathBase;
        if (emPath->QueryStringAttribute("base", &pathBase) != TIXML_SUCCESS
                || pathBase.empty()) {
            continue;
        }

        //
        std::string pathPrefix;
        if (pathBase == "jframe") {
            pathPrefix = this->frameDirPath() + "/bin";
        } else if (pathBase == "app") {
            pathPrefix = appDirPath();
        } else {
            continue;   // not supported
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
            path = pathPrefix + '/' + path;
            if (newPaths.empty()) {
                newPaths = path;
            } else {
                newPaths += ';' + path;
            }
        }

        //
        if (pathType == "append") {
            paths += ';' + newPaths;
        } else if (pathType == "prefix") {
            paths = newPaths + ';' + paths;
        } else if (pathType == "replace") {
            paths = newPaths;
        } else {
            //
        }
    }

    //
    paths += ';' + envPath;

    // 设置环境变量
    qputenv("Path", QByteArray(paths.c_str()));

    /// for qt.conf

    // 获取qtconf节点
    TiXmlElement *emQtConf = emRoot->FirstChildElement("qtconf");
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
        std::string sValue;
        if (emItem->QueryStringAttribute("value", &sValue) != TIXML_SUCCESS
                || sValue.empty()) {
            continue;
        }

        //
        if (sKey == "Plugins") {
            if (sValue == "$(jframe)") {
                values[sKey] = this->frameDirPath() + "/bin/kernel/Qt/plugins";
            } else if (sValue == "$(app)") {
                values[sKey] = appDirPath() + "/kernel/Qt/plugins";
            } else {
                values[sKey] = sValue;
            }
        } else if (sKey == "Imports") {
            if (sValue == "$(jframe)") {
                values[sKey] = this->frameDirPath() + "/bin/kernel/Qt/imports";
            } else if (sValue == "$(app)") {
                values[sKey] = appDirPath() + "/kernel/Qt/imports";
            } else {
                values[sKey] = sValue;
            }
        } else if (sKey == "Qml2Imports") {
            if (sValue == "$(jframe)") {
                values[sKey] = this->frameDirPath() + "/bin/kernel/Qt/qml";
            } else if (sValue == "$(app)") {
                values[sKey] = appDirPath() + "/kernel/Qt/qml";
            } else {
                values[sKey] = sValue;
            }
        }
    }

    //
    if (values.empty()) {
        generateAppQtConf(values);
    }

    // 解析系统节点信息

    return true;
}

bool JFrameFacade::generateAppQtConf(const std::map<std::string, std::string> &values)
{
    std::string filePath = appDirPath() = "/qt.conf";
    QSettings settings(QString::fromStdString(filePath), QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForLocale());
    settings.beginGroup("Paths");
    std::map<std::string, std::string>::const_iterator citer(values.begin());
    for (; citer != values.end(); ++citer) {
        settings.setValue(QString::fromStdString(citer->first),
                          QString::fromStdString(citer->second));
        //

        QCoreApplication::addLibraryPath(QString::fromStdString(citer->second));
    }
    settings.endGroup();

    return true;
}

std::string JFrameFacade::dynamicSuffix() const
{
#ifdef _MSC_VER
#if defined(_DEBUG) || defined(DEBUG)
    return "d.dll";
#else
    return ".dll";
#endif
#else
    return ".so";
#endif
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
#endif
}

bool JFrameFacade::loadDefaultSystem()
{
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

bool JFrameFacade::invokeLog(const std::string &method, int argc, va_list ap)
{
    // 检测框架内核系统部件有效性
    if (!data->frameKernel) {
        return false;       // 框架内核系统部件无效
    }

    // 参数个数检测
    if (argc < 2) {
        return false;       // 参数个数无效
    }

    // 解析各参数值
    const char *type = va_arg(ap, const char *);
    const char *msg = va_arg(ap, const char *);
    const char *file = 0;
    int line = 0;
    const char *func = 0;
    if (argc > 2) {
        file = va_arg(ap, const char *);
    }
    if (argc > 3) {
        line = va_arg(ap, int);
    }
    if (argc > 4) {
        func = va_arg(ap, const char *);
    }

    // 转到框架内核系统
    return data->frameKernel->invokeMethod(
                method, argc, type, msg, file, line, func);
}

bool JFrameFacade::invokeExitFrame()
{
    // 释放框架系统部件
    releaseInterface();

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

    // 检测文件是否存在
    return QFile::exists(QString("%1/%2%3")
                         .arg(dir).arg(fileName).arg(dynamicSuffix().c_str()));
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
    void* tmpFunc = (void *)QLibrary::resolve(QString("%1/%2%3")
                                              .arg(dir).arg(fileName).arg(dynamicSuffix().c_str()),
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

    // 初始化Qt系统文本编码
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
#else
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
#endif

    // 获取框架配置文件路径前缀
    data->frameConfigPath = QDir(QString::fromStdString(appDirPath().append("/../config")))
            .absolutePath().toStdString();

    // 初始化框架各配置文件路径
    data->frameGlobalPath = data->frameConfigPath + "/frame/jframe_global.xml";
    data->frameLayoutPath = data->frameConfigPath + "/frame/jframe_layout.xml";
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
