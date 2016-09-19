#ifndef [!output COMPONENT_UI_HEADER_GUARD]
#define [!output COMPONENT_UI_HEADER_GUARD]

[!if INCLUDE_QWIDGET]
#include <QWidget>
[!endif]
[!if INCLUDE_QDIALOG]
#include <QDialog>
[!endif]
[!if INCLUDE_QMAINWINDOW]
#include <QMainWindow>
[!endif]
[!if INCLUDE_QSHAREDDATA]
#include <QSharedDataPointer>
[!endif]

[!if INCLUDE_QSHAREDDATA]
class [!output COMPONENT_UI_CLASS_NAME]Data;
[!endif]
class IJAttempter;

[!if UI_BASE_CLASS_NAME_IS_EMPTY]
class [!output COMPONENT_UI_CLASS_NAME]
[!else]
class [!output COMPONENT_UI_CLASS_NAME] : public [!output UI_BASE_CLASS_NAME]
[!endif]
{
[!if BASECLASS_INSTANCEOF_QOBJECT]
    Q_OBJECT
[!endif]
public:
[!if UI_BASE_CLASS_IS_QOBJECT]
    explicit [!output COMPONENT_UI_CLASS_NAME](IJAttempter &attempter, QObject *parent = 0);
[!else]
[!if UI_BASE_CLASS_INERITS_QWIDGET]
    explicit [!output COMPONENT_UI_CLASS_NAME](IJAttempter &attempter, QWidget *parent = 0);
[!else]
    [!output COMPONENT_UI_CLASS_NAME](IJAttempter &attempter);
[!endif]
[!endif]
[!if INCLUDE_QSHAREDDATA]
    [!output COMPONENT_UI_CLASS_NAME](const [!output COMPONENT_UI_CLASS_NAME] &);
    [!output COMPONENT_UI_CLASS_NAME] &operator=(const [!output COMPONENT_UI_CLASS_NAME] &);
[!endif]
    ~[!output COMPONENT_UI_CLASS_NAME]();

[!if BASECLASS_INSTANCEOF_QOBJECT]
signals:

public slots:

[!endif]
private:
[!if INCLUDE_QSHAREDDATA]
    QSharedDataPointer<[!output COMPONENT_UI_CLASS_NAME]Data> data;
[!else]
    IJAttempter &q_attempter;
[!endif]
};

#endif // [!output COMPONENT_UI_HEADER_GUARD]
