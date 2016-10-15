#ifndef JABSTRACTCHART_H
#define JABSTRACTCHART_H

#include "jcustomplot_global.h"
#include <QWidget>

class JAbstractChartPrivate;

/**
 * @brief The JAbstractChart class
 */
class JAbstractChart : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief JAbstractChart
     * @param parent
     */
    explicit JAbstractChart(QWidget *parent = 0);
    ~JAbstractChart();

signals:

public slots:

private:
    Q_DISABLE_COPY(JAbstractChart)
    J_DECLARE_PRIVATE(JAbstractChart)
};

#endif // JABSTRACTCHART_H
