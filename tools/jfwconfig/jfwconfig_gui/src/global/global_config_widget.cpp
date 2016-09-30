#include "precomp.h"
#include "global_config_widget.h"

GlobalConfigWidget::GlobalConfigWidget(QWidget *parent)
    : QWidget(parent)
{

}

bool GlobalConfigWidget::appDirChanged(const QString &path)
{
    return true;
}
