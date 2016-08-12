#include "precomp.h"
#include "frame_filter.h"
#include "IGF_Kernel.h"
#include "kernel/jnotifier.h"

// struct FrameFilterData

struct FrameFilterData
{
    INotifier *notifier;
    QtRibbon::RibbonMainWindow *mainWindow;

    FrameFilterData()
        : notifier(0)
        , mainWindow(0)
    {

    }
};

// class FrameFilter

FrameFilter::FrameFilter(INotifier *notifier, IGF_Attempter *gAttempter, QObject *parent)
    : QObject(parent)
{
    data = new FrameFilterData;

    //
    data->notifier = notifier;

    // ����������
    data->mainWindow = qobject_cast<QtRibbon::RibbonMainWindow *>(parseMainWindow(gAttempter));
    if (data->mainWindow) {
        // ��װ�������¼�����
        data->mainWindow->installEventFilter(this);
    }
}

FrameFilter::~FrameFilter()
{
    delete data;
}

bool FrameFilter::init()
{
    // ��������
    if (!loadConfig()) {
        return false;
    }

    // ���ÿ�������ڱ���
    if (jframeLogin()->loginManager()->isValid()) {
        const QString windowTitle = QString("%1 ���� %1@%2")
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
        case QEvent::Move:
            data->notifier->post("j_mainwindow_moved");
            break;
        case QEvent::Close:
        {
            const int result = QMessageBox::warning(data->mainWindow,
                                                    QStringLiteral("����"),
                                                    QStringLiteral("��ѡ�����°�ť����������"),
                                                    QStringLiteral("ע����������"),
                                                    QStringLiteral("�˳�"),
                                                    QStringLiteral("ȡ��"), 1);
            switch (result) {
            case 0:     // ע����������
                data->notifier->post("jlayout.notify_manager", "j_frame_restart");
                break;
            case 1:     // �˳�
                data->notifier->post("jlayout.notify_manager", "j_frame_exit");
                break;
            case 2:     // ȡ��
            default:    // ����
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
            || !widget->inherits("MainWindow")
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
        const QString text = QStringLiteral("GF�����ͼ�����ļ�\"%1\"��ʧ�ܣ�")
                .arg(file.fileName());
        QMessageBox::warning(data->mainWindow, QStringLiteral("����"), text);
        return false;
    }

    //
    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    QDomDocument document;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        const QString text = QStringLiteral("GF�����ͼ�����ļ�\"%1\"����ʧ�ܣ�\n"
                                            "����������%2\n"
                                            "����λ�ã����кţ�%3���кã�%4��")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning(data->mainWindow, QStringLiteral("����"), text);
        return false;
    }

    // �ر��ļ�
    file.close();

    // ��ȡ���ڵ�
    QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return false;
    }

    // ��ȡMainWindow�ڵ�
    QDomElement emMainWindow = emRoot.firstChildElement("MainWindow");
    if (emMainWindow.isNull()) {
        return false;
    }

    // ��ȡ�������
    if (emMainWindow.hasAttribute("Style")) {
        jframeLayout()->setFrameTheme(emMainWindow.attribute("Style").toStdString().c_str());
    }

    /// for ribbon

    // ��ȡRibbonBar�ڵ�
    QDomElement emRibbonBar = emMainWindow.firstChildElement("RibbonBar");
    if (!emRibbonBar.isNull()) {
        // ��ȡRibbonBar����
        QtRibbon::RibbonBar *ribbonBar = data->mainWindow->ribbonBar();
        if (ribbonBar) {
            // font
            ribbonBar->setFont(QFont("microsoft yahei", 9));
            // visible
            if (emRibbonBar.hasAttribute("visible")) {
                ribbonBar->setVisible(QVariant(emRibbonBar.attribute("visible")).toBool());
            }
            // minimized
            if (emRibbonBar.hasAttribute("minimized")) {
                ribbonBar->setMinimized(QVariant(emRibbonBar.attribute("minimized")).toBool());
            }
        }
    }

    ///

    // ����ԭʼ��С����
    data->mainWindow->resize(1024, 600);

    // stylesheet - background
    data->mainWindow->setStyleSheet(QString("%1#%2{backbground:'#6E7E93';}")
                                    .arg(data->mainWindow->metaObject()->className())
                                    .arg(data->mainWindow->objectName()));

    return true;
}