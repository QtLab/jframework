[!if INCLUDE_PRECOMPILED]
#include "stdafx.h"
[!endif]
#include "[!output UI_HEADER_FILE_NAME]"

[!if INCLUDE_QSHAREDDATA]
class [!output COMPONENT_UI_CLASS_NAME]Data : public QSharedData
{
    friend class [!output COMPONENT_UI_CLASS_NAME];
public:
    explicit [!output COMPONENT_UI_CLASS_NAME]Data(IJAttempter &attempter)
        : attempter(attempter)
    {

    }

private:
    IJAttempter &attempter;
};

[!endif]
[!if UI_BASE_CLASS_IS_QOBJECT]
[!output COMPONENT_UI_CLASS_NAME]::[!output COMPONENT_UI_CLASS_NAME](IJAttempter &attempter, QObject *parent)
    : QObject(parent)
[!if INCLUDE_QSHAREDDATA]
    , data(new [!output COMPONENT_UI_CLASS_NAME]Data(attempter)
[!else]
    , d_attempter(attempter)
[!endif]
[!else]
[!if UI_BASE_CLASS_INERITS_QWIDGET]
[!output COMPONENT_UI_CLASS_NAME]::[!output COMPONENT_UI_CLASS_NAME](IJAttempter &attempter, QWidget *parent)
    : [!output UI_BASE_CLASS_NAME](parent)
[!if INCLUDE_QSHAREDDATA]
    , data(new [!output COMPONENT_UI_CLASS_NAME]Data(attempter)
[!else]
    , d_attempter(attempter)
[!endif]
[!else]
[!output COMPONENT_UI_CLASS_NAME]::[!output COMPONENT_UI_CLASS_NAME](IJAttempter &attempter)
[!if INCLUDE_QSHAREDDATA]
    : data(new [!output COMPONENT_UI_CLASS_NAME]Data(attempter)
[!else]
    : d_attempter(attempter)
[!endif]
[!endif]
[!endif]
{

}

[!output COMPONENT_UI_CLASS_NAME]::~[!output COMPONENT_UI_CLASS_NAME]()
{

}

[!if INCLUDE_QSHAREDDATA]

[!output COMPONENT_UI_CLASS_NAME]::[!output COMPONENT_UI_CLASS_NAME](const [!output COMPONENT_UI_CLASS_NAME] &rhs) : data(rhs.data)
{

}

[!output COMPONENT_UI_CLASS_NAME] &[!output COMPONENT_UI_CLASS_NAME]::operator=(const [!output COMPONENT_UI_CLASS_NAME] &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }

    return *this;
}

[!output COMPONENT_UI_CLASS_NAME]::~[!output COMPONENT_UI_CLASS_NAME]()
{

}
[!endif]
