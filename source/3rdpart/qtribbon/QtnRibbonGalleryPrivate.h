
#ifndef QTN_RIBBONGALLERYPRIVATE_H
#define QTN_RIBBONGALLERYPRIVATE_H

#include <QRect>
#include <QVector>
#include <QVariant>
#include <QBasicTimer>
#include <QIcon>

#include "QtRibbonDef.h"

class QPainter;
class QScrollBar;
class QAction;

class QMenu;
class QToolButton;

namespace QtRibbon
{
    class RibbonGalleryGroup;
    /* WidgetItemData */
    class WidgetItemData
    {
    public:
        inline WidgetItemData() : role(-1) {}
        inline WidgetItemData(int r, QVariant v) : role(r), value(v) {}
        int role;
        QVariant value;
        inline bool operator==(const WidgetItemData &other) { return role == other.role && value == other.value; }
    };

    class RibbonGalleryItem;
    /* RibbonGalleryItemPrivate */
    class RibbonGalleryItemPrivate: public QObject
    {
    public:
        QTN_DECLARE_PUBLIC(RibbonGalleryItem)
    public:
        explicit RibbonGalleryItemPrivate();
    public:
        void init();
    public:
        int m_index;
        QString m_descriptionText;
        RibbonGalleryGroup* m_items;
        bool m_separator : 1;
        bool m_enabled   : 1;
        bool m_visible   : 1;

        QVector<WidgetItemData> values;
    };

    class RibbonGalleryGroup;
    /* RibbonGalleryGroupPrivate */
    class RibbonGalleryGroupPrivate : public QObject
    {
    public:
        QTN_DECLARE_PUBLIC(RibbonGalleryGroup)
    public:
        explicit RibbonGalleryGroupPrivate();
        virtual ~RibbonGalleryGroupPrivate();

    public:
        void init();
        void updateIndexes(int start /*= 0*/);
        void itemsChanged();
        void redrawWidget();

        void clear();

     public:
        bool m_clipItems;
        QSize m_sizeItem;
        QVector<RibbonGalleryItem*> m_arrItems;
        QList<QWidget*> m_viewWidgets;
    };


    class RibbonGallery;

    struct qtn_galleryitem_rect
    {
        RibbonGalleryItem* item;
        QRect rect;
        bool beginRow;
    };

    class OfficePopupMenu;
    /* RibbonGalleryPrivate */
    class RibbonGalleryPrivate : public QObject
    {
    public:
        Q_OBJECT
        QTN_DECLARE_PUBLIC(RibbonGallery)
    public:
        explicit RibbonGalleryPrivate();
        virtual ~RibbonGalleryPrivate();

    public:
        void init();
        void layoutItems();
        void layoutScrollBar();
        void setScrollBarValue();
        void setScrollBarPolicy(Qt::ScrollBarPolicy policy);
        Qt::ScrollBarPolicy scrollBarPolicy() const;
        void setPopupMenu(OfficePopupMenu* popup);

        void drawItems(QPainter* painter);
        void repaintItems(QRect* pRect = Q_NULL, bool bAnimate = true);

        bool isItemChecked(RibbonGalleryItem* pItem) const;
        bool isScrollButtonEnabled(bool buttonUp);

        int scrollWidth() const;

        void startAnimation(int scrollPos);
        void startAnimate();

    public Q_SLOTS:
        void setScrollPos(int nScrollPos);
        void actionTriggered(int action);
        void pressedScrollUp();
        void pressedScrollDown();

    protected:
        virtual bool event(QEvent* event);
        virtual bool eventFilter(QObject* object, QEvent* event);

    public:
        QScrollBar* m_scrollBar; 
        QToolButton* m_buttonScrollUp;
        QToolButton* m_buttonScrollDown;
        QToolButton* m_buttonPopup;
        OfficePopupMenu* m_menuBar;
        QVector<qtn_galleryitem_rect> m_arrRects;

        bool m_showBorders       : 1;
        bool m_showLabels        : 1;
        bool m_hideSelection     : 1;
        bool m_keyboardSelected  : 1;
        bool m_preview           : 1;
        bool m_pressed           : 1;
        bool m_animation         : 1;

        int m_totalHeight;
        int m_scrollPos;
        int m_scrollPosTarget;
        double m_animationStep;
        int m_timerElapse;

        int m_selected;
        RibbonGalleryItem* m_checkedItem;
        RibbonGalleryGroup* m_items;

        QPoint m_ptPressed;
        QBasicTimer m_scrollTimer;
    };
} // namespace QtRibbon


#endif // QTN_RIBBONGALLERYPRIVATE_H
