#include "precomp.h"
#include "jmain_window.h"
#include "jframewnd.h"
#include <QtXml>
#include "../layout/jframe_layout_p.h"

// class JMainWindow

JMainWindow::JMainWindow(JAttempter *attempter)
    : q_attempter(attempter)
    , q_frameWnd(0)
{

}

JMainWindow::~JMainWindow()
{
    if (q_frameWnd) {
        //q_frameWnd->deleteLater();
        delete q_frameWnd;
        q_frameWnd = 0;
    }
}

std::string JMainWindow::interfaceIdentity() const
{
    return IID_IJMainWindow;
}

unsigned int JMainWindow::interfaceVersion() const
{
    return VER_IJMainWindow;
}

void *JMainWindow::queryInterface(const std::string &iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJUnknown, iid, ver);

    return 0;
}

bool JMainWindow::loadInterface()
{
    bool result = true;

    //
    if (!q_frameWnd) {
        q_frameWnd = new JFrameWnd(q_attempter);
    }

    //
    result = result && q_frameWnd->init();

    return result;
}

void JMainWindow::releaseInterface()
{
    if (q_frameWnd) {
        q_frameWnd->close();
    }
}

std::list<std::string> JMainWindow::queryMethod() const
{
    std::list<std::string> methods;

    //

    return methods;
}

bool JMainWindow::invokeMethod(const std::string &method, int argc, ...)
{
    bool result = false;
    va_list ap;
    va_start(ap, argc);

    Q_UNUSED(method);

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

void *JMainWindow::queryObject(const std::string &objectName)
{
    QHash<QString, QObject*>::const_iterator citer =
            q_hashObject.find(QString::fromStdString(objectName));
    if (citer != q_hashObject.end()) {
        return (void *)citer.value();
    } else {
        return 0;
    }
}

void *JMainWindow::statusBar()
{
    return (void *)q_frameWnd->statusBar();
}

void JMainWindow::activeView(const std::string &viewName)
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

void JMainWindow::updateSplashInfo(const std::string &info)
{
    q_frameWnd->updateSplashInfo(QString::fromStdString(info));
}

bool JMainWindow::createComponentUi(IJComponent *component, const std::string &filePath)
{
    // 参数有效性检测
    if (!component) {
        return false;       // 参数无效
    }

    // 打开组件配置文件
    QFile file(QString::fromStdString(filePath));
    if(!file.exists()) {
        return false;       // ignore
    }

    // 打开文件
    if (!file.open(QFile::ReadWrite)) {
        const QString text = QStringLiteral("组件配置文件\"%1\"打开失败！")
                .arg(file.fileName());
        QMessageBox::warning(q_frameWnd, QStringLiteral("警告"), text);
        return false;
        return false;       // 打开失败
    }

    // 解析文件
    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    QDomDocument document;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        const QString text = QStringLiteral("组件配置文件\"%1\"解析失败！\n"
                                            "错误描述：%2\n"
                                            "错误位置：（行号：%3，列号：%4）")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning(q_frameWnd, QStringLiteral("警告"), text);
        return false;
    }

    // 关闭文件
    file.close();

    // 获取根节点
    QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return false;   // 无效
    }

    // 获取ribbonPage节点
    QDomElement emRibbonPage = emRoot.firstChildElement("ribbonPage");
    if (!emRibbonPage.isNull()) {
        if (!createRibbonPage(emRibbonPage)) {
            // ignore
        }
    }

    // 获取window节点
    QDomElement emWindow = emRoot.firstChildElement("window");
    if (!emWindow.isNull()) {
        if (!createComponentUi(component, emWindow)) {
            // ignore
        }
        //
    }

    return true;
}

void *JMainWindow::mainWidget()
{
    return static_cast<QWidget *>(q_frameWnd);
}

void JMainWindow::setTheme(const std::string &theme)
{
    q_frameWnd->setTheme(QString::fromStdString(theme));
}

std::string JMainWindow::toolBarType() const
{
    return q_mainWindowConfig.toolBarType;
}

std::string JMainWindow::layoutType() const
{
    return q_mainWindowConfig.layoutType;
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

bool JMainWindow::loadConfig()
{
    return true;
}

void JMainWindow::saveWindowState()
{

}

void JMainWindow::restoreWindowState()
{

}

bool JMainWindow::createRibbonPage(const QDomElement &emRibbonPage)
{
    //
    if (emRibbonPage.isNull()) {
        return false;   //
    }

    //

    return true;
}

bool JMainWindow::createComponentUi(IJComponent *component, const QDomElement &emWindow)
{
    //
    if (!component || emWindow.isNull()) {
        return false;   //
    }

    //
    for (QDomElement emItem = emWindow.firstChildElement();
         !emItem.isNull();
         emItem = emItem.nextSiblingElement()) {
        // objectName
        if (!emItem.hasAttribute("objectName")) {
            continue;       // ignore
        }
        const QString objectName = emItem.attribute("objectName").trimmed();
        if (objectName.isEmpty()) {
            continue;       // ignore
        }
        // nodeName
        const QString nodeName = emItem.nodeName();
        //
        if (nodeName == "view") {
            // windowType
            if (!emItem.hasAttribute("windowType")) {
                continue;   // ignore
            }
            const QString windowType = emItem.attribute("windowType").trimmed();
            // query ui interface
            IJComponentUi *componentUi = (IJComponentUi *)component
                    ->queryInterface(IID_IJComponentUi, VER_IJComponentUi);
            if (!componentUi) {
                continue;   // ignore
            }
            // create ui
            void *window = componentUi->createWindow(qobject_cast<QWidget *>(q_frameWnd),
                                                     objectName.toStdString());
            if (!window) {
                continue;   // ignore
            }
            //
            if (windowType == "Qt") {
                QWidget *widget = (QWidget *)window;
                //
                q_hashObject.insert(objectName, qobject_cast<QObject *>(widget));
                //
                QStackedWidget *centralWidget = q_frameWnd->stackedWidget();
                if (centralWidget) {
                    centralWidget->addWidget(widget);
                    centralWidget->setCurrentWidget(widget);
                }
                // 挂载组件窗口到框架布局系统
                JFrameLayout::getInstance()->invokeMethod(
                            "attach_component_ui", 2, component, widget);
            } else if (windowType == "MFC") {
                //
            }
        } else {
            //
        }
    }

    return true;
}
