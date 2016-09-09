
#ifndef QTN_RIBBONBACKSTAGEPVIEWRIVATE_H
#define QTN_RIBBONBACKSTAGEPVIEWRIVATE_H

#include <QObject>
#include <QTimer>
#include <QHash>

#include "QtnRibbonBackstageView.h"

class QStyleOptionMenuItem;
class QWidgetAction;

namespace QtRibbon
{
    class RibbonBar;
    class HackFilter;
    /* RibbonBackstageViewPrivate */
    class RibbonBackstageViewPrivate : public QObject
    {
    public:
        Q_OBJECT
        QTN_DECLARE_PUBLIC(RibbonBackstageView)
    public:
        explicit RibbonBackstageViewPrivate();
        virtual ~RibbonBackstageViewPrivate();

    public:
        void init(QWidget* parent);
        void initStyleOption(QStyleOptionMenuItem* option, const QAction* action) const;
        void layoutBackstage();
        void updateActionRects() const;
        void updateGeometryPage(QWidget* widget);

        void setCurrentAction(QAction *action);
        QAction* currentAction() const;

        QRect actionRect(QAction* act) const;

        QWidgetAction* getAction(QWidget* w) const;
        QAction* actionAt(const QPoint& p) const;

    protected slots:
        void trackMouseEvent();

    public:
        RibbonBar* ribbon_; 
        QWidget* activePage_;
        QAction* currentAction_;

        bool mouseDown_;
        bool closePrevented_;

        QTimer timer_;
        mutable bool itemsDirty_;
        mutable int menuWidth_;
        mutable int maxIconWidth_;
        mutable QVector<QRect> actionRects_;
        mutable QHash<QAction*, QWidget*> widgetItems_;
    };
} // namespace QtRibbon


#endif // QTN_RIBBONBACKSTAGEPVIEWRIVATE_H

