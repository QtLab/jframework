#include "precomp.h"
#include "qtconf_config_widget.h"

QtConfConfigWidget::QtConfConfigWidget(QWidget *parent)
    : QWidget(parent)
{

}

bool QtConfConfigWidget::init(const QString &path)
{
    //
    q_appDir = path;

    return true;
}
