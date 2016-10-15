#ifndef JABSTRACTSERIES_H
#define JABSTRACTSERIES_H

#include "jcustomplot_global.h"
#include <QWidget>

class JAbstractSeriesPrivate;

class JCUSTOMPLOT_EXPORT JAbstractSeries : public QWidget
{
    Q_OBJECT
public:
    explicit JAbstractSeries(QWidget *parent = 0);
    ~JAbstractSeries();

signals:

public slots:

private:
    Q_DISABLE_COPY(JAbstractSeries)
    J_DECLARE_PRIVATE(JAbstractSeries)
};

#endif // JABSTRACTSERIES_H
