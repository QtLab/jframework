
#ifndef QTN_OFFICEPOPUPMENU_H
#define QTN_OFFICEPOPUPMENU_H

#include <QMenu>

#include "QtRibbonDef.h"


namespace QtRibbon
{
    class OfficePopupMenuPrivate;
    /* OfficePopupMenu */
    class QTRIBBON_EXPORT OfficePopupMenu : public QMenu
    {
        Q_OBJECT
        Q_PROPERTY(bool gripVisible READ isGripVisible WRITE setGripVisible)

    public:
        OfficePopupMenu(QWidget* parent = Q_NULL);
        virtual ~OfficePopupMenu();

        static OfficePopupMenu* createPopupMenu(QWidget* parent = Q_NULL);

    public:
        QAction* addWidget(QWidget* widget);

        void setGripVisible(bool visible);
        bool isGripVisible() const;

    public:
        virtual QSize sizeHint() const;
    protected:
        void setWidgetBar(QWidget* widget);

    private Q_SLOTS:
        void aboutToShowBar();

    protected:
        virtual bool event(QEvent* event);
        virtual void paintEvent(QPaintEvent* event);
        virtual void mousePressEvent(QMouseEvent* event);
        virtual void mouseMoveEvent(QMouseEvent* event);
        virtual void mouseReleaseEvent(QMouseEvent* event);
        virtual void moveEvent(QMoveEvent* event);
        virtual void resizeEvent(QResizeEvent* event);

    private:
        friend class RibbonGalleryPrivate;
        QTN_DECLARE_PRIVATE(OfficePopupMenu)
        Q_DISABLE_COPY(OfficePopupMenu)
    };

} // namespace QtRibbon


#endif // QTN_OFFICEPOPUPMENU_H
