
#ifndef QTN_RIBBONSLIDERPANE_H
#define QTN_RIBBONSLIDERPANE_H

#include <QWidget>

#include "QtRibbonDef.h"

namespace QtRibbon
{
    class RibbonSliderPanePrivate;

    /* RibbonSliderPane */
    class QTRIBBON_EXPORT RibbonSliderPane : public QWidget
    {
        Q_OBJECT
        Q_PROPERTY(int singleStep READ singleStep WRITE setSingleStep)
        Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged USER true)
        Q_PROPERTY(int sliderPosition READ sliderPosition WRITE setSliderPosition NOTIFY sliderMoved)

    public:
        RibbonSliderPane(QWidget* parent = Q_NULL);
        virtual ~RibbonSliderPane();

    public:
        void setRange(int min, int max);
        void setScrollButtons(bool on);

        void setSliderPosition(int);
        int sliderPosition() const;

        void setSingleStep(int);
        int singleStep() const;

        int value() const;

    public Q_SLOTS:
        void setValue(int);

    Q_SIGNALS:
        void valueChanged(int value);
        void sliderMoved(int position);

    protected Q_SLOTS:
        void increment();
        void decrement();

    private:
        QTN_DECLARE_PRIVATE(RibbonSliderPane)
        Q_DISABLE_COPY(RibbonSliderPane)
    };

} // namespace QtRibbon

#endif // QTN_RIBBONSLIDERPANE_H
