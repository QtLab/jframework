#include "precomp.h"
#include "jchartview.h"
#include "jchart.h"

// class JChartViewPrivate

class JChartViewPrivate
{
public:
    JChartViewPrivate(JChartView *parent)
        : q_ptr(parent)
    {

    }

    void init();

private:
    J_DECLARE_PUBLIC(JChartView)
    QList<JChart *> charts;
    QSpinBox *spinBoxColumnCount;
    QScrollArea *scrollArea;
    QWidget *widgetCharts;
    QGridLayout *gridLayoutChart;
};

void JChartViewPrivate::init()
{
    Q_Q(JChartView);

    QVBoxLayout *vertLayoutMain = new QVBoxLayout(q);
    vertLayoutMain->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *horiLayoutTop = new QHBoxLayout();
    horiLayoutTop->setContentsMargins(0, 0, 0, 0);
    vertLayoutMain->addLayout(horiLayoutTop);

    QFormLayout *formLayoutColumnCount = new QFormLayout();
    horiLayoutTop->addLayout(formLayoutColumnCount);
    horiLayoutTop->addStretch();

    spinBoxColumnCount = new QSpinBox(q);
    spinBoxColumnCount->setMinimumWidth(120);
    spinBoxColumnCount->setRange(1, 20);
    formLayoutColumnCount->addRow(QStringLiteral("列数："), spinBoxColumnCount);

    scrollArea = new QScrollArea(q);
    vertLayoutMain->addWidget(scrollArea);

    widgetCharts = new QWidget(q);
    scrollArea->setWidget(widgetCharts);

    gridLayoutChart = new QGridLayout(widgetCharts);
    gridLayoutChart->setContentsMargins(2, 2, 2, 2);
    gridLayoutChart->setSpacing(5);

    //
    QObject::connect(spinBoxColumnCount, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            [q](){
        q->updateChartLayout();
    });
}

// class JChartView

JChartView::JChartView(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new JChartViewPrivate(this))
{
    Q_D(JChartView);
    d->init();
}

JChartView::~JChartView()
{
    Q_D(JChartView);
    delete d;
}

void JChartView::appendChart(JChart *chart)
{
    //
    if (!chart) {
        return;     //
    }
    //
    Q_D(JChartView);
    if (d->charts.contains(chart)) {
        return;     //
    }
    //
    d->charts.append(chart);
    //
    updateChartLayout();
}

void JChartView::insertChart(int index, JChart *chart)
{
    //
    if (!chart) {
        return;     //
    }
    //
    Q_D(JChartView);
    if (d->charts.contains(chart)) {
        return;     //
    }
    //
    d->charts.insert(index, chart);
}

JChart *JChartView::takeChart(int index)
{
    Q_D(JChartView);
    //
    if (index < 0 || index >= d->charts.count()) {
        return 0;
    }
    //
    return d->charts.takeAt(index);
}

void JChartView::removeChart(JChart *chart)
{
    //
    if (!chart) {
        return;     //
    }
    //
    Q_D(JChartView);
    if (!d->charts.contains(chart)) {
        return;     //
    }
    //
    d->charts.removeAll(chart);
    //
    chart->deleteLater();
}

void JChartView::clearChart(bool destroy)
{
    Q_D(JChartView);
    if (destroy) {
        QListIterator<JChart *> citer(d->charts);
        while (citer.hasNext()) {
            citer.next()->deleteLater();
        }
    }
    d->charts.clear();
}

JChart *JChartView::chartAt(int index) const
{
    Q_D(const JChartView);
    if (index < 0 || index >= d->charts.count()) {
        return 0;
    }
    return d->charts.at(index);
}

int JChartView::indexOfChart(JChart *chart) const
{
    if (!chart) {
        return -1;
    }
    Q_D(const JChartView);
    return d->charts.indexOf(chart);
}

QList<JChart *> JChartView::charts() const
{
    Q_D(const JChartView);
    return d->charts;
}

void JChartView::updateChartLayout()
{
    Q_D(JChartView);
    //
    QListIterator<JChart *> citer(d->charts);
    while (citer.hasNext()) {
        d->gridLayoutChart->removeWidget(citer.next());
    }
    //
    citer.toBack();
    int rowIndex = 0, columnIndex = 0;
    int columnCount = d->spinBoxColumnCount->value();
    while (citer.hasNext()) {
        d->gridLayoutChart->addWidget(citer.next(), rowIndex, columnIndex++, 1, 1);
        if (columnIndex == columnCount) {
            ++rowIndex;
            columnIndex = 0;
        }
    }
}
