#ifndef MAINVIEW_MANAGER_H
#define MAINVIEW_MANAGER_H

#include <QWidget>

// class MainViewManager

class QVBoxLayout;
class JSplitter;

class MainViewManager : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QRect clientGeomtry READ clientGeomtry)
public:
    explicit MainViewManager(QWidget *parent = 0);

    bool init();

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
