#ifndef LAYOUT_MANAGER_H
#define LAYOUT_MANAGER_H

#include "factory/jnotifier.h"
#include "jsplitter.h"

// struct JLayoutConfig

struct JLayoutConfig
{
    // -- splitter --
    struct {
        int width;
        bool checkable;
        bool opaqueResize;
        QColor color;
        bool syncScales;    // 同步scales属性
    } splitter;
    // -- switch --
    struct {
        bool remember;      // 是否记忆模式切换结果
    } sw;
};

// class LayoutManager

class JFrameLayout;
struct JComponentInfo;
class QDomElement;

class LayoutManager : public IJUnknown
{
public:
    explicit LayoutManager(JFrameLayout *frameLayout);
    ~LayoutManager();

    // IJUnknown interface
public:
    bool loadInterface();
    void releaseInterface();

public:
    JLayoutConfig layoutConfig() const;

    //
    bool updateLayout(const QString &section = QString::null);

private:
    bool loadLayoutConfig();

    // 复位布局配置文件信息（软件启动，进行初始化时调用）
    bool resetModuleElement();
    void resetModuleElement(QDomElement emModule);

    // 获取配置文件中指定系统的当前模式
    QString currentConfigModule(const QDomElement &emSystem);

    // 解析系统状态
    bool parseSystemStatus(QDomElement &emRoot, const QString &section,
                           QDomElement &emSystem, QDomElement &emModule);

    // 递归创建视图窗口布局
    JSplitter *createView(const QDomElement &emSplitter, QWidget *parent = 0);

    // 加载模式节点视图类型组件
    bool loadModuleNode(const QDomElement &emModule);

    // 加载模块subview节点
    bool loadModuleViewNode(const QDomElement &emView);

    // 加载模式节点popup类型组件
    bool loadModulePopupNode(const QDomElement &emPopup);

    // 加载模块centerview节点
    bool loadModuleCenterviewNode(const QDomElement &emCenterView);

    // 加载模块statusbar节点
    bool loadModuleStatusBarNode(const QDomElement &emStatusBar);

    // 加载模块dock节点
    bool loadModuleDockNode(const QDomElement &emDock);

    // 查找系统节点
    QDomElement findSystemNode(QDomElement &emParent, const QString &system);

    // 查找模式节点
    QDomElement findModuleNode(QDomElement &emParent, QString &section);

    // 查找模式节点（不修改配置信息）
    QDomElement findModuleNode(const QDomElement &emParent, const QString &section);

    // 检测组件是否可以挂载（相应用户系统、模式下）
    bool isComponentAttachEnabled(const JComponentInfo *componentInfo) const;

    // 激活组件
    JComponentInfo *activateComponent(const QDomElement &emComponent, bool show);

    // 创建分割器
    JSplitter *createSplitter(const QDomElement &emSplitter, QWidget *parent = 0);

    // 保存splitter的scales值到布局管理配置文件
    bool saveCurrentSplitterScales();

    // 递归更新splitter的scales值
    bool updateSpltterScales(const QDomElement &emParent, const QWidget *widget = 0);

private:
    JFrameLayout *q_frameLayout;
    INotifier *q_notifier;
    QPointer<JSplitter> q_mainSplitter;
    JLayoutConfig q_layoutConfig;

    friend class JSplitterSync;
};

// class JSplitterSync

class JSplitterSync : public JSplitter
{
    Q_OBJECT
public:
    explicit JSplitterSync(LayoutManager *layoutManager,
                           QWidget *parent = 0);
    explicit JSplitterSync(LayoutManager *layoutManager,
                           Qt::Orientation orientation,
                           QWidget *parent = 0);

public Q_SLOTS:
    void onSplitterMoved(int pos, int index);

private:
    LayoutManager *q_layoutManager;
};

#endif // LAYOUT_MANAGER_H
