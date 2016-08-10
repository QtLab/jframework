#include "precomp.h"
#include "mainview_manager.h"
#include "../jframe_layout_p.h"

// class MainViewManager


MainViewManager::MainViewManager(QWidget *parent):
    QWidget(parent),
    q_clientWidget(0),
    q_ctrlPane(0)
{
    setObjectName("MainViewManager");

    q_layout = new QVBoxLayout(this);
    q_layout->setContentsMargins(0, 0, 0, 0);
    q_layout->setSpacing(0);

    q_splitterMain = new JSplitter(Qt::Vertical, this);
    q_splitterMain->setScales(QList<double>() << 50 << 1);
    q_splitterMain->setHandleWidth(1);
    // ��ֹ����ָ����϶�
    QSplitterHandle *handle = q_splitterMain->handle(1);
    if (handle) {
        handle->setDisabled(true);
    }
    q_layout->addWidget(q_splitterMain);

    //
    q_clientWidget = new QWidget(this);
    q_clientWidget->setEnabled(true);
    q_splitterMain->addWidget(q_clientWidget);

    //
    q_layoutClient = new QVBoxLayout(q_clientWidget);
    q_layoutClient->setContentsMargins(0, 0, 0, 0);
    q_layoutClient->setSpacing(0);
}

bool MainViewManager::init()
{
    return true;
}

void MainViewManager::attachSplitter(QWidget *widget)
{
    //
    q_layoutClient->addWidget(widget);
}

void MainViewManager::detachSplitter(QWidget *widget)
{
    //
    q_layoutClient->removeWidget(widget);
}

QRect MainViewManager::clientGeometry() const
{
    //
    if (!q_clientWidget) {
        return QRect(); //
    }

    //
    QRect rect = q_clientWidget->rect();
    rect.moveTopLeft(q_clientWidget->mapToGlobal(rect.topLeft()));

    //
    return rect;
}

void MainViewManager::setCtrlPane(QWidget *widget)
{
    if (q_ctrlPane) {
        return; //
    }

    //
    q_splitterMain->addWidget(widget);

    //
    q_ctrlPane = widget;
}

void MainViewManager::removeCtrlPane()
{
    // �������
    if (!q_ctrlPane) {
        return;     //
    }

    // �Ӵ��ڷָ�����ɾ��������崰��
    q_ctrlPane = 0; // ����ɾ������Զ���splitterɾ��
}
