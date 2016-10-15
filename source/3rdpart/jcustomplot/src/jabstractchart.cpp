#include "precomp.h"
#include "jabstractchart.h"

// class JAbstractChartPrivate

class JAbstractChartPrivate
{
public:
    JAbstractChartPrivate(JAbstractChart *parent)
        : q_ptr(parent)
    {

    }

    void init();

private:
    J_DECLARE_PUBLIC(JAbstractChart)
};

void JAbstractChartPrivate::init()
{

}

// class JAbstractChart

JAbstractChart::JAbstractChart(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new JAbstractChartPrivate(this))
{
    Q_D(JAbstractChart);
    d->init();
}

JAbstractChart::~JAbstractChart()
{
    Q_D(JAbstractChart);
    delete d;
}
