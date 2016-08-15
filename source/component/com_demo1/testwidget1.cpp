#include "precomp.h"
#include "testwidget1.h"

TestWidget1::TestWidget1(INotifier *notifier, QWidget *parent)
    : QWidget(parent)
    , q_notifier(notifier)
{
    QHBoxLayout *horiLayoutMain = new QHBoxLayout(this);

    QPushButton *button1 = new QPushButton(QStringLiteral("²âÊÔ×é¼þ1"), this);
    horiLayoutMain->addWidget(button1);

    //
    connect(button1, SIGNAL(clicked(bool)), SLOT(onButton1Clicked()));
}

void TestWidget1::onButton1Clicked()
{
    const std::string currentModule = jframeLayout()->currentModule();
    if (currentModule == "module #1") {
        q_notifier->postMessage("j_switch_module", "module #2 >> module #3 >> module #4");
    } else {
        q_notifier->postMessage("j_switch_module", "module #1");
    }
}
