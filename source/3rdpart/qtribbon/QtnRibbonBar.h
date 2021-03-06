
#ifndef QTN_RIBBONBAR_H
#define QTN_RIBBONBAR_H

#include <QMenuBar>

#include "QtRibbonDef.h"


class QMenu;
class QToolButton;

namespace QtRibbon
{
    class RibbonPage;
    class RibbonBarPrivate;
    class RibbonSystemButton;
    class RibbonQuickAccessBar;
    /* RibbonBar */
    class QTRIBBON_EXPORT RibbonBar : public QMenuBar
    {
        Q_OBJECT
        Q_PROPERTY(int currentIndexPage READ currentIndexPage WRITE setCurrentPage NOTIFY currentPageChanged)
        Q_PROPERTY(bool minimized READ isMinimized WRITE setMinimized NOTIFY minimizationChanged)
        Q_PROPERTY(bool maximized READ isMaximized WRITE setMaximized NOTIFY minimizationChanged)
    public:
        RibbonBar(QWidget* parent = Q_NULL);
        virtual ~RibbonBar();

    public:
        bool isVisible() const;

    public:
        RibbonPage* addPage(const QString& text);
        void addPage(RibbonPage* page);

        RibbonPage* insertPage(int index, const QString& text);
        void insertPage(int index, RibbonPage* page);

        void movePage(RibbonPage* page, int newIndex);
        void movePage(int index, int newIndex);

        void removePage(RibbonPage* page);
        void removePage(int index);

        void detachPage(RibbonPage* page);
        void detachPage(int index);

        void clearPages();

        bool isKeyTipsShowing() const;
        bool keyTipsEnabled() const;
        void setKeyTipsEnable(bool enable);
        bool isKeyTipsComplement() const;
        void setKeyTipsComplement(bool complement);
        void setKeyTip(QAction* action, const QString& keyTip);

        RibbonQuickAccessBar* getQuickAccessBar() const;
        void showQuickAccess(bool show = true);
        bool isQuickAccessVisible() const;

        void minimize();
        bool isMinimized() const;
        void setMinimized(bool flag);
        
        void maximize();
        bool isMaximized() const;
        void setMaximized(bool flag);

        void setMinimizationEnabled(bool enabled);
        bool isMinimizationEnabled() const;

    public Q_SLOTS:
        void setCurrentPage(int index);

    public:
        int currentIndexPage() const;
        RibbonPage* getPage(int index) const;
        int getPageCount() const;

    public:
        QMenu* addMenu(const QString& text);
        QAction* addAction(const QIcon& icon, const QString& text, Qt::ToolButtonStyle style, QMenu* menu = Q_NULL);
        QAction* addAction(QAction* action, Qt::ToolButtonStyle style);

        QAction* addSystemButton(const QString& text);
        QAction* addSystemButton(const QIcon& icon, const QString& text);

        RibbonSystemButton* getSystemButton() const;

        bool isBackstageVisible() const;

        void setFrameThemeEnabled(bool enable = true);
        bool isFrameThemeEnabled() const;

        bool isTitleBarVisible() const;
        void setTitleBarVisible(bool show);

        int titleBarHeight() const;
        int topBorder() const;

    private Q_SLOTS:
        void currentChanged(int index);

    Q_SIGNALS:
        void minimizationChanged(bool minimized);
        void currentPageChanged(int index);
        void currentPageChanged(RibbonPage* page);
        void keyTipsShowed(bool showed);

    protected:
        int tabBarHeight() const;
        void setMinimizedFlag(bool flag);

    public:
        virtual int heightForWidth(int) const;
        virtual QSize sizeHint() const;

    protected:
        virtual bool event(QEvent* event);
        virtual bool eventFilter(QObject* object, QEvent* event);
        virtual void paintEvent(QPaintEvent* p);
        virtual void changeEvent(QEvent* event);
        virtual void resizeEvent(QResizeEvent* event);

        virtual void mouseDoubleClickEvent(QMouseEvent* event);
        virtual void mousePressEvent(QMouseEvent* event);
        virtual void wheelEvent(QWheelEvent* event);

#ifdef Q_OS_WIN
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result);
#else
        virtual bool winEvent(MSG* message, long* result);
#endif
#endif // Q_OS_WIN
    private:
        friend class RibbonSystemPopupBar;
        friend class RibbonBackstageViewPrivate;
        friend class RibbonBackstageView;
        friend class RibbonStyle;
        #ifdef Q_OS_WIN
        friend class OfficeFrameHelperWin;
        #endif // Q_OS_WIN
        QTN_DECLARE_PRIVATE(RibbonBar)
        Q_DISABLE_COPY(RibbonBar)
    };

} // namespace QtRibbon


#endif // QTN_RIBBONBAR_H


