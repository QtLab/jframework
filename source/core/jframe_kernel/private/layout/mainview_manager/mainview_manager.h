#ifndef MAINVIEW_MANAGER_H
#define MAINVIEW_MANAGER_H

#include <QWidget>
#include "../../../jframe_core.h"

// class MainViewManager

class QVBoxLayout;
class JSplitter;

class MainViewManager :
        public QWidget,
        public IJUnknown
{
    Q_OBJECT
    Q_PROPERTY(QRect clientGeometry READ clientGeometry)
public:
    explicit MainViewManager(QWidget *parent = 0);

    // IJUnknown interface
public:
    bool loadInterface();
    void releaseInterface();

public:
    void attachSplitter(QWidget *widget);
    void detachSplitter(QWidget *widget);

    QRect clientGeometry() const;

    Q_INVOKABLE void setCtrlPane(QWidget *widget);
    Q_INVOKABLE void removeCtrlPane();

private:
    QVBoxLayout *q_layoutClient;
    QVBoxLayout *q_layout;
    JSplitter *q_splitterMain;
    QWidget *q_clientWidget;
    QWidget *q_ctrlPane;
};

#endif // MAINVIEW_MANAGER_H
