#include "precomp.h"
#include "customplottest.h"
#include "customplottestui.h"

//
J_EXTERN_C J_ATTR_EXPORT void *CreateComponent(void* attemper)
{
    return static_cast<IJComponent *>
            (new CustomPlotTest(reinterpret_cast<IJAttempter *>(attemper)));
}

CustomPlotTest::CustomPlotTest(IJAttempter *attemper)
    : q_attempter(attemper)
    , q_ui(0)
{

}

CustomPlotTest::~CustomPlotTest()
{
    if (q_ui) {
        q_ui->deleteLater();
        q_ui = 0;
    }
}

bool CustomPlotTest::loadInterface()
{
    bool result = true;

    //TODO: Add your loading statements.

    return result;
}

bool CustomPlotTest::updateInterface()
{
    bool result = true;

    //TODO: Add your updating statements.

    return result;
}

void CustomPlotTest::releaseInterface()
{
    //TODO: Add your release statements.
}

void* CustomPlotTest::queryInterface(const std::string &iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJComponentUi, iid, ver);
    Q_UNUSED(iid);
    Q_UNUSED(ver);

    return 0;
}

std::string CustomPlotTest::componentName() const
{
    return "customplot_test";
}

std::string CustomPlotTest::componentDesc() const
{
    return "customplot_test";
}

void CustomPlotTest::attach()
{
    // 订阅消息
    q_attempter->notifier().beginGroup(this)
            .endGroup();

    //TODO: Add your statements.
}

void CustomPlotTest::detach()
{
    //TODO: Add your statements.

    // 取消订阅消息
    q_attempter->notifier().remove(this);
}

void *CustomPlotTest::createWindow(void *parent, const std::string &objectName)
{
    Q_UNUSED(parent);
    Q_UNUSED(objectName);

    //
    if (q_ui) {
        Q_ASSERT(false);
        return 0;
    }

    //
    q_ui = new CustomPlotTestUi(*q_attempter);

    return qobject_cast<QWidget *>(q_ui);
}

std::string CustomPlotTest::observerId() const
{
    return componentName();
}
