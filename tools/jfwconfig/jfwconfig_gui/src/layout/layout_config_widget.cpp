#include "precomp.h"
#include "layout_config_widget.h"

LayoutConfigWidget::LayoutConfigWidget(QWidget *parent)
    : QWidget(parent)
{

}

bool LayoutConfigWidget::appDirChanged(const QString &path)
{
    return true;
}
