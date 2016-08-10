#include "precomp.h"
#include "jframe_facade_p.h"

// struct JFrameFacadeData

struct JFrameFacadeData
{
    bool frameLoaded;           // 框架加载状态
    IJUnknown* frameKernel;     // 框架内核系统部件
    IJUnknown* frameFactory;    // 框架工厂系统部件
    IJUnknown* frameLayout;     // 框架布局系统部件
    IJUnknown* frameLogin;      // 框架登录系统部件

    std::string frameDirPath;   // 框架部署路径
    std::string frameVersion;   // 框架版本

    //
    std::string frameGlobalPath;    // 框架全局配置文件路径
    std::string frameLayoutPath;    // 框架布局配置文件路径
    std::string frameComponentPath; // 框架组件加载配置文件路径

    //
    std::list<std::string> arguments;   // 重启框架命令行参数临时存放变量

    JFrameFacadeData():
        frameLoaded(false),
        frameKernel(0),
        frameFactory(0),
        frameLayout(0),
        frameLogin(0)
    {

    }
};

// class JFrameFacade

//
QMutex JFrameFacade::_instance_mutex;
JFrameFacade* JFrameFacade::_instance;

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
            delete JFrameFacade::_instance;
            JFrameFacade::_instance = 0;
        }
        JFrameFacade::_instance_mutex.unlock();
    }
}

void JFrameFacade::releaseInterface()
{
    // 释放框架登录部件
    if (data->frameLogin) {
        data->frameLogin->releaseInterface();
    }
    // 释放框架布局部件
    if (data->frameLayout) {
        data->frameLayout->releaseInterface();
    }
    // 释放框架工厂部件
    if (data->frameFactory) {
        data->frameFactory->releaseInterface();
    }
    // 释放框架内核部件
    if (data->frameKernel) {
        data->frameKernel->releaseInterface();
    }
}

void *JFrameFacade::queryInterface(const char *iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJObject, iid, ver);
    J_QUERY_MEMBER_OBJECT(IJUnknown, iid, ver, data->frameKernel);
    J_QUERY_MEMBER_OBJECT(IJUnknown, iid, ver, data->frameFactory);
    J_QUERY_MEMBER_OBJECT(IJUnknown, iid, ver, data->frameLayout);
    J_QUERY_MEMBER_OBJECT(IJUnknown, iid, ver, data->frameLogin);

    return 0;
}

std::string JFrameFacade::objectIdentity() const
{
    return IID_IJFrameFacade
}

unsigned int JFrameFacade::objectVersion() const
{
    return VER_IJFrameFacade;
}

bool JFrameFacade::invoke(const char *method, int argc)
{
    if (!method) {
        return false;
    }

    bool result = false;
    va_list ap;
    va_start(ap, argc);

    // 输出一条日志
    if (strcmp(method, "log") == 0) {
        result = invokeLog(method, argc, ap);
    }
    // 退出框架（同步方式）
    else if (strcmp(method, "frame_exit") == 0) {
        result = invokeExitFrame();
    }
    // 重启框架（同步方式）
    else if (strcmp(method, "frame_restart") == 0) {
        result = invokeRestartFrame();
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

std::string JFrameFacade::frameGlobalPath() const
{
    return data->frameGlobalPath;
}

std::string JFrameFacade::frameLayoutPath() const
{
    return data->frameLayoutPath;
}

std::string JFrameFacade::frameComponentPath() const
{
    return data->frameComponentPath;
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

bool JFrameFacade::loadFrame(const char *version)
{
    // 检测参数有效性
    if (!version) {
        return false;   // 参数无效
    }

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

void JFrameFacade::showFrame(bool show, bool maximumed)
{
    // 提升对象
    IJObject *frameLayout = dynamic_cast<IJObject *>(data->frameLayout);
    if (!frameLayout) {
        return;     // error
    }

    // 转到框架布局系统
    frameLayout->invoke("frame_show", 2, show, maximumed);
}

void JFrameFacade::tryExitFrame()
{
    // 提升对象
    IJObject *frameLayout = dynamic_cast<IJObject *>(data->frameLayout);
    if (!frameLayout) {
        return;     // error
    }

    // 转到框架布局系统
    frameLayout->invoke("frame_try_exit");
}

void JFrameFacade::exitFrame()
{
    // 提升对象
    IJObject *frameLayout = dynamic_cast<IJObject *>(data->frameLayout);
    if (!frameLayout) {
        return;     // error
    }

    // 转到框架布局系统
    //（通过在布局系统中异步中转会facade，来实现框架的退出，防止部件自身拥有时的释放出错）
    frameLayout->invoke("frame_exit");
}

void JFrameFacade::restartFrame(const std::string<std::string> &arguments)
{
    // 提升对象
    IJObject *frameLayout = dynamic_cast<IJObject *>(data->frameLayout);
    if (!frameLayout) {
        return;     // error
    }

    // 暂存参数
    data->arguments = arguments;

    // 转到框架布局系统
    //（通过在布局系统中异步中转会facade，来实现框架的退出，防止部件自身拥有时的释放出错）
    frameLayout->invoke("frame_restart");
}

bool JFrameFacade::loginFrame()
{
    // 检测框架是否已加载
    if (!data->frameLoaded) {
        return false;       // 未加载
    }

    //  提升对象
    IJObject *frameLogin = dynamic_cast<IJObject *>(data->frameLogin);
    if (!frameLogin) {
        return false;       // 提升失败
    }

    // 转到框架登录系统
    return frameLogin->invoke("frame_login");
}

bool JFrameFacade::logoutFrame()
{
    // 检测框架是否已加载
    if (!data->frameLoaded) {
        return false;       // 未加载
    }

    //  提升对象
    IJObject *frameLogin = dynamic_cast<IJObject *>(data->frameLogin);
    if (!frameLogin) {
        return false;       // 提升失败
    }

    // 转到框架登录系统
    return frameLogin->invoke("frame_logout");
}

std::string JFrameFacade::getEnvValue(const char *name) const
{
    QByteArray env = qgetenv(name);
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

    //
    if (!loadFrameLayout()) {
        return -1;      // 布局系统未加载
    }

    //  提升对象
    IJObject *frameLayout = dynamic_cast<IJObject *>(data->frameLayout);
    if (!frameLayout) {
        return false;       // 提升失败
    }

    // 转到框架布局系统
    int ret = 0;
    if (!frameLayout->invoke("run_q_app", 2, mfcApp, &ret)) {
        return -1;
    }

    return ret;
}

long JFrameFacade::windowHandle(void *window, const char *winType)
{
    // 参数检测
    if (!window || !winType) {
        return 0;   // 参数无效
    }

    // 检测框架是否已加载
    if (!data->frameLoaded) {
        return 0;   // 未加载
    }

    // 加载框架布局系统
    if (!loadFrameLayout()) {
        return 0;   // 加载失败
    }

    //  提升对象
    IJObject *frameLayout = dynamic_cast<IJObject *>(data->frameLayout);
    if (!frameLayout) {
        return 0;   // 提升失败
    }

    // 转到框架布局系统
    long handle = 0;
    if (!frameLayout->invoke("window_handle", 3, window, winType, &handle)) {
        return -1;  // 调用失败
    }

    return handle;
}

bool JFrameFacade::loadFramePrivate()
{
    // 赋值框架动态库到应用实例响应位置，进行框架动态更新
    copyFrameFile();

    bool result = true;

    // 加载核心系统
    result = result && loadFrameKernel();

    // 加载工厂系统
    result = result && loadFrameFactory();

    // 加载布局系统
    result = result && loadFrameLayout();

    // 加载登录系统
    result = result && loadFrameLogin();

    //
    data->frameLoaded = result;

    return result;
}

IJUnknown *JFrameFacade::loadFrameInterface(const std::string &moduleName)
{
    // 定义导出接口
    typedef IJUnknown* (*FunctionUnknown)();

    // 获取导出接口
    FunctionUnknown func = (FunctionUnknown)QLibrary::resolve(
                QString::fromStdString(frameDirPath() + "/bin" + moduleName + dynamicSuffix()),
                ("_func_" + moduleName).c_str());
    if (!func) {
        return 0;       // 获取失败
    }

    // 获取模块实例
    return func();
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

bool JFrameFacade::loadFrameLayout()
{
    // 检测框架内核系统是否已加载
    if (data->frameLayout) {
        return true;    // 已加载
    }

    // 获取接口实例
    data->frameLayout = loadFrameInterface("jframe_layout");
    if (!data->frameLayout) {
        return false;   // 加载失败
    }

    return true;
}

bool JFrameFacade::loadFrameLogin()
{
    // 检测框架登录系统是否已加载
    if (data->frameLogin) {
        return true;    // 已加载
    }

    // 获取接口实例
    data->frameLogin = loadFrameInterface("jframe_login");
    if (!data->frameLogin) {
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
                      .toStdString())
}

void JFrameFacade::copyFrameFile()
{
    QString srcdir = QString::fromStdString(frameDirPath().append("/bin").toLower());
    QString dstdir = QString::fromStdString(appDirPath().toLower());
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
    TixmlDocument document(frameGlobalPath());
    if (!document.LoadFile(TIXML_ENCODING_UTF8)) {
        Q_ASSERT_X(false, "Warning", "配置文件打开失败，即将退出软件！");
        return false;
    }

    // 获取根节点
    TixmlElement *emRoot = document.RootElement();
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
    TixmlDocument document(frameGlobalPath());
    if (!document.LoadFile(TIXML_ENCODING_UTF8)) {
        Q_ASSERT_X(false, "Warning", "配置文件加载错误，即将推出软件！");
        return false;
    }

    // 获取根节点
    TixmlElement *emRoot = document.RootElement();
    if (!emRoot) {
        return false;
    }

    // 获取环境变量配置项
    TixmlElement *emEnvval = emRoot.FistChildElement("envval");
    if (!emEnvval) {
        return false;
    }

    // 获取全局环境变量配置节点
    TixmlElement *emGlobal = emEnvval->FistChildElement("global");
    if (!emGlobal) {
        return false;
    }

    //
    std::string envPath = getEnvValue("Path");
    std::string paths = this->frameDirPath() + "/bin";

    //
    for (TixmlElement *emPath = emGlobal->FirstChildElement("path");
         emPath != 0;
         emPath = emPath->NextSiblingElement("path")) {
        // 获取base属性值
        std::string pathBase;
        if (emPath->QueryStringAttribute("base", &pathBase) != TIXML_SUCCESS
                || pathBase.empty()) {
            continue;
        }
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
    for (TixmlElement *emItem = emPath->FirstChildElement("item");
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

    //
    paths += ';' + envPath;

    // 设置环境变量
    qputenv("Path", QByteArray(path.c_str()));

    /// for qt.conf

    // 获取qtconf节点
    TixmlElemet *emQtConf = emEnvval->FirstChildElement("qtconf");
    if (!emQtConf) {
        return false;
    }

    // 获取paths节点
    TixmlElement *emPaths = emQtConf->FirstChildElement("paths");
    if (!emPaths) {
        return false;
    }

    //
    std::map<std::string, std::string> values;

    // 获取item节点
    for (TixmlElement *emItem = emPaths->FirstChildElement("item");
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
        if (emItem->QueryStringAtrribute("value", &sValue) != TIXML_SUCCESS
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

    return true;
}

bool JFrameFacade::generateAppQtConf(const std::map<std::string, std::sting> &values)
{
    std::string filePath = appDirPath() = "/qt.conf";
    QSettings settings(QString::fromStdString(filePath), QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForLocale());
    settings.beginGroup(Paths);
    std::map<std::string, std::string>::const_iterator citer(values.begin());
    for (; citer != values.end(); ++citer) {
        settings.setValue(QString::fromStdString(citer->first),
                          Qstring::fromStdString(citer->second));
        //
        QApplication::addLibraryPath(QString::fromStdString(citer->second));
    }
    settings.endGroup();

    return true;
}

std::string JFrameFacade::dynamicSuffix() const
{
#ifdef _MSC_VER
#if defined(_DEBUG) || defined(DEBUG)
    return "d";
#else
    return "";
#endif
#else
    return "";
#endif
}

bool JFrameFacade::terminateProcess()
{
    // 释放框架门面系统部件单实例
    releaseInstance();

#ifdef _MSC_VER
    // 强制退出当前进程
    return !!::TerminateProcess(::GetCurrentProcess(), 0);
#else
    // 退出Qt系统
    QApplication::quit();
#endif
}

bool JFrameFacade::loadDefaultSystem()
{
    // 提升框架布局系统部件实例接口
    IJObject *frameLayout = dynamic_cast<IJObject *>(data->frameLayout);
    if (!frameLayout) {
        return false;
    }

    // 转到框架布局系统
    return frameLayout->invoke("load_default_system");
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
    QListIterator<QFileInfo> citerFileInfo(sourceDit.entryInfoList());
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

bool JFrameFacade::invokeLog(const char *method, int argc, va_list ap)
{
    // 检测框架内核系统部件有效性
    if (!data->frameKernel) {
        return false;       // 框架内核系统部件无效
    }

    // 提升框架内核系统部件实例接口类型
    IJObject *frameKernel = dynamic_cast<IJObject *>(data->frameKernel);
    if (!frameKernel) {
        return false;
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
    return frameKernel->invoke(method, argc, type, msg, file, line, func);
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
        args.append(QString::fromStdSting(*citer));
    }

    // 清空参数列表
    data->arguments.clear();

    // 重启框架
    QProcess::startDetached(QApplication::applicationFilePath(), args);

    // 退出当前框架
    invokeExitFrame();
}

JFrameFacade::JFrameFacade()
{
    data = new JFrameFacade();

    // 初始化Qt系统文本编码
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
#else
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
#endif

    // 获取框架配置文件路径前缀
    std::string configDir = QDir(QString::fromStdString(appDirPath().append("/../config")))
            .absolutePath().toStdString();

    // 初始化框架各配置文件路径
    data->frameGlobalPath = configDir + "/jframe_global.xml";
    data->frameLayoutPath = configDir + "/jframe_layout.xml";
    data->frameComponentPath = configDir + "/jframe_component.xml";
}

JFrameFacade::~JFrameFacade()
{
    delete data;
}
