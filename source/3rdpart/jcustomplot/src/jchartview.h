#ifndef JCHARTVIEW_H
#define JCHARTVIEW_H

#include "jcustomplot_global.h"
#include <QWidget>

class JChart;
class JChartViewPrivate;

/**
 * @brief The JChartView class
 */
class JCUSTOMPLOT_EXPORT JChartView : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief JChartView
     * @param parent
     */
    explicit JChartView(QWidget *parent = 0);

    ~JChartView();

    /**
     * @brief appendChart
     * @param chart
     */
    void appendChart(JChart *chart);

    /**
     * @brief insertChart
     * @param index
     * @param chart
     */
    void insertChart(int index, JChart *chart);

    /**
     * @brief takeChart
     * @param index
     * @return
     */
    JChart *takeChart(int index);

    /**
     * @brief removeChart
     * @param chart
     */
    void removeChart(JChart *chart);

    /**
     * @brief clearChart
     * @param destroy
     */
    void clearChart(bool destroy = true);

    /**
     * @brief chartAt
     * @param index
     * @return
     */
    JChart *chartAt(int index) const;

    /**
     * @brief indexOfChart
     * @param chart
     * @return
     */
    int indexOfChart(JChart *chart) const;

    /**
     * @brief charts
     * @return
     */
    QList<JChart *> charts() const;

signals:

private:
    void updateChartLayout();

private:
    Q_DISABLE_COPY(JChartView)
    J_DECLARE_PRIVATE(JChartView)
};

#endif // JCHARTVIEW_H
