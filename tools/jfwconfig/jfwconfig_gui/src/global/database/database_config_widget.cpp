#include "precomp.h"
#include "database_config_widget.h"

DatabaseConfigWidget::DatabaseConfigWidget(QWidget *parent)
    : QWidget(parent)
{

}

bool DatabaseConfigWidget::init(const QString &path)
{
    //
    q_appDir = path;

    return true;
}
