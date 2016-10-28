﻿#include "stdafx.h"
#include "com_demo4.h"

//
J_EXTERN_C J_ATTR_EXPORT void *CreateComponent(void* attemper)
{
	// 非管理员用户才能使用此组件
	if (jframeLogin()->loginManager()->isAdminUser()) {
		return 0;
	}

	return static_cast<IJComponent *>
		(new ComDemo4(reinterpret_cast<IJAttempter *>(attemper)));
}

ComDemo4::ComDemo4(IJAttempter *attemper)
	: q_attempter(attemper)
	, q_widget(0)
{

}

ComDemo4::~ComDemo4()
{
	if (q_widget) {
		q_widget->deleteLater();
		q_widget = 0;
	}
}

bool ComDemo4::loadInterface()
{
	return true;
}

bool ComDemo4::updateInterface()
{
	bool result = true;

	//

	return result;
}

void ComDemo4::releaseInterface()
{

}

void* ComDemo4::queryInterface(const std::string &iid, unsigned int ver)
{
	J_QUERY_INTERFACE(IJComponentUi, iid, ver);

	return 0;
}

std::string ComDemo4::componentName() const
{
	return "com_demo4";
}

std::string ComDemo4::componentDesc() const
{
	return QStringLiteral("组件示例 #4").toStdString();
}

std::string ComDemo4::componentType() const
{
	return "mfc";
}

void ComDemo4::attach()
{
	// 订阅消息
	q_attempter->notifier().beginGroup(this)
		.endGroup();
}

void ComDemo4::detach()
{
	// 取消订阅消息
	q_attempter->notifier().remove(this);
}

void *ComDemo4::createWindow(void *parent, const std::string &objectName)
{
	Q_UNUSED(parent);
	Q_UNUSED(objectName);

	//
	if (q_widget) {
		Q_ASSERT(false);
		return 0;
	}

	//
	q_widget = new QPushButton(QStringLiteral("测试组件4"));

	return qobject_cast<QWidget *>(q_widget);
}

std::string ComDemo4::observerId() const
{
	return componentName();
}
