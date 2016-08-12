
#ifndef QTN_RIBBONBUTTON_H
#define QTN_RIBBONBUTTON_H

#include <QStyle>
#include <QToolButton>

#include "QtRibbonDef.h"

class QStyleOption;
namespace QtRibbon
{
    /* RibbonButton */
    class RibbonButton : public QToolButton
    {
        Q_OBJECT
    public:
        RibbonButton(QWidget* parent = Q_NULL);
        virtual ~RibbonButton();

    public:
        virtual QSize sizeHint() const;
    private:
        Q_DISABLE_COPY(RibbonButton)
    };

    /* RibbonSeparator */
    class RibbonSeparator : public QWidget
    {
        Q_OBJECT
    public:
        explicit RibbonSeparator(QWidget* parent = Q_NULL);

    public:
        virtual QSize sizeHint() const;

    protected:
        virtual void paintEvent(QPaintEvent*);

    protected:
        void initStyleOption(QStyleOption* option) const;

    private:
        Q_DISABLE_COPY(RibbonSeparator)
    };


    /* RibbonTitleButton */
    class RibbonTitleButton : public QToolButton
    {
        Q_OBJECT
    public:
        RibbonTitleButton(QWidget* parent, QStyle::SubControl subControl);
        virtual ~RibbonTitleButton();

    public:
        QStyle::SubControl getSubControl() const;

    protected:
        virtual void paintEvent(QPaintEvent*);

    private:
        QStyle::SubControl m_subControl;
    };
} // namespace QtRibbon

#endif // QTN_RIBBONBUTTON_H
