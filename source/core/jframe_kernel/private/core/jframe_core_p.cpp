#include "precomp.h"
#include "jframe_core_p.h"
#include "jattempter.h"
#include "qmfcapp.h"

// struct JFrameCoreData

struct JFrameCoreData
{
    IJAttempter *attempter;     //
    QString codeForName;        // 系统编码

    JFrameCoreData()
        : attempter(0)
        , codeForName("GBK")
    {

    }
};

// class JFrameCore

//
QMutex JFrameCore::_instance_mutex;
JFrameCore* JFrameCore::_instance = 0;

JFrameCore* JFrameCore::getInstance()
{
    // 双重检测（防止多线程竞争）
    if (JFrameCore::_instance == 0) {
        JFrameCore::_instance_mutex.lock();
        if (JFrameCore::_instance == 0) {
            JFrameCore* tempInstance = new JFrameCore;
            JFrameCore::_instance = tempInstance;
        }
        JFrameCore::_instance_mutex.unlock();
    }

    return JFrameCore::_instance;
}

void JFrameCore::releaseInstance()
{
    // 双重检测（防止多线程竞争）
    if (JFrameCore::_instance != 0) {
        JFrameCore::_instance_mutex.lock();
        if (JFrameCore::_instance != 0) {
            JFrameCore::_instance_mutex.unlock();
            delete JFrameCore::_instance;
            JFrameCore::_instance_mutex.lock();
            JFrameCore::_instance = 0;
        }
        JFrameCore::_instance_mutex.unlock();
    }
}

std::string JFrameCore::interfaceIdentity() const
{
    return IID_IJFrameCore;
}

unsigned int JFrameCore::interfaceVersion() const
{
    return VER_IJFrameCore;
}

void *JFrameCore::queryInterface(const std::string &iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJUnknown, iid, ver);

    return 0;
}

bool JFrameCore::loadInterface()
{
    bool result = true;

    // 加载框架调度器
    result = result && data->attempter->loadInterface();

    return result;
}

void JFrameCore::releaseInterface()
{
    // 释放框架调度器
    if (data->attempter) {
        data->attempter->releaseInterface();
    }
}

std::list<std::string> JFrameCore::queryMethod() const
{
    std::list<std::string> methods;

    // frame_show
    methods.push_back(std::string("frame_show").append("..."));

    return methods;
}

bool JFrameCore::invokeMethod(const std::string &method, int argc, ...)
{
    bool result = false;
    va_list ap;
    va_start(ap, argc);

    // 显示或隐藏框架主窗口
    if (method == "frame_show") {
        result = invokeShowFrame(argc, ap);
    }
#ifdef _AFXDLL
    // MFC资源保护
    else if (method == "afx_res_lock") {
        if (argc == 1) {
            //
        }
    }
    // MFC资源保护
    else if (method == "afx_res_unlock") {
        if (argc == 1) {
            //
        }
    }
#endif // _AFXDLL
    // 运行Qt消息循环系统
    else if (method == "run_q_app") {
        if (argc == 2) {
            int ret = runQApp(va_arg(ap, void*));
            int *pRet = va_arg(ap, int*);
            if (pRet) {
                *pRet = ret;
                result = true;
            }
        }
    }
    // 获取窗口实例的窗口句柄
    else if (method == "window_handle") {
        if (argc == 3) {
            void *window = va_arg(ap, void*);
            const char* winType = va_arg(ap, char*);
            long handle = invokeWindowHandle(window, winType);
            long *pHandle = va_arg(ap, long*);
            if (pHandle) {
                *pHandle = handle;
                result = true;
            }
        }
    }
    // 创建Qt应用实体
    else if (method == "create_qapp") {
        result = invokeCreateQApp(argc, ap);
    }

    va_end(ap);

    return result;
}

IJAttempter *JFrameCore::attempter()
{
    return data->attempter;
}

bool JFrameCore::loadConfig()
{
    if (!QFileInfo(QString::fromStdString(jframeFacade()->frameGlobalPath())).isReadable()) {
        Q_ASSERT_X(false, "Warning", "配置文件不存在，即将退出软件！");
        return false;
    }

    // 读取配置文件
    TiXmlDocument document(jframeFacade()->frameGlobalPath());
    if (!document.LoadFile(TIXML_ENCODING_UTF8)) {
        Q_ASSERT_X(false, "Warning",
                   QStringLiteral("配置文件打\"%1\"开失败，即将退出软件！\n"
                                  "错误标识：%2\n"
                                  "错误描述：%3\n"
                                  "错误位置：[%4, %5]")
                   .arg(QString::fromStdString(jframeFacade()->frameGlobalPath()))
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

    // 获取框架文本编码节点
    TiXmlElement *emTextCodec = emRoot->FirstChildElement("textCodec");
    if (!emTextCodec) {
        return false;
    }

    std::string sVal;

    // framework encoding
    if (emTextCodec->QueryStringAttribute("encoding", &sVal) != TIXML_SUCCESS) {
        return false;
    }

    //
    data->codeForName = QString::fromUtf8(emTextCodec->Attribute("encoding"));

    return true;
}

bool JFrameCore::invokeShowFrame(int argc, va_list ap)
{
    // 参数有效性检测
    if (argc != 2) {
        return false;
    }

    //
    if (!data->attempter || !data->attempter->mainWindow()) {
        return false;
    }

    //
    bool show = va_arg(ap, bool);
    bool maximized = va_arg(ap, bool);

    //
    if (show) {
        if (maximized) {
            data->attempter->mainWindow()->showMaximized();
        } else {
            data->attempter->mainWindow()->setVisible(true);
        }
    } else {
        data->attempter->mainWindow()->setVisible(false);
    }

    return true;
}

int JFrameCore::runQApp(void *mfcApp)
{
#if defined(_AFXDLL)
    return QMfcApp::run(reinterpret_cast<CWinApp *>(mfcApp));
#else
    Q_UNUSED(mfcApp);
    return 0;
#endif
}

bool JFrameCore::invokeCreateQApp(int argc, va_list ap)
{
    if (argc != 3) {
        return false;   // 参数无效
    }

    int *argc1 = va_arg(ap, int*);
    char **argv = va_arg(ap, char**);
    void *app = va_arg(ap, void*);
#ifdef _AFXDLL
    if (app) {
        QMfcApp::instance((CWinApp *)app);
    } else
#endif
        if (argc1) {
            (void)new QMfcApp(0, *argc1, argv);
        } else {
            int argc1 = 0;
            (void)new QMfcApp(0, argc1, argv);
        }

    return true;
}

long JFrameCore::invokeWindowHandle(void *window, const std::string &winType)
{
    // 参数有效性检测
    if (!window) {
        return 0;   // 参数无效
    }

    // 获取QWidget实例窗口句柄
    if (winType == "QWidget") {
        QWidget *widget = reinterpret_cast<QWidget *>(window);
        if (widget) {
            return (long)widget->winId();
        }
    }
#ifdef _AFXDLL
    else if (winType == "CWnd") {
        CWnd *wnd = reinterpret_cast<CWnd *>(window);
        if (wnd) {
            return (long)wnd->GetSafeHwnd();
        }
    }
#endif

    return 0;
}

JFrameCore::JFrameCore()
{
    data = new JFrameCoreData;

    //
    data->attempter = new JAttempter();

    // 加载配置信息
    if (!loadConfig()) {
        // 加载失败
    }

    // text codec
    QTextCodec::setCodecForLocale(
                QTextCodec::codecForName(data->codeForName.toLocal8Bit().data()));
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
#endif
}

JFrameCore::~JFrameCore()
{
    //
    if (data->attempter) {
        delete data->attempter;
        data->attempter = 0;
    }

    delete data;
}
