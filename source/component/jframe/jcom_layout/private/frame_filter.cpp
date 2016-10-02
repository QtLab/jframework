#include "precomp.h"
#include "frame_filter.h"
#include "factory/jnotifier.h"
#include "kernel/jframe_kernel.h"
#include "kernel/jframe_core.h"

// struct FrameFilterData

struct FrameFilterData
{
    IJAttempter &attempter;
    QWidget *mainWindow;

    FrameFilterData(IJAttempter &_attempter)
        : attempter(_attempter)
        , mainWindow(0)
    {

    }
};

// class FrameFilter

FrameFilter::FrameFilter(IJAttempter *attempter, QObject *parent)
    : QObject(parent)
{
    data = new FrameFilterData(*attempter);

    // 解析主窗口
    data->mainWindow = parseMainWindow();
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
    if (jloginManager()->isValid()) {
        const QString windowTitle = QString("%1 —— %1@%2")
                .arg(QString::fromStdString(jloginManager()->currentSystem()))
                .arg(QString::fromStdString(jloginManager()->userName()))
                .arg(QString::fromStdString(jloginManager()->currentSeat()));
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

void FrameFilter::attachEventFilter()
{
    if (data->mainWindow) {
        data->mainWindow->installEventFilter(this);
    }
}

void FrameFilter::detachEventFilter()
{
    if (data->mainWindow) {
        data->mainWindow->removeEventFilter(this);
    }
}

bool FrameFilter::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == data->mainWindow) {
        switch (event->type()) {
        case QEvent::Resize:
            data->attempter.notifier().postMessage("j_mainwindow_resized");
            break;
        case QEvent::Move:
            data->attempter.notifier().postMessage("j_mainwindow_moved");
            break;
        case QEvent::Close:
        {
            const int result = QMessageBox::warning(data->mainWindow,
                                                    QStringLiteral("关闭软件"),
                                                    QStringLiteral("请选择以下按钮，继续……"),
                                                    QStringLiteral("注销（重启）"),
                                                    QStringLiteral("退出"),
                                                    QStringLiteral("取消"), 1);
            switch (result) {
            case 0:     // 注销（重启）
                data->attempter.notifier().imm().postMessage("jlayout.notify_manager", "j_frame_restart");
                break;
            case 1:     // 退出
                data->attempter.notifier().imm().postMessage("jlayout.notify_manager", "j_frame_exit");
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

QWidget *FrameFilter::parseMainWindow()
{
    //
    if (!data->attempter.mainWindow()) {
        return 0;
    }

    //
    QWidget *widget = reinterpret_cast<QWidget *>(data->attempter.mainWindow()->mainWidget());
    if (!widget || widget->objectName() != "jframeWnd") {
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
    QFile file(QString::fromStdString(jframeFacade()->frameGlobalPath()));
    if (!file.exists()) {
        const QString text = QStringLiteral("框架全局配置文件\"%1\"不存在！")
                .arg(file.fileName());
        QMessageBox::warning(data->mainWindow, QStringLiteral("警告"), text);
        return false;   // 文件不存在
    }

    // 打开文件
    if (!file.open(QFile::ReadOnly)) {
        const QString text = QStringLiteral("框架全局配置文件\"%1\"打开失败！")
                .arg(file.fileName());
        QMessageBox::warning(data->mainWindow, QStringLiteral("警告"), text);
        return false;
    }

    //
    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    QDomDocument document;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        const QString text =
                QStringLiteral("框架全局配置文件\"%1\"解析失败！\n错误描述：%2\n错误位置：（行号：%3，列号：%4）")
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
    QDomElement emMainWindow = emRoot.firstChildElement("mainWindow");
    if (emMainWindow.isNull()) {
        return false;
    }

    return true;
}
