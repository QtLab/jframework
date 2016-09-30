#include "precomp.h"
#include "testwidget1.h"

TestWidget1::TestWidget1(IJAttempter &attempter, QWidget *parent)
    : QWidget(parent)
    , q_attempter(attempter)
{
    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);

    QPushButton *buttomModeTest = new QPushButton(QStringLiteral("模式切换测试"), this);
    buttomModeTest->setMaximumWidth(200);
    vertLayoutMain->addWidget(buttomModeTest);
    QPushButton *buttomCmdTest = new QPushButton(QStringLiteral("组件命令测试"), this);
    buttomCmdTest->setMaximumWidth(200);
    vertLayoutMain->addWidget(buttomCmdTest);
    QPushButton *buttonMsgTest = new QPushButton(QStringLiteral("组件消息测试"), this);
    buttonMsgTest->setMaximumWidth(200);
    vertLayoutMain->addWidget(buttonMsgTest);

    //
    connect(buttomModeTest, SIGNAL(clicked(bool)), SLOT(onButtonModeTestClicked()));
    connect(buttomCmdTest, SIGNAL(clicked(bool)), SLOT(onButtonCmdTestClicked()));
    connect(buttonMsgTest, SIGNAL(clicked(bool)), SLOT(onButtonMsgTestClicked()));
}

TestWidget1::~TestWidget1()
{

}

void TestWidget1::onButtonModeTestClicked()
{
#if 1
    const std::string currentModule = jframeLayout()->currentModule();
    if (currentModule == "module #1") {
        q_attempter.notifier().postMessage("j_switch_module", "module #2 >> module #3 >> module #4");
    } else {
        q_attempter.notifier().postMessage("j_switch_module", "module #1");
    }
#else
    q_attempter.notifier().postMessage("helloworld");
#endif
}

void TestWidget1::onButtonCmdTestClicked()
{

}

void TestWidget1::onButtonMsgTestClicked()
{
    //
    IJComponent *component2 = q_attempter.queryComponent("com_demo2");
    if (!component2) {
        return;
    }

    //
    const std::string &text = "hello, world!";
    q_attempter.sendMessage(component2, "show_text", (JWPARAM)&text);
}
