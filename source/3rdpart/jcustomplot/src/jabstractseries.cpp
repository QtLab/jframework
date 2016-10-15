#include "precomp.h"
#include "jabstractseries.h"

// class JAbstractSeriesPrivate

class JAbstractSeriesPrivate
{
public:
    JAbstractSeriesPrivate(JAbstractSeries *parent)
        : q_ptr(parent)
    {

    }

    void init();

private:
    J_DECLARE_PUBLIC(JAbstractSeries)
};

void JAbstractSeriesPrivate::init()
{

}

// class JAbstractSeries

JAbstractSeries::JAbstractSeries(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new JAbstractSeriesPrivate(this))
{
    Q_D(JAbstractSeries);
    d->init();
}

JAbstractSeries::~JAbstractSeries()
{
    Q_D(JAbstractSeries);
    delete d;
}
