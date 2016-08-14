#include "precomp.h"
#include "jmain_window.h"
#include "jframewnd.h"

// class JMainWindow

JMainWindow::JMainWindow(JAttempter *attempter)
    : q_attempter(attempter)
    , q_frameWnd(0)
{
    //
    q_frameWnd = new JFrameWnd(attempter);
}

JMainWindow::~JMainWindow()
{
    if (q_frameWnd) {
        q_frameWnd->deleteLater();
        q_frameWnd = 0;
    }
}

bool JMainWindow::init()
{
    bool result = true;

    //
    result = result && q_frameWnd->init();

    return result;
}

void JMainWindow::startSplash()
{
    q_frameWnd->splashScreen()->showFullScreen();
    updateSplash();
}

void JMainWindow::finishSplash()
{
    q_frameWnd->splashScreen()->finish(q_frameWnd);
}

void JMainWindow::updateSplash()
{
    QApplication::processEvents();
}

void JMainWindow::releaseInterface()
{
    if (q_frameWnd) {
        q_frameWnd->close();
    }
}

void *JMainWindow::queryInterface(const char *iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJObject, iid, ver);

    return 0;
}

std::string JMainWindow::objectIdentity() const
{
    return IID_IJMainWindow;
}

unsigned int JMainWindow::objectVersion() const
{
    return VER_IJMainWindow;
}

bool JMainWindow::invoke(const char *method, int argc)
{
    if (!method) {
        return false;
    }

    bool result = false;
    va_list ap;
    va_start(ap, argc);

    va_end(ap);

    return result;
}

void JMainWindow::showNormal()
{
    q_frameWnd->showNormal();
}

void JMainWindow::showMinimized()
{
    q_frameWnd->showMinimized();
}

void JMainWindow::showMaximized()
{
    q_frameWnd->showMaximized();
}

void JMainWindow::showFullScreen()
{
    q_frameWnd->showFullScreen();
}

void JMainWindow::closeWindow()
{
    q_frameWnd->close();
}

void JMainWindow::setVisible(bool visible)
{
    q_frameWnd->setVisible(visible);

    //
    restoreWindowState();
}

void JMainWindow::showStaysOnTop(bool stayOnTop)
{
    if (stayOnTop) {
        q_frameWnd->setWindowFlags(q_frameWnd->windowFlags() | Qt::WindowStaysOnTopHint);
    } else {
        q_frameWnd->setWindowFlags(q_frameWnd->windowFlags() & (~Qt::WindowStaysOnTopHint));
    }
}

void JMainWindow::resize(int width, int height)
{
    q_frameWnd->resize(width, height);
}

void *JMainWindow::queryObject(const char *objectName)
{
    return (void *)q_frameWnd->queryObject(QString::fromLatin1(objectName));
}

void *JMainWindow::statusBar()
{
    return (void *)q_frameWnd->statusBar();
}

void JMainWindow::activeView(const char *viewName)
{
    //
    QObject *object = reinterpret_cast<QObject *>(queryObject(viewName));
    if (!object) {
        return;
    }

    //
    if (!object->inherits("QWidget")) {
        return;
    }

    //
    QWidget *widget = qobject_cast<QWidget *>(object);
    if (!widget) {
        return;
    }

    //
    q_frameWnd->setCurrentWidget(widget);
}

void JMainWindow::updateSplashInfo(const char *info)
{
    q_frameWnd->updateSplashInfo(QString::fromLatin1(info));
}

void JMainWindow::createComponentUi(IJComponent *component, const char *xmlName)
{
    Q_UNUSED(component);
    Q_UNUSED(xmlName);
}

void *JMainWindow::mainWidget()
{
    return static_cast<QWidget *>(q_frameWnd);
}

void JMainWindow::setTheme(const char *theme)
{
    q_frameWnd->setTheme(theme);
}

void JMainWindow::saveWindowState()
{

}

void JMainWindow::restoreWindowState()
{

}
