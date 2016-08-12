#if defined(PRO_CORE)
#include "precomp.h"
#endif
#include <QApplication>
#include <QStyleOption>
#include <QWidgetAction>
#include <QPainter>
#include <qevent.h>
#include <QLayout>
#include <qdebug.h>

#include "QtnRibbonStyle.h"
#include "QtnOfficeDefines.h"
#include "QtnOfficePopupMenu.h"

using namespace QtRibbon;


/*!
\property QtRibbon::OfficePopupMenu::gripVisible
*/

namespace QtRibbon
{
    /* OfficePopupMenuPrivate */
    class OfficePopupMenuPrivate : public QObject
    {
    public:
        QTN_DECLARE_PUBLIC(OfficePopupMenu)
    public:
        explicit OfficePopupMenuPrivate();

    public:
        void init();
        int calcMinimumHeight();
        QWidget* findWidget(const char* nameWidget) const;

    public:
        QWidget* m_widgetPopup; 
        bool m_resizable      : 1;
        bool m_pressSizeGrip  : 1;
        bool m_showGrip       : 1;
        int m_lastWidth;
    };
};

OfficePopupMenuPrivate::OfficePopupMenuPrivate()
{
    m_widgetPopup = Q_NULL;
    m_resizable = false;
    m_pressSizeGrip = false;
    m_showGrip = true;
    m_lastWidth = -1;
}

void OfficePopupMenuPrivate::init()
{
    QTN_P(OfficePopupMenu);
    p.setProperty(qtn_PopupBar, true);
    p.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    p.setMouseTracking(true);
}

int OfficePopupMenuPrivate::calcMinimumHeight()
{
    QTN_P(OfficePopupMenu);

    int height = 11;

    QList<QAction*> listActions = p.actions();
    for (int i = 0; i < listActions.count(); i++) 
    {
        if (QWidgetAction* widgetAction = qobject_cast<QWidgetAction*>(listActions.at(i)))
        {
            if (QWidget* w = widgetAction->defaultWidget())
            {
                int minHeight = w->minimumSizeHint().height();
                if (minHeight == -1)
                    minHeight = w->sizeHint().height();
                height += minHeight;
            }
        }
        else
            height += p.actionGeometry(listActions.at(i)).height();
    }
    return height;
}

QWidget* OfficePopupMenuPrivate::findWidget(const char* nameWidget) const
{
    QTN_P(const OfficePopupMenu);
    const QObjectList& listChildren = p.children();
    for (int i = 0; i < listChildren.size(); ++i) 
    {
        QWidget* w = qobject_cast<QWidget*>(listChildren.at(i));

        if (w && w->property(nameWidget).toBool() )
            return w;
    }
    return Q_NULL;
}

/*!
class OfficePopupMenu
\brief Constructs the OfficePopupMenu object with the given \a parent.
*/ 
OfficePopupMenu::OfficePopupMenu(QWidget* parent)
    : QMenu(parent)
{
    QTN_INIT_PRIVATE(OfficePopupMenu);
    QTN_D(OfficePopupMenu);
    d.init();

    connect(this, SIGNAL(aboutToShow()), this, SLOT(aboutToShowBar()));
}


/*!
\brief Destructor of the OfficePopupMenu object.
*/ 
OfficePopupMenu::~OfficePopupMenu()
{
    QTN_FINI_PRIVATE();
}

/*!
\brief Creates an instance of the OfficePopupMenu object with given \a parent.
*/ 
OfficePopupMenu* OfficePopupMenu::createPopupMenu(QWidget* parent)
{
    OfficePopupMenu* popupBar = new OfficePopupMenu(parent);
    return popupBar;
}

/*!
\brief Adds \a widget to the Office Popup Window and return an action associated with them.
*/ 
QAction* OfficePopupMenu::addWidget(QWidget* widget)
{
    QTN_D(OfficePopupMenu);
    QWidgetAction* action = new QWidgetAction(this);
    action->setDefaultWidget(widget);
    addAction(action);

    if (widget && widget->property("isResizable").toBool())
        d.m_resizable = true;
	sizeHint();	
    return action;
}

/*!
\brief Sets visibility of the Popup Window Grip to \a visible.
*/ 
void OfficePopupMenu::setGripVisible(bool visible)
{
    QTN_D(OfficePopupMenu);
    d.m_showGrip = visible;
}

/*!
\brief Returns the visibility of the Popup Window Grip.
*/ 
bool OfficePopupMenu::isGripVisible() const
{
    QTN_D(const OfficePopupMenu);
    return d.m_showGrip ;
}

/*! \internal */
void OfficePopupMenu::setWidgetBar(QWidget* widget)
{
    QTN_D(OfficePopupMenu);
    Q_ASSERT(widget != Q_NULL && d.m_widgetPopup == Q_NULL);

    d.m_widgetPopup = widget;
}

/*! \internal */
void OfficePopupMenu::aboutToShowBar()
{
    QApplication::sendEvent(this, new QResizeEvent(size(), QSize(-1, -1)));
}

/*! \reimp */
QSize OfficePopupMenu::sizeHint() const
{
    QSize size = QMenu::sizeHint();
    
    QTN_D(const OfficePopupMenu);

    int height = 0;
    if (d.m_resizable)
        height = 11;// : 8;

    QSize sz;
    if (d.m_widgetPopup)
        sz = d.m_widgetPopup->geometry().size();
    else if (QWidget* w = d.findWidget(qtn_WidgetGallery))
        sz = w->baseSize();

    size.setWidth(d.m_lastWidth != -1 ? d.m_lastWidth : sz.width());
    size.setHeight(size.height() + height);

    return size;
}

/*! \reimp */
bool OfficePopupMenu::event(QEvent* event)
{
    QTN_D(OfficePopupMenu);

    switch(event->type())
    {
        case QEvent::Show :
                if (d.m_widgetPopup && d.m_resizable)
                {
                    setMinimumWidth(d.m_widgetPopup->geometry().width());
                    setMinimumHeight(d.calcMinimumHeight());
                }
            break;
        case QEvent::Resize :
            {
                QResizeEvent* resizeEvent = (QResizeEvent*)event;
                const QSize& sz = resizeEvent->size();
                if (!resizeEvent->oldSize().isEmpty() && resizeEvent->oldSize() != sz)
                {
                    if (QWidget* w = d.findWidget(qtn_WidgetGallery))
                    {
                        if (w && !w->isWindow() && !w->isHidden())
                        {
							QSize szOld = sizeHint();
                            QSize sizeWidget = w->baseSize();
//                            sizeWidget.setHeight(sizeWidget.height()-(resizeEvent->oldSize().height()-sz.height()));
							int offset = szOld.height()-sz.height();
							if ( offset != 0 )
							{
								sizeWidget.setHeight(sizeWidget.height() - offset);
								if (sizeWidget != w->geometry().size())
									w->setBaseSize(sizeWidget);
							}
                        }
                    }
                }
            }
            break;
        default:
            break;
    }
    return QMenu::event(event);
}

/*! \reimp */
void OfficePopupMenu::paintEvent(QPaintEvent* event)
{
    QMenu::paintEvent(event);
    QTN_D(OfficePopupMenu);

    if (d.m_resizable)
    {
        QPainter p(this);
        QStyleOptionSizeGrip opt;
        opt.init(this);
        opt.rect.adjust(1, 0, -1, -1);

        opt.rect.setTop(opt.rect.bottom()-11);

        style()->drawControl((QStyle::ControlElement)RibbonStyle::CE_PopupSizeGrip, &opt, &p, this);
    }
}

/*! \reimp */
void OfficePopupMenu::mousePressEvent(QMouseEvent* event)
{
    QTN_D(OfficePopupMenu);
    if (d.m_resizable)
    {
        if (event->buttons() == Qt::LeftButton) 
        {
            QRect rcResizeGripper(rect());
            rcResizeGripper.setTop(rcResizeGripper.bottom()-11);
            QRect rcResizeGripperAll = rcResizeGripper;
            rcResizeGripper.setLeft(rcResizeGripper.right() - rcResizeGripper.height());
            if (rcResizeGripper.contains(event->pos()))
            {
                if (QWidget* w = d.findWidget(qtn_WidgetGallery))
                {
                    if (w && !w->isWindow() && !w->isHidden())
                        w->setAttribute(Qt::WA_TransparentForMouseEvents, true);
                }
                d.m_pressSizeGrip = true;
                return;
            }
            else if (rcResizeGripperAll.contains(event->pos()))
                return;
        }
    }
    QMenu::mousePressEvent(event);
}

/*! \reimp */
void OfficePopupMenu::mouseMoveEvent(QMouseEvent* event)
{
    QTN_D(OfficePopupMenu);
    if (d.m_resizable)
    {
        QRect rcResizeGripper(rect());
        rcResizeGripper.setTop(rcResizeGripper.bottom()-11);
        rcResizeGripper.setLeft(rcResizeGripper.right() - rcResizeGripper.height());

        if (d.m_pressSizeGrip || rcResizeGripper.contains(event->pos()))
            setCursor(Qt::SizeFDiagCursor);
        else
            unsetCursor();

        if (d.m_pressSizeGrip)
        {
            QPoint np(event->globalPos());

            QRect rect = geometry();
//            QRect rectOld = rect;

            rect.setRight(np.x());
            rect.setBottom(np.y());

            d.m_lastWidth = rect.width();
            setGeometry(rect);
            return;
        }
    }

    if (!d.m_pressSizeGrip)
        QMenu::mouseMoveEvent(event);
}

/*! \reimp */
void OfficePopupMenu::mouseReleaseEvent(QMouseEvent* event)
{
    QTN_D(OfficePopupMenu);
    d.m_pressSizeGrip = false;

    if (QWidget* w = d.findWidget(qtn_WidgetGallery))
    {
        if (w && !w->isWindow() && !w->isHidden())
            w->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    }
    QMenu::mouseReleaseEvent(event);
}

/*! \reimp */
void OfficePopupMenu::moveEvent(QMoveEvent* event)
{
    Q_UNUSED(event);
    QTN_D(OfficePopupMenu);
    if (d.m_pressSizeGrip)
        setCursor(Qt::SizeFDiagCursor);
}

/*! \reimp */
void OfficePopupMenu::resizeEvent(QResizeEvent* event)
{
    QMenu::resizeEvent(event);
}

