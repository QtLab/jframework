#include "precomp.h"
#include "logging_config_widget.h"

LoggingConfigWidget::LoggingConfigWidget(QWidget *parent)
    : QWidget(parent)
{

}

bool LoggingConfigWidget::appDirChanged(const QString &path)
{
    return true;
}
