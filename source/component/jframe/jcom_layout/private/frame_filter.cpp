#include "precomp.h"
#include "frame_filter.h"
#include "QtnRibbonMainWindow.h"
#include "QtnRibbonStyle.h"
#include "IGF_Kernel.h"
#include "kernel/jnotifier.h"

// struct FrameFilterData

struct FrameFilterData
{
    INotifier *notifier;
    QtHosoft::RibbonMainWindow *mainWindow;

    FrameFilterData()
        : notifier(0)
        , mainWindow(0)
    {

    }
};

// class FrameFilter


FrameFilter::FrameFilter(INotifier *nofitier, IGF_Attempter *gAttempter, QObject *parent)
    : QObject(parent)
{
    data = new FrameFilterData;

    //
    data->notifier = notifier;

    // 解析主窗口
    data->mainWindow = qobject_cast<QtHosoft::RibbonMainWindow *>(parseMainWindow(gAttempter));
    if (data->mainWindow) {
        // 安装主窗口事件过滤
        data->mainWindow->installEventFilter(this);
    }
}

FrameFilter::~FrameFilter()
{
    delete data;
}

bool FrameFilter::init()
{
    // 加载配置
    if (!loadConfig()) {
        return false;
    }

    // 设置框架主窗口标题
    if (jframeLogin()->loginManager()->isValid()) {
        const QString windowTitle = QString("%1 —— %1@%2")
                .arg(QString::fromStdString(jframeLogin()->loginManager()->currentSystem()))
                .arg(QString::fromStdString(jframeLogin()->loginManager()->userName()))
                .arg(QString::fromStdString(jframeLogin()->loginManager()->currentSeat()));
        data->mainWindow->setWindowTitle(windowTitle);
    }

    return true;
}

QRect FrameFilter::mainWindowRect() const
{
    if (data->mainWindow) {
        return data->mainWindow->rect();
    }

    return QRect();
}

bool FrameFilter::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == data->mainWindow) {
        switch (event->type()) {
        case QEvent::Resize:
            data->notifier->post("j_mainwindow_resized");
            break;
        case QEvent::Moved:
            data->notifier->post("j_mainwindow_moved");
            break;
        case QEvent::Moved:
        {
            const int result = QMessageBox::warning(data->mainWindow,
                                                    QStringLiteral("警告"),
                                                    QStringLiteral("请选择以下按钮，继续……"),
                                                    QStringLiteral("注销（重启）"),
                                                    QStringLiteral("退出"),
                                                    QStringLiteral("取消"), 1);
            switch (result) {
            case 0:     // 注销（重启）
                data->notifier->post("jlayout.notify_manager", "j_frame_restart");
                break;
            case 1:     // 退出
                data->notifier->post("jlayout.notify_manager", "j_frame_exit");
                break;
            case 2:     // 取消
            default:    // 忽略
                break;
            }
            event->ignore();
            return true;
        }
        default:
            break;
        }
    }

    return false;
}

QWidget *FrameFilter::parseMainWindow(IGF_Attempter *gAttempter)
{
    //
    if (!gAttempter) {
        return 0;
    }

    //
    IGF_MainWindow *mainWindowInterface =
            reinterpret_cast<IGF_MainWindow *>(gAttempter->GetMainWindow());
    if (!mainWindowInterface) {
        return 0;
    }

    //
    QWidget *widget = reinterpret_cast<QWidget *>(mainWindowInterface->GetMainWndHandle());
    if (!widget
            || !widget->inherit("MainWindow")
            || widget->objectName() != "RibbonMainWindow") {
        return 0;
    }

    //

    return widget;
}

bool FrameFilter::loadConfig()
{
    //
    if (!data->mainWindow) {
        return false;
    }

    //
    QFile file(QString::fromStdString(jframeFacade()->frameFramViewPath()));
    if (!file.open(QFile::ReadOnly)) {
        const QString text = QStringLiteral("GF框架视图配置文件\"%1\"打开失败！")
                .arg(file.fileName());
        QMessageBox::warning(data->mainWindow, QStringLiteral("警告"), text);
        return false;
    }

    //
    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    QDomDocument document;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        const QString text = QStringLiteral("GF框架视图配置文件\"%1\"解析失败！\n"
                                            "错误描述：%2\n"
                                            "错误位置：（行号：%3，列好：%4）")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning(data->mainWindow, QStringLiteral("警告"), text);
        return false;
    }

    // 关闭文件
    file.close();

    // 获取根节点
    QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return false;
    }

    // 获取MainWindow节点
    QDomElement emMainWindow = emRoot.firstChildElement("MainWindow");
    if (emMainWindow.isNull()) {
        return false;
    }

    // 获取框架主体
    if (emMainWindow.hasAttribute("Style")) {
        jframeLayout()->setFrameTheme(emMainWindow.attribute("Style").toStdString().c_str());
    }

    /// for ribbon

    // 获取RibbonBar节点
    QDomElement emRibbonBar = emMainWindow.firstChildElement("RibbonBar");
    if (!emRibbonBar.isNull()) {
        // 获取RibbonBar对象
        QtHosoft::RibbonBar *ribbonBar = data->mainWindow->ribbonBar();
        if (ribbonBar) {
            // font
            ribbonBar->setFont(QFont("microsoft yahei", 9));
            // visible
            if (emRibbonBar.hasAttribute("visible")) {
                ribbonBar->setVisible(QVariant(emRibbonBar.attribute("visible")).toBool());
            }
            // minimized
            if (emRibbonBar.hasAttribute("minimized")) {
                ribbonBar->setMinimized(QVarient(emRibbonBar.attribute("minimized")).toBool());
            }
        }
    }

    ///

    // 窗口原始大小调整
    data->mainWindow->resize(1024, 600);

    // stylesheet - background
    data->mainWindow->setStyleSheet(QString("%1#%2{backbground:'#6E7E93';}")
                                    .arg(data->mainWindow->metaObject()->className())
                                    .arg(data->mainWindow->objectName()));

    return true;
}
