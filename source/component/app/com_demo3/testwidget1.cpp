#include "precomp.h"
#include "testwidget1.h"

TestWidget1::TestWidget1(IJAttempter &attempter, QWidget *parent)
    : QWidget(parent)
    , q_attempter(attempter)
{
    QHBoxLayout *horiLayoutMain = new QHBoxLayout(this);

    QPushButton *button1 = new QPushButton(QStringLiteral("测试组件3"), this);
    horiLayoutMain->addWidget(button1);

    //
    connect(button1, SIGNAL(clicked(bool)), SLOT(onButton1Clicked()));
}

TestWidget1::~TestWidget1()
{

}

void TestWidget1::onButton1Clicked()
{
    //const std::string currentModule = jframeLayout()->currentModule();
    q_attempter.notifier().postMessage("j_switch_module", "module #2 >> module #3");
}
