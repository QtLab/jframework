﻿#include "precomp.h"
#include "com_demo2.h"
#include "testwidget1.h"

//
J_EXTERN_C J_ATTR_EXPORT void *CreateComponent(void* attemper)
{
    // 非管理员用户才能使用此组件
    if (jframeLogin()->loginManager()->isAdminUser()) {
        return 0;
    }

    return static_cast<IJComponent *>
            (new ComDemo2(reinterpret_cast<IJAttempter *>(attemper)));
}

ComDemo2::ComDemo2(IJAttempter *attemper)
    : q_attempter(attemper)
    , q_testWidget1(0)
{
    q_notifier = jframeLayout()->notifier();
}

ComDemo2::~ComDemo2()
{
    if (q_testWidget1) {
        q_testWidget1->deleteLater();
        q_testWidget1 = 0;
    }
}

bool ComDemo2::loadInterface()
{
    //TODO
    ::Sleep(3000);
    return true;
}

bool ComDemo2::updateInterface()
{
    bool result = true;

    //

    return result;
}

void ComDemo2::releaseInterface()
{

}

void* ComDemo2::queryInterface(const std::string &iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJComponentUi, iid, ver);

    return 0;
}

std::string ComDemo2::componentName() const
{
    return "com_demo2";
}

std::string ComDemo2::componentDesc() const
{
    return QStringLiteral("组件示例 #2").toStdString();
}

void ComDemo2::attach()
{
    // 订阅消息
    q_notifier->beginGroup(this)
            .endGroup();
}

void ComDemo2::detach()
{
    // 取消订阅消息
    q_notifier->remove(this);
}

void *ComDemo2::createWindow(void *parent, const std::string &objectName)
{
    Q_UNUSED(parent);
    Q_UNUSED(objectName);

    //
    if (q_testWidget1) {
        Q_ASSERT(false);
        return 0;
    }

    //
    q_testWidget1 = new TestWidget1(q_notifier);

    return qobject_cast<QWidget *>(q_testWidget1);
}

std::string ComDemo2::observerId() const
{
    return componentName();
}
