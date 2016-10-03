#include "precomp.h"
#include "generate_config_widget.h"

GenerateConfigWidget::GenerateConfigWidget(QWidget *parent)
    : QWidget(parent)
{

}

bool GenerateConfigWidget::init(const QString &path)
{
    //
    q_appDir = path;

    return true;
}
