
#ifndef QTN_RIBBONSTATUSBAR_H
#define QTN_RIBBONSTATUSBAR_H

#include <QStatusBar>

#include "QtRibbonDef.h"


namespace QtRibbon
{
    class RibbonStatusBarSwitchGroupPrivate;
    /* RibbonStatusBarSwitchGroup */
    class QTRIBBON_EXPORT RibbonStatusBarSwitchGroup : public QWidget
    {
        Q_OBJECT
    public:
        explicit RibbonStatusBarSwitchGroup();
        virtual ~RibbonStatusBarSwitchGroup();

    public:
        void clear();

    protected:
        virtual void actionEvent(QActionEvent* event);
    private:
        QTN_DECLARE_PRIVATE(RibbonStatusBarSwitchGroup)
        Q_DISABLE_COPY(RibbonStatusBarSwitchGroup)
    };

    class RibbonStatusBarPrivate;
    /* RibbonStatusBar */
    class QTRIBBON_EXPORT RibbonStatusBar : public QStatusBar
    {
        Q_OBJECT
    public:
        explicit RibbonStatusBar(QWidget* parent = Q_NULL);
        virtual ~RibbonStatusBar();

    public:
        QAction* addAction(const QString& text);
        QAction* addSeparator();

        void addPermanentAction(QAction* action);

        QRect childrenPermanentRect() const;

    public:
        using QWidget::addAction;

    protected:
        virtual bool event(QEvent* event);
        virtual void actionEvent(QActionEvent* event);

    private:
        QTN_DECLARE_PRIVATE(RibbonStatusBar)
        Q_DISABLE_COPY(RibbonStatusBar)
    };

} // namespace QtRibbon


#endif // QTN_RIBBONSTATUSBAR_H
