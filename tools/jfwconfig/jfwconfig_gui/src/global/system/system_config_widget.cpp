#include "precomp.h"
#include "system_config_widget.h"

SystemConfigWidget::SystemConfigWidget(QWidget *parent)
    : QWidget(parent)
{

}

bool SystemConfigWidget::init(const QString &path)
{
    //
    q_appDir = path;

    return true;
}
