
#ifndef QTN_RIBBONBARPRIVATE_H
#define QTN_RIBBONBARPRIVATE_H

#include <QStyle>
#include <QStack>

#include "QtnRibbonBar.h"
#include "QtnRibbonTabBar.h"

#include "QtRibbonDef.h"


namespace QtRibbon
{
    class RibbonTabBar;
    class RibbonTitleButton;
    class OfficeFrameHelper;
    class StyleOptionTitleBar;
    class RibbonSystemButton;
    class MenuMinimizedGroups;
    class RibbonKeyTip;

    /* RibbonBarPrivate */
    class RibbonBarPrivate : public QObject
    {
    public:
        Q_OBJECT
        QTN_DECLARE_PUBLIC(RibbonBar)
    public:
        explicit RibbonBarPrivate();
        virtual ~RibbonBarPrivate();

    public:
        void init(); 
        void layoutRibbon();
        void layoutTitleButton();
        void layoutContextHeaders();
        void layoutCorner();

        void removeAllContextHeaders();
        bool reduceContextHeaders();

        void updateMinimizedModePage(RibbonPage* page);
        void swapVisiblePages(int index);

        QAction* createSystemButton(const QIcon& icon, const QString& text);

        RibbonPage* insertPage(int indTab, int index);
        void insertPage(int indTab, int index, RibbonPage* page);
        void removePage(int index, bool deletePage = true);
        int getIndexPage(RibbonPage* page) const;

        void currentChanged(int index);
        bool validIndex(int index) const { return index >= 0 && index < m_listPages.count(); }

        void titleBarOptions(StyleOptionTitleBar& opt) const;
        QStyle::SubControl getSubControl(const QPoint& pos) const;

        RibbonTitleButton* findTitleButton(QStyle::SubControl subControl) const;
        QAction* addTitleButton(QStyle::SubControl subControl, bool add, QRect& rcControl);

        ContextHeader* hitTestContextHeaders(const QPoint& point) const;

        void showKeyTips(QWidget* w);
        bool hideKeyTips();
        void createKeyTips();
        void createGroupKeyTips();
        void createWidgetKeyTips(RibbonGroup* group, QWidget* widget, const QString& prefix, const QRect& rcGroups, bool visible);
        bool createPopupKeyTips(QMenu* levalMenu);
        void destroyKeyTips();
        void calcPositionKeyTips();
        int getMaxHeightPages() const;

    private Q_SLOTS:
        void processClickedSubControl();

    protected:
        bool pressTipCharEvent(const QKeyEvent* key);
        virtual bool eventFilter(QObject*, QEvent* event);

    public:
        QRect m_rcPageArea;
        QRect m_rcTabBar;
        int m_currentIndexPage;
        RibbonTabBar* m_tabBar;
        RibbonQuickAccessBar* m_quickAccessBar; 
        RibbonSystemButton* m_controlSystemButton;
        OfficeFrameHelper* m_frameHelper;
        QRect m_rcTitle;
        QRect m_rcHeader;
        QRect m_rcTitleText;
        QRect m_rcQuickAccess;

        bool m_showTitleAlways;
        bool m_quickAccessOnTop;
        bool m_ribbonBarVisible;
        bool m_minimizationEnabled;
        bool m_minimized;
        bool m_keyTipsComplement;
        bool m_keyTipsEnabled;
        bool m_keyTipsShowing;

        QAction* m_actCloseButton;
        QAction* m_actNormalButton;
        QAction* m_actMaxButton;
        QAction* m_actMinButton;

        QList<ContextHeader*> m_listContextHeaders;
        QList<RibbonKeyTip*> m_keyTips;
        QStack<QWidget*> m_levels;
        int m_countKey;

        QStyle::SubControl m_hoveredSubControl;
        QStyle::SubControl m_activeSubControl;
        QString m_windowTitle;

    protected:
        QList<RibbonPage*> m_listPages;
        QVector<int> m_dirtyPages;
        bool m_wheelEvent;
    private:
        Q_DISABLE_COPY(RibbonBarPrivate)
    };
} // namespace QtRibbon


#endif // QTN_RIBBONPRIVATE_H


