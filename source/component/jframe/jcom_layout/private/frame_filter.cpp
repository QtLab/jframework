#include "precomp.h"
#include "frame_filter.h"
#include "jframe_kernel.h"

// struct FrameFilterData

struct FrameFilterData
{
    INotifier *notifier;
    QWidget *mainWindow;

    FrameFilterData()
        : notifier(0)
        , mainWindow(0)
    {

    }
};

// class FrameFilter

FrameFilter::FrameFilter(INotifier *notifier, IJAttempter *attempter, QObject *parent)
    : QObject(parent)
{
    data = new FrameFilterData;

    //
    data->notifier = notifier;

    // ����������
    data->mainWindow = parseMainWindow(attempter);
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
    if (jloginManager()->isValid()) {
        const QString windowTitle = QString("%1 ���� %1@%2")
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
            data->notifier->post("j_mainwindow_resized");
            break;
        case QEvent::Move:
            data->notifier->post("j_mainwindow_moved");
            break;
        case QEvent::Close:
        {
            const int result = QMessageBox::warning(data->mainWindow,
                                                    QStringLiteral("�ر����"),
                                                    QStringLiteral("��ѡ�����°�ť����������"),
                                                    QStringLiteral("ע����������"),
                                                    QStringLiteral("�˳�"),
                                                    QStringLiteral("ȡ��"), 1);
            switch (result) {
            case 0:     // ע����������
                data->notifier->imm().post("jlayout.notify_manager", "j_frame_restart");
                break;
            case 1:     // �˳�
                data->notifier->imm().post("jlayout.notify_manager", "j_frame_exit");
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

QWidget *FrameFilter::parseMainWindow(IJAttempter *attempter)
{
    //
    if (!attempter || !attempter->mainWindow()) {
        return 0;
    }

    //
    QWidget *widget = reinterpret_cast<QWidget *>(attempter->mainWindow()->mainWidget());
    if (!widget || widget->objectName() != "JFrameWnd") {
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
        const QString text = QStringLiteral("���ȫ�������ļ�\"%1\"�����ڣ�")
                .arg(file.fileName());
        QMessageBox::warning(data->mainWindow, QStringLiteral("����"), text);
        return false;   // �ļ�������
    }

    // ���ļ�
    if (!file.open(QFile::ReadOnly)) {
        const QString text = QStringLiteral("���ȫ�������ļ�\"%1\"��ʧ�ܣ�")
                .arg(file.fileName());
        QMessageBox::warning(data->mainWindow, QStringLiteral("����"), text);
        return false;
    }

    //
    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    QDomDocument document;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        const QString text = QStringLiteral("���ȫ�������ļ�\"%1\"����ʧ�ܣ�\n"
                                            "����������%2\n"
                                            "����λ�ã����кţ�%3���кţ�%4��")
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
    QDomElement emMainWindow = emRoot.firstChildElement("mainWindow");
    if (emMainWindow.isNull()) {
        return false;
    }

    return true;
}
