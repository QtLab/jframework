#include "precomp.h"
#include "logging_conf_widget.h"

LoggingConfWidget::LoggingConfWidget(QWidget *parent)
    : QWidget(parent)
{

}

bool LoggingConfWidget::appDirChanged(const QString &path)
{
    //
    q_appDir = path;

    return true;
}
