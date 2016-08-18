#include "precomp.h"
#include "testwidget1.h"

TestWidget1::TestWidget1(INotifier *notifier, QWidget *parent)
    : QWidget(parent)
    , q_notifier(notifier)
{
    QHBoxLayout *horiLayoutMain = new QHBoxLayout(this);

    QPushButton *button1 = new QPushButton(QStringLiteral("²âÊÔ×é¼þ3"), this);
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
    q_notifier->postMessage("j_switch_module", "module #2");
}
