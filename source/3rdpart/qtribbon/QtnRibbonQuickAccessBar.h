
#ifndef QTN_RIBBONQUICKACCESSBAR_H
#define QTN_RIBBONQUICKACCESSBAR_H

#include <QToolBar>

#include "QtRibbonDef.h"


namespace QtRibbon
{
    class RibbonQuickAccessBarPrivate;
    /* RibbonQuickAccessBar */
    class QTRIBBON_EXPORT RibbonQuickAccessBar : public QToolBar
    {
        Q_OBJECT
    public:
        RibbonQuickAccessBar(QWidget* parent = Q_NULL);
        virtual ~RibbonQuickAccessBar();

    public:
        QAction* actionCustomizeButton() const;
        void setActionVisible(QAction* action, bool visible);
        bool isActionVisible(QAction* action) const;
        int getVisibleCount() const; 

    public:
        virtual QSize sizeHint() const;

    protected slots:
        void customizeAction(QAction*);

    protected:
        virtual bool event(QEvent* event);
        virtual void actionEvent(QActionEvent*);
        virtual void paintEvent(QPaintEvent*);

    private:
        QTN_DECLARE_PRIVATE(RibbonQuickAccessBar)
        Q_DISABLE_COPY(RibbonQuickAccessBar)
    };

} // namespace QtRibbon


#endif // QTN_RIBBONQUICKACCESSBAR_H
