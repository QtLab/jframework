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
    q_frameWnd->deleteLater();
    q_frameWnd = 0;
}

bool JMainWindow::initialize()
{
    //
    if (!q_frameWnd->init()) {
        return false;
    }

    return true;
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
}

void *JMainWindow::mainWindowHandle()
{
    return static_cast<QtRibbon::RibbonMainWindow *>(q_frameWnd);
}

void JMainWindow::saveWindowState()
{

}

void JMainWindow::restoreWindowState()
{

}
