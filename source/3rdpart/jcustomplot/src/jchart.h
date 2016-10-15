#ifndef JCHART_H
#define JCHART_H

#include "jcustomplot_global.h"
#include <QWidget>

class JChartPrivate;

/**
 * @brief The JChart class
 */
class JCUSTOMPLOT_EXPORT JChart : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief JChart
     * @param parent
     */
    explicit JChart(QWidget *parent = 0);
    ~JChart();

signals:

public slots:

private:
    Q_DISABLE_COPY(JChart)
    J_DECLARE_PRIVATE(JChart)
};

#endif // JCHART_H
