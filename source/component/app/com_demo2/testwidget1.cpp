#include "precomp.h"
#include "testwidget1.h"

TestWidget1::TestWidget1(IJAttempter &attempter, QWidget *parent)
    : QWidget(parent)
    , q_attempter(attempter)
{
    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);

    QPushButton *button1 = new QPushButton(QStringLiteral("测试组件2"), this);
    button1->setFixedWidth(200);
    vertLayoutMain->addWidget(button1, 0, Qt::AlignHCenter);

    QPushButton *button2 = new QPushButton(QStringLiteral("D-BUS命令测试"), this);
    button2->setFixedWidth(200);
    vertLayoutMain->addWidget(button2, 0, Qt::AlignHCenter);

    //
    connect(button1, SIGNAL(clicked(bool)), SLOT(onButton1Clicked()));
    connect(button2, SIGNAL(clicked(bool)), SLOT(onButton2Clicked()));
}

TestWidget1::~TestWidget1()
{

}

void TestWidget1::onButton1Clicked()
{
    jframeFacade()->tryExitFrame();
}

void TestWidget1::onButton2Clicked()
{
#if 1
    int result = q_attempter.notifier().dbus().sendString(
                "helloworld @ com_demo1 @ *",
                "hello,world");
    qDebug() << "result: " << result;
#else
    q_attempter.notifier().dbus().postString(
                    "helloworld @ com_demo1 @ com.smartsoft.jframe1",
                    "hello,world");
#endif
}
