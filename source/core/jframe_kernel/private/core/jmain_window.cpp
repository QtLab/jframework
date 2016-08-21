#include "precomp.h"
#include "jmain_window.h"
#include "jframewnd.h"
#include <QtXml>
#include "../layout/jframe_layout_p.h"
#include "QtnRibbonGroup.h"
#include "QtnRibbonGroupLayout.h"

// class JMainWindow

JMainWindow::JMainWindow(JAttempter *attempter)
    : q_attempter(attempter)
    , q_frameWnd(0)
{

}

JMainWindow::~JMainWindow()
{
    if (q_frameWnd) {
        q_frameWnd->deleteLater();
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
    if (q_frameWnd) {
        q_frameWnd->showNormal();
    }
}

void JMainWindow::showMinimized()
{
    if (q_frameWnd) {
        q_frameWnd->showMinimized();
    }
}

void JMainWindow::showMaximized()
{
    if (q_frameWnd) {
        q_frameWnd->showMaximized();
    }
}

void JMainWindow::showFullScreen()
{
    if (q_frameWnd) {
        q_frameWnd->showFullScreen();
    }
}

void JMainWindow::closeWindow()
{
    if (q_frameWnd) {
        q_frameWnd->close();
    }
}

void JMainWindow::setVisible(bool visible)
{
    if (q_frameWnd) {
        q_frameWnd->setVisible(visible);

        //
        restoreWindowState();
    }
}

void JMainWindow::showStaysOnTop(bool stayOnTop)
{
    if (q_frameWnd) {
        if (stayOnTop) {
            q_frameWnd->setWindowFlags(q_frameWnd->windowFlags() | Qt::WindowStaysOnTopHint);
        } else {
            q_frameWnd->setWindowFlags(q_frameWnd->windowFlags() & (~Qt::WindowStaysOnTopHint));
        }
    }
}

void JMainWindow::resize(int width, int height)
{
    if (q_frameWnd) {
        q_frameWnd->resize(width, height);
    }
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
    if (q_frameWnd) {
        return (void *)q_frameWnd->statusBar();
    } else {
        return 0;
    }
}

void JMainWindow::activeView(const std::string &viewName)
{
    if (!q_frameWnd) {
        return;
    }

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
    if (q_frameWnd) {
        q_frameWnd->updateSplashInfo(QString::fromStdString(info));
    }
}

bool JMainWindow::createComponentUi(IJComponent *component, const std::string &filePath)
{
    if (!q_frameWnd) {
        return false;
    }

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

    // 获取 ribbon节点
    QDomElement emRibbon = emRoot.firstChildElement("ribbon");
    if (!emRibbon.isNull()) {
        if (!createRibbonPage(emRibbon)) {
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
    if (q_frameWnd) {
        return static_cast<QWidget *>(q_frameWnd);
    } else {
        return 0;
    }
}

void JMainWindow::setTheme(const std::string &theme)
{
    if (q_frameWnd) {
        q_frameWnd->setTheme(QString::fromStdString(theme));
    }
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
    if (q_frameWnd) {
        q_frameWnd->startSplash();
    }
}

void JMainWindow::finishSplash()
{
    if (q_frameWnd) {
        q_frameWnd->finishSplash();
    }
}

void JMainWindow::updateSplash()
{
    if (q_frameWnd) {
        q_frameWnd->updateSplash();
    }
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

bool JMainWindow::createComponentUi(IJComponent *component, const QDomElement &emWindow)
{
    if (!q_frameWnd) {
        return false;
    }

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
                QWidget *widget = reinterpret_cast<QWidget *>(window);
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

bool JMainWindow::createRibbonPage(const QDomElement &emRibbon)
{
    //
    if (emRibbon.isNull()) {
        return false;   //
    }

    //
    for (QDomElement emPage = emRibbon.firstChildElement("page");
         !emPage.isNull();
         emPage = emPage.nextSiblingElement("page")) {
        //
        const QString objectName = emPage.attribute("objectName");
        const QString text = emPage.attribute("text");
        //
        QtRibbon::RibbonPage *ribbonPage = 0;
        if (q_hashObject.contains(objectName)) {
            ribbonPage = qobject_cast<QtRibbon::RibbonPage *>(q_hashObject.value(objectName));
            if(!ribbonPage) {
                ribbonPage = q_frameWnd->ribbonBar()->addPage(text);
            }
        } else {
            ribbonPage = q_frameWnd->ribbonBar()->addPage(text);
        }
        //
        if (ribbonPage)	{
            ribbonPage->setObjectName(objectName);
            if(!objectName.isEmpty()) {
                q_hashObject.insert(objectName, ribbonPage);
            }
            //
            if (!createRibbinGroup(emPage, ribbonPage)) {
                // create failure
            }
        }
    }

    return true;
}

bool JMainWindow::createRibbinGroup(const QDomElement &emPage, QtRibbon::RibbonPage *ribbonPage)
{
    //
    if (emPage.isNull() || !ribbonPage) {
        return false;
    }

    //
    for (QDomElement emGroup = emPage.firstChildElement("group");
         !emGroup.isNull();
         emGroup = emGroup.nextSiblingElement("group")) {
        //
        const QString objectName = emGroup.attribute("objectName");
        const QString text = emGroup.attribute("text");
        //
        QtRibbon::RibbonGroup* ribbonGroup = 0;
        if(q_hashObject.contains(objectName)) {
            ribbonGroup = qobject_cast<QtRibbon::RibbonGroup *>(q_hashObject.value(objectName));
            if(!ribbonGroup) {
                ribbonGroup = ribbonPage->addGroup(text);
                ribbonGroup->setObjectName(objectName);
                if(!objectName.isEmpty()) {
                    q_hashObject.insert(objectName, ribbonGroup);
                }
            }
        } else {
            ribbonGroup = ribbonPage->addGroup(text);
            ribbonGroup->setObjectName(objectName);
            if(!objectName.isEmpty()) {
                q_hashObject.insert(objectName, ribbonGroup);
            }
            //
            if (!createRibbinItem(emGroup, ribbonGroup)) {
                // create failure
            }
        }
    }

    return true;
}

bool JMainWindow::createRibbinItem(const QDomElement &emGroup, QtRibbon::RibbonGroup *ribbonGroup)
{
    //
    if (emGroup.isNull() || !ribbonGroup) {
        return false;
    }

    //
    for (QDomElement emItem = emGroup.firstChildElement();
         !emItem.isNull();
         emItem = emItem.nextSiblingElement())
    {
        const QString nodeName = emItem.nodeName();
        if(nodeName == "action") {
            //
            QAction *action = createItemAction(emItem);
            if (action) {
                ribbonGroup->addAction(action, Qt::ToolButtonTextUnderIcon);
            }
        } else if(nodeName == "subMenu") {
            //
            QMenu *menu = createSubMenu(emItem, ribbonGroup);
            if (!menu) {
                //
            }
        }
        else if(nodeName == "separator") {
            //
            ribbonGroup->addSeparator();
        } else if(nodeName == "comboBox") {
            QComboBox *comboBox = createItemComboBox(emItem);
            if (comboBox) {
                ribbonGroup->addWidget(comboBox);
            }
        } else if(nodeName == "checkBox") {
            //
            QCheckBox *checkBox = createItemCheckBox(emItem);
            if (checkBox) {
                ribbonGroup->addWidget(checkBox);
            }
        }
        else if(nodeName == "radioButton") {
            //
            QRadioButton *radioButton = createItemRadioButton(emItem);
            if (radioButton) {
                ribbonGroup->addWidget(radioButton);
            }
        } else if(nodeName == "label") {
            QLabel *label = createItemLabel(emItem);
            if (label) {
                ribbonGroup->addWidget(label);
            }
        } else if(nodeName == "bitmap") {
            //
            QLabel *label = createItemLabel(emItem);
            if (label) {
                label->setPixmap(QPixmap(parsePath(emItem.attribute("icon"))));
                ribbonGroup->addWidget(label);
            }
        }
    }

    return true;
}

QAction* JMainWindow::createItemAction(const QDomElement &emItem)
{
    //
    if (emItem.isNull()) {
        return 0;
    }

    //
    const QString objectName = emItem.attribute("objectName");

    //
    QAction *action = 0;
    if(q_hashObject.contains(objectName)) {
        action = qobject_cast<QAction *>(q_hashObject.value(objectName));
        if (!action) {
            action = new QAction();
            if(!objectName.isEmpty()) {
                q_hashObject.insert(objectName, action);
            }
        } else {
            //
            QObject::disconnect(action, SIGNAL(triggered(QAction*)),
                                q_frameWnd, SLOT(onActionTriggered(QAction*)));
        }
    } else {
        action = new QAction();
        if(!objectName.isEmpty()) {
            q_hashObject.insert(objectName, action);
        }
    }
    //
    action->setObjectName(objectName);
    action->setText(emItem.attribute("text"));
    action->setIcon(QIcon(parsePath(emItem.attribute("icon"))));
    action->setShortcut(emItem.attribute("shortcut"));
    action->setCheckable(QVariant(emItem.attribute("checkable", "true")).toBool());
    //
    QObject::connect(action, SIGNAL(triggered(QAction*)),
                     q_frameWnd, SLOT(onActionTriggered(QAction*)));

    return action;
}

QComboBox *JMainWindow::createItemComboBox(const QDomElement &emItem)
{
    //
    if (emItem.isNull()) {
        return 0;
    }

    //
    const QString objectName = emItem.attribute("objectName");

    //
    QComboBox *comboBox = 0;
    if(q_hashObject.contains(objectName)) {
        comboBox = qobject_cast<QComboBox *>(q_hashObject.value(objectName));
        if (!comboBox) {
            comboBox = new QComboBox();
            if(!objectName.isEmpty()) {
                q_hashObject.insert(objectName, comboBox);
            }
        } else {
            //
            QObject::disconnect(comboBox, SIGNAL(triggered(QWidget*)),
                                q_frameWnd, SLOT(onComboBoxTriggered(QWidget*)));
        }
    } else {
        comboBox = new QComboBox();
        if(!objectName.isEmpty()) {
            q_hashObject.insert(objectName, comboBox);
        }
    }

    //
    comboBox->setObjectName(objectName);

    // items
    for (QDomElement emComboBoxItem = emItem.firstChildElement("item");
         !emComboBoxItem.isNull();
         emComboBoxItem = emComboBoxItem.nextSiblingElement("item")) {
        //
        comboBox->addItem(QIcon(parsePath(emComboBoxItem.attribute("icon"))),
                          emComboBoxItem.attribute("text"));
    }

    //
    QObject::connect(comboBox, SIGNAL(triggered(QWidget*)),
                     q_frameWnd, SLOT(onComboBoxTriggered(QWidget*)));

    return comboBox;
}

QCheckBox *JMainWindow::createItemCheckBox(const QDomElement &emItem)
{
    //
    if (emItem.isNull()) {
        return 0;
    }

    //
    const QString objectName = emItem.attribute("objectName");

    //
    QCheckBox *checkBox = 0;
    if(q_hashObject.contains(objectName)) {
        checkBox = qobject_cast<QCheckBox *>(q_hashObject.value(objectName));
        if (!checkBox) {
            checkBox = new QCheckBox();
            if(!objectName.isEmpty()) {
                q_hashObject.insert(objectName, checkBox);
            }
        } else {
            //
            QObject::disconnect(checkBox, SIGNAL(triggered(QWidget*)),
                                q_frameWnd, SLOT(onCheckBoxTriggered(QWidget*)));
        }
    } else {
        checkBox = new QCheckBox();
        if(!objectName.isEmpty()) {
            q_hashObject.insert(objectName, checkBox);
        }
    }

    //
    checkBox->setObjectName(objectName);
    checkBox->setText(emItem.attribute("text"));
    checkBox->setChecked(QVariant(emItem.attribute("checked", "false")).toBool());

    //
    QObject::connect(checkBox, SIGNAL(triggered(QWidget*)),
                     q_frameWnd, SLOT(onComboBoxTriggered(QWidget*)));

    return checkBox;
}

QRadioButton *JMainWindow::createItemRadioButton(const QDomElement &emItem)
{
    //
    if (emItem.isNull()) {
        return 0;
    }

    //
    const QString objectName = emItem.attribute("objectName");

    //
    QRadioButton *radioButton = 0;
    if(q_hashObject.contains(objectName)) {
        radioButton = qobject_cast<QRadioButton *>(q_hashObject.value(objectName));
        if (!radioButton) {
            radioButton = new QRadioButton();
            if(!objectName.isEmpty()) {
                q_hashObject.insert(objectName, radioButton);
            }
        } else {
            //
            QObject::disconnect(radioButton, SIGNAL(triggered(QWidget*)),
                                q_frameWnd, SLOT(onCheckBoxTriggered(QWidget*)));
        }
    } else {
        radioButton = new QRadioButton();
        if(!objectName.isEmpty()) {
            q_hashObject.insert(objectName, radioButton);
        }
    }

    //
    radioButton->setObjectName(objectName);
    radioButton->setText(emItem.attribute("text"));
    radioButton->setChecked(QVariant(emItem.attribute("checked", "false")).toBool());

    //
    QObject::connect(radioButton, SIGNAL(triggered(QWidget*)),
                     q_frameWnd, SLOT(onRadioButtonTriggered(QWidget*)));

    return radioButton;
}

QLabel *JMainWindow::createItemLabel(const QDomElement &emItem)
{
    //
    if (emItem.isNull()) {
        return 0;
    }

    //
    const QString objectName = emItem.attribute("objectName");

    //
    QLabel *label = 0;
    if(q_hashObject.contains(objectName)) {
        label = qobject_cast<QLabel *>(q_hashObject.value(objectName));
        if (!label) {
            label = new QLabel();
            if(!objectName.isEmpty()) {
                q_hashObject.insert(objectName, label);
            }
        }
    } else {
        label = new QLabel();
        if(!objectName.isEmpty()) {
            q_hashObject.insert(objectName, label);
        }
    }

    //
    label->setObjectName(objectName);
    label->setText(emItem.attribute("text"));

    return label;
}

QMenu *JMainWindow::createSubMenu(const QDomElement &emItem, QWidget *parentWidget)
{
    //
    if (emItem.isNull() || !parentWidget) {
        return 0;
    }

    //
    const QString objectName = emItem.attribute("objectName");
    const QString text = emItem.attribute("text");
    const QString icon = parsePath(emItem.attribute("icon"));

    //
    QMenu *menu = 0;
    if (parentWidget->inherits("RibbonGroup")) {
        QtRibbon::RibbonGroup *ribbonGroup = qobject_cast<QtRibbon::RibbonGroup *>(parentWidget);
        if (ribbonGroup) {
            menu = ribbonGroup->addMenu(QIcon(icon), text);
        }
    } else if (parentWidget->inherits("QMenu")) {
        QMenu *parentMenu = qobject_cast<QMenu *>(parentWidget);
        if (parentMenu) {
            menu = parentMenu->addMenu(QIcon(icon), text);
        }
    }

    //
    if (!menu) {
        return 0;   // create failure
    }

    //
    menu->setObjectName(objectName);
    if(!objectName.isEmpty()) {
        q_hashObject.insert(objectName, menu);
    }

    //
    if (!createMenuItem(emItem, menu)) {
        // create failure
    }

    return menu;
}

bool JMainWindow::createMenuItem(const QDomElement &emItem, QMenu *menu)
{
    //
    if (emItem.isNull() || !menu) {
        return false;
    }

    //
    for (QDomElement emSubItem = emItem.firstChildElement();
         !emSubItem.isNull();
         emSubItem = emSubItem.nextSiblingElement()) {
        //
        const QString nodeName = emSubItem.nodeName();
        if(nodeName == "action") {
            //
            QAction *action = createItemAction(emItem);
            if (action) {
                menu->addAction(action);
            }
        } else if(nodeName == "subMenu") {
            //
            QMenu *subMenu = createSubMenu(emItem, menu);
            if (!subMenu) {
                // create failure
            }
        } else if(nodeName == "separator") {
            //
            menu->addSeparator();
        }
    }

    return true;
}

QString JMainWindow::parsePath(const QString &src) const
{
    return QString(src)
            .replace("@AppDir", QString::fromStdString(jframeFacade()->appDirPath()))
            .replace("@ConfigDir", QString::fromStdString(jframeFacade()->configDirPath()))
            .replace("@ThisDir", QString::fromStdString(jframeFacade()->thisDirPath()))
            .replace("@FrameDit", QString::fromStdString(jframeFacade()->frameDirPath()));
}
