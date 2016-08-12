
#ifndef QTN_RIBBONBACKSTAGEVIEW_H
#define QTN_RIBBONBACKSTAGEVIEW_H

#include <QFrame>
#include <QToolButton>

#include "QtRibbonDef.h"


namespace QtRibbon
{
    /* RibbonBackstageSeparator */
    class QTRIBBON_EXPORT RibbonBackstageSeparator : public QFrame
    {
        Q_OBJECT
        Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)
    public:
        RibbonBackstageSeparator(QWidget* parent);
        virtual ~RibbonBackstageSeparator();

    public:
        void setOrientation(Qt::Orientation orient);
        Qt::Orientation orientation() const;

    private:
        Q_DISABLE_COPY(RibbonBackstageSeparator)
    };

    class RibbonBackstageButtonPrivate;
    /* RibbonBackstagePage */
    class QTRIBBON_EXPORT RibbonBackstageButton : public QToolButton
    {
        Q_OBJECT
        Q_PROPERTY(bool tabStyle READ tabStyle WRITE setTabStyle)
        Q_PROPERTY(bool flatStyle READ flatStyle WRITE setFlatStyle)
    public:
        RibbonBackstageButton(QWidget* parent);
        virtual ~RibbonBackstageButton();

    public:
        bool tabStyle() const;
        void setTabStyle(bool style);

        bool flatStyle();
        void setFlatStyle(bool flat);

    protected:
        virtual void paintEvent(QPaintEvent* event);

    private:
        QTN_DECLARE_PRIVATE(RibbonBackstageButton)
        Q_DISABLE_COPY(RibbonBackstageButton)
    };


    /* RibbonBackstagePage */
    class QTRIBBON_EXPORT RibbonBackstagePage : public QWidget
    {
        Q_OBJECT
    public:
        RibbonBackstagePage(QWidget* parent);
        virtual ~RibbonBackstagePage();

    protected:
        virtual void paintEvent(QPaintEvent* event);

    private:
        Q_DISABLE_COPY(RibbonBackstagePage)
    };


    class RibbonBackstageViewPrivate;
    /* RibbonBackstageView */
    class QTRIBBON_EXPORT RibbonBackstageView : public QWidget
    {
        Q_OBJECT
    public:
        RibbonBackstageView(QWidget* parent = Q_NULL);
        virtual ~RibbonBackstageView();

    public:
        bool isClosePrevented() const;
        void setClosePrevented(bool prevent);

        QAction* addAction(const QIcon& icon, const QString& text);
        QAction* addPage(QWidget* widget);

        void setActivePage(QWidget* widget);
        QWidget* getActivePage() const;

        QRect actionGeometry(QAction* ) const;

        using QWidget::addAction;

    public Q_SLOTS:
        void open();

    Q_SIGNALS:
        void aboutToShow();
        void aboutToHide();

    protected:
        virtual bool event(QEvent* event);
        virtual bool eventFilter(QObject* object, QEvent* event);
        virtual void paintEvent(QPaintEvent* event);
        virtual void actionEvent(QActionEvent* event);
        virtual void mousePressEvent(QMouseEvent* event);
        virtual void mouseMoveEvent(QMouseEvent* event);
        virtual void mouseReleaseEvent(QMouseEvent* event);

    private:
        QTN_DECLARE_PRIVATE(RibbonBackstageView)
        Q_DISABLE_COPY(RibbonBackstageView)
    };

} // namespace QtRibbon


#endif // QTN_RIBBONBACKSTAGEVIEW_H
