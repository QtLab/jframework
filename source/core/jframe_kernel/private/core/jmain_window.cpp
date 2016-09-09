#include "precomp.h"
#include "jmain_window.h"
#include "jframewnd.h"
#include <QtXml>
#include "../layout/jframe_layout_p.h"
#include <QHash>
#include <QPair>

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
        return false;       // invalid
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
        return false;       // open failure
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
        return false;       // parse failure
    }

    // 关闭文件
    file.close();

    // 获取根节点
    QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return false;       // invalid
    }

    // 获取 ribbon节点
    QDomElement emRibbon = emRoot.firstChildElement("ribbon");
    if (!emRibbon.isNull()) {
        if (!createRibbonPage(emRibbon, QString::fromStdString(component->componentName()))) {
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

void *JMainWindow::queryObject(const std::string &objectName, const std::string &componentName)
{
    if (componentName.empty()) {
        QHashIterator<QString, QHash<QString, QObject *> > citerHash(q_hashObject);
        while (citerHash.hasNext()) {
            citerHash.next();
            // find object with objectName
            QHash<QString, QObject *>::const_iterator citerObject =
                    citerHash.value().find(QString::fromStdString(objectName));
            if (citerObject != citerHash.value().end()) {
                return citerObject.value();   // found out
            }
        }
    } else {
        // find hash<object> with componentName
        QHash<QString, QHash<QString, QObject *> >::const_iterator citerHash =
                q_hashObject.find(QString::fromStdString(componentName));
        if (citerHash == q_hashObject.end()) {
            return 0;   // not found
        }

        // find object with objectName
        QHash<QString, QObject *>::const_iterator citerObject =
                citerHash.value().find(QString::fromStdString(objectName));
        if (citerObject == citerHash.value().end()) {
            return 0;   // not found
        }

        //
        return citerObject.value(); // found out
    }

    return 0;   // not found
}

void *JMainWindow::menuBar()
{
    if (q_frameWnd) {
        return (void *)q_frameWnd->menuBar();
    } else {
        return 0;
    }
}

void *JMainWindow::ribbonBar()
{
    if (q_frameWnd) {
        return (void *)q_frameWnd->ribbonBar();
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
                if (!widget) {
                    Q_ASSERT(false);
                    // convert failure
                }
                // set domain of object
                widget->setProperty("domain", QString::fromStdString(component->componentName()));
                //
                if (!insertHashObject(QString::fromStdString(component->componentName()), objectName,
                                      qobject_cast<QObject *>(widget))) {
                    // insert failure
                }
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

bool JMainWindow::createRibbonPage(const QDomElement &emRibbon, const QString &componentName)
{
    //
    if (emRibbon.isNull() || componentName.isEmpty()) {
        return false;   //
    }

    //
    for (QDomElement emPage = emRibbon.firstChildElement("page");
         !emPage.isNull();
         emPage = emPage.nextSiblingElement("page")) {
        //
        const QString objectName = emPage.attribute("objectName");
        const QString title = emPage.attribute("title");
        //
        QtRibbon::RibbonPage *ribbonPage = 0;
        QMutableHashIterator<QString, QHash<QString, QObject *> > iterHash(q_hashObject);
        while (iterHash.hasNext()) {
            iterHash.next();
            QHash<QString, QObject *>::iterator iterObject
                    = iterHash.value().find(objectName);
            if (iterObject != iterHash.value().end()) {
                ribbonPage = qobject_cast<QtRibbon::RibbonPage *>(iterObject.value());
                if(ribbonPage) {
                    break;  // found out
                } else if (iterHash.key() == componentName) {
                    // create
                    ribbonPage = q_frameWnd->ribbonBar()->addPage(title);
                    if (ribbonPage) {
                        ribbonPage->setObjectName(objectName);
                        // set domain of object
                        ribbonPage->setProperty("domain", componentName);
                        //
                        qWarning() << QStringLiteral("objectName[%1] has been exists in component[%2]!"
                                                     " destory and replace it!")
                                      .arg(componentName).arg(objectName);
                        // destory old object
                        iterObject.value()->deleteLater();
                        // replace
                        *iterObject = ribbonPage;
                    } else {
                        ribbonPage = 0;
                    }
                } else {
                    ribbonPage = 0;
                }
            }
        }

        //
        if (!ribbonPage) {
            // create
            ribbonPage = q_frameWnd->ribbonBar()->addPage(title);
            if (ribbonPage) {
                ribbonPage->setObjectName(objectName);
                // set domain of object
                ribbonPage->setProperty("domain", componentName);
                //
                QHash<QString, QObject *> hash;
                hash.insert(objectName, ribbonPage);
                q_hashObject.insert(componentName, hash);
            } else {
                return 0;       // create failure
            }
        }

        //
        ribbonPage->setVisible(QVariant(emPage.attribute("visible", "true")).toBool());
        ribbonPage->setToolTip(emPage.attribute("tooltip"));
        ribbonPage->setContextPage(parseContextColor(emPage.attribute("contextColor")));
        ribbonPage->setContextTitle(emPage.attribute("contextTitle"));
        ribbonPage->setShortcutEnabled(QVariant(emPage.attribute("shortcutEabled", "true")).toBool());

        //
        if (!createRibbinGroup(emPage, ribbonPage, componentName)) {
            // create failure
        }
    }

    return true;
}

bool JMainWindow::createRibbinGroup(const QDomElement &emPage, QtRibbon::RibbonPage *ribbonPage, const QString &componentName)
{
    //
    if (emPage.isNull() || !ribbonPage || componentName.isEmpty()) {
        return false;
    }

    //
    for (QDomElement emGroup = emPage.firstChildElement("group");
         !emGroup.isNull();
         emGroup = emGroup.nextSiblingElement("group")) {
        //
        const QString objectName = emGroup.attribute("objectName");
        const QString text = emGroup.attribute("title");
        //
        QtRibbon::RibbonGroup *ribbonGroup = 0;
        QMutableHashIterator<QString, QHash<QString, QObject *> > iterHash(q_hashObject);
        while (iterHash.hasNext()) {
            iterHash.next();
            QHash<QString, QObject *>::iterator iterObject
                    = iterHash.value().find(objectName);
            if (iterObject != iterHash.value().end()) {
                ribbonGroup = qobject_cast<QtRibbon::RibbonGroup *>(iterObject.value());
                if(ribbonGroup) {
                    break;  // found out
                } else if (iterHash.key() == componentName) {
                    // create
                    ribbonGroup = ribbonPage->addGroup(text);
                    if (ribbonGroup) {
                        ribbonGroup->setObjectName(objectName);
                        // set domain of object
                        ribbonGroup->setProperty("domain", componentName);
                        //
                        qWarning() << QStringLiteral("objectName[%1] has been exists in component[%2]!"
                                                     " destory and replace it!")
                                      .arg(componentName).arg(objectName);
                        // destory old object
                        iterObject.value()->deleteLater();
                        // replace
                        *iterObject = ribbonGroup;
                    } else {
                        ribbonGroup = 0;
                    }
                } else {
                    ribbonGroup = 0;
                }
            }
        }

        //
        if (!ribbonGroup) {
            // create
            ribbonGroup = ribbonPage->addGroup(text);
            if (ribbonGroup) {
                ribbonGroup->setObjectName(objectName);
                // set domain of object
                ribbonGroup->setProperty("domain", componentName);
                //
                QHash<QString, QObject *> hash;
                hash.insert(objectName, ribbonGroup);
                q_hashObject.insert(componentName, hash);
            } else {
                return 0;       // create failure
            }
        }

        //
        ribbonGroup->setVisible(QVariant(emGroup.attribute("visible", "true")).toBool());
        //ribbonGroup->setControlsGrouping();

        //
        if (!createRibbinItem(emGroup, ribbonGroup, componentName)) {
            // create failure
        }
    }

    return true;
}

bool JMainWindow::createRibbinItem(const QDomElement &emGroup, QtRibbon::RibbonGroup *ribbonGroup, const QString &componentName)
{
    //
    if (emGroup.isNull() || !ribbonGroup || componentName.isEmpty()) {
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
            QAction *action = createItemAction(emItem, componentName);
            if (action) {
                // set domain of object
                action->setProperty("domain", componentName);
                //
                action->setParent(ribbonGroup);
                ribbonGroup->addAction(action, Qt::ToolButtonTextUnderIcon);
            }
        } else if(nodeName == "subMenu") {
            //
            QMenu *menu = createSubMenu(emItem, ribbonGroup, componentName);
            if (menu) {
                // set domain of object
                menu->setProperty("domain", componentName);
            }
        }
        else if(nodeName == "separator") {
            //
            ribbonGroup->addSeparator();
        } else if(nodeName == "comboBox") {
            QComboBox *comboBox = createItemComboBox(emItem, componentName);
            if (comboBox) {
                // set domain of object
                comboBox->setProperty("domain", componentName);
                //
                comboBox->setParent(ribbonGroup);
                ribbonGroup->addWidget(comboBox);
            }
        } else if(nodeName == "checkBox") {
            //
            QCheckBox *checkBox = createItemCheckBox(emItem, componentName);
            if (checkBox) {
                // set domain of object
                checkBox->setProperty("domain", componentName);
                //
                checkBox->setParent(ribbonGroup);
                ribbonGroup->addWidget(checkBox);
            }
        }
        else if(nodeName == "radioButton") {
            //
            QRadioButton *radioButton = createItemRadioButton(emItem, componentName);
            if (radioButton) {
                // set domain of object
                radioButton->setProperty("domain", componentName);
                //
                radioButton->setParent(ribbonGroup);
                ribbonGroup->addWidget(radioButton);
            }
        } else if(nodeName == "label") {
            QLabel *label = createItemLabel(emItem, componentName);
            if (label) {
                // set domain of object
                label->setProperty("domain", componentName);
                //
                label->setParent(ribbonGroup);
                ribbonGroup->addWidget(label);
            }
        } else if(nodeName == "bitmap") {
            //
            QLabel *label = createItemLabel(emItem, componentName);
            if (label) {
                // set domain of object
                label->setProperty("domain", componentName);
                //
                label->setParent(ribbonGroup);
                label->setPixmap(QPixmap(parsePath(emItem.attribute("icon"))));
                ribbonGroup->addWidget(label);
            }
        }
    }

    return true;
}

QAction* JMainWindow::createItemAction(const QDomElement &emItem, const QString &componentName)
{
    //
    if (emItem.isNull() || componentName.isEmpty()) {
        return 0;
    }

    //
    const QString objectName = emItem.attribute("objectName");

    // insert to hash table
    QAction *action = createHashObject<QAction>
            (componentName, objectName, q_frameWnd,
             SIGNAL(triggered(bool)), SLOT(onActionTriggered(bool)));
    if(!action) {
        return 0;   // insert failure
    }

    //
    action->setObjectName(objectName);
    action->setText(emItem.attribute("text"));
    action->setIcon(QIcon(parsePath(emItem.attribute("icon"))));
    action->setShortcut(emItem.attribute("shortcut"));
    action->setCheckable(QVariant(emItem.attribute("checkable", "true")).toBool());

    //
    QObject::connect(action, SIGNAL(triggered(bool)),
                     q_frameWnd, SLOT(onActionTriggered(bool)));

    return action;
}

QComboBox *JMainWindow::createItemComboBox(const QDomElement &emItem, const QString &componentName)
{
    //
    if (emItem.isNull() || componentName.isEmpty()) {
        return 0;
    }

    //
    const QString objectName = emItem.attribute("objectName");

    // insert to hash table
    QComboBox *comboBox = createHashObject<QComboBox>
            (componentName, objectName, q_frameWnd,
             SIGNAL(currentIndexChanged(int)), SLOT(onComboBoxCurrentIndexChanged(int)));
    if(!comboBox) {
        return 0;   // insert failure
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
    QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)),
                     q_frameWnd, SLOT(onComboBoxCurrentIndexChanged(int)));

    return comboBox;
}

QCheckBox *JMainWindow::createItemCheckBox(const QDomElement &emItem, const QString &componentName)
{
    //
    if (emItem.isNull() || componentName.isEmpty()) {
        return 0;
    }

    //
    const QString objectName = emItem.attribute("objectName");

    // insert to hash table
    QCheckBox *checkBox = createHashObject<QCheckBox>
            (componentName, objectName, q_frameWnd,
             SIGNAL(stateChanged(int)), SLOT(onCheckBoxStateChanged(int)));
    if(!checkBox) {
        return 0;   // insert failure
    }

    //
    checkBox->setObjectName(objectName);
    checkBox->setText(emItem.attribute("text"));
    checkBox->setChecked(QVariant(emItem.attribute("checked", "false")).toBool());

    //
    QObject::connect(checkBox, SIGNAL(stateChanged(int)),
                     q_frameWnd, SLOT(onCheckBoxStateChanged(int)));

    return checkBox;
}

QRadioButton *JMainWindow::createItemRadioButton(const QDomElement &emItem, const QString &componentName)
{
    //
    if (emItem.isNull() || componentName.isEmpty()) {
        return 0;
    }

    //
    const QString objectName = emItem.attribute("objectName");

    // insert to hash table
    QRadioButton *radioButton = createHashObject<QRadioButton>
            (componentName, objectName, q_frameWnd,
             SIGNAL(clicked(bool)), SLOT(onRadioButtonClicked(bool)));
    if(!radioButton) {
        return 0;   // insert failure
    }

    //
    radioButton->setObjectName(objectName);
    radioButton->setText(emItem.attribute("text"));
    radioButton->setChecked(QVariant(emItem.attribute("checked", "false")).toBool());

    //
    QObject::connect(radioButton, SIGNAL(clicked(bool)),
                     q_frameWnd, SLOT(onRadioButtonClicked(bool)));

    return radioButton;
}

QLabel *JMainWindow::createItemLabel(const QDomElement &emItem, const QString &componentName)
{
    //
    if (emItem.isNull() || componentName.isEmpty()) {
        return 0;
    }

    //
    const QString objectName = emItem.attribute("objectName");

    // insert to hash table
    QLabel *label = createHashObject<QLabel>(componentName, objectName);
    if(!label) {
        return 0;   // insert failure
    }

    //
    label->setObjectName(objectName);
    label->setText(emItem.attribute("text"));

    return label;
}

QMenu *JMainWindow::createSubMenu(const QDomElement &emItem, QWidget *parentWidget, const QString &componentName)
{
    //
    if (emItem.isNull() || !parentWidget || componentName.isEmpty()) {
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
    // set domain of object
    menu->setProperty("domain", componentName);

    // insert to hash table
    if(!insertHashObject(componentName, objectName, menu)) {
        // insert failure
    }

    //
    if (!createMenuItem(emItem, menu, componentName)) {
        // create failure
    }

    return menu;
}

bool JMainWindow::createMenuItem(const QDomElement &emItem, QMenu *menu, const QString &componentName)
{
    //
    if (emItem.isNull() || !menu || componentName.isEmpty()) {
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
            QAction *action = createItemAction(emItem, componentName);
            if (action) {
                // set domain of object
                menu->setProperty("domain", componentName);
                //
                menu->addAction(action);
            }
        } else if(nodeName == "subMenu") {
            //
            QMenu *subMenu = createSubMenu(emItem, menu, componentName);
            if (subMenu) {
                // set domain of object
                subMenu->setProperty("domain", componentName);
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
            .replace("@FrameDir", QString::fromStdString(jframeFacade()->frameDirPath()));
}

bool JMainWindow::insertHashObject(const QString &componentName, const QString &objectName, QObject *object)
{
    //
    if (componentName.isEmpty() || objectName.isEmpty() || !object) {
        Q_ASSERT(false);
        return false;   //
    }

    //
    QHash<QString, QHash<QString, QObject *> >::iterator iterHash =
            q_hashObject.find(componentName);
    if (iterHash != q_hashObject.end()) {
        QHash<QString, QObject *>::iterator iterObject =
                iterHash.value().find(objectName);
        if (iterObject != iterHash.value().end()) {
            qWarning() << QStringLiteral("objectName[%1] has been exists in component[%2]!"
                                         " replace it!")
                          .arg(componentName).arg(objectName);
#if 1
            // destroy old object
            iterObject.value()->deleteLater();
#else
            return false;
#endif
        }
        // insert or update
        iterHash.value().insert(objectName, object);
    } else {
        QHash<QString, QObject *> hash;
        hash.insert(objectName, object);
        q_hashObject.insert(componentName, hash);
    }

    return true;
}

bool JMainWindow::checkObjectName(const QDomElement &element)
{
    if (element.isNull() && element.attribute("objectName").isEmpty()) {
        //
        return false;
    }

    return false;
}

QtRibbon::ContextColor JMainWindow::parseContextColor(const QString &text)
{
    if (text == "none") {
        return QtRibbon::ContextColorNone;
    } else if (text == "green") {
        return QtRibbon::ContextColorGreen;
    } else if (text == "blue") {
        return QtRibbon::ContextColorBlue;
    } else if (text == "red") {
        return QtRibbon::ContextColorRed;
    } else if (text == "yellow") {
        return QtRibbon::ContextColorYellow;
    } else if (text == "cyan") {
        return QtRibbon::ContextColorCyan;
    } else if (text == "purple") {
        return QtRibbon::ContextColorPurple;
    } else if (text == "orange") {
        return QtRibbon::ContextColorOrange;
    } else {
        return QtRibbon::ContextColorNone;
    }
}
