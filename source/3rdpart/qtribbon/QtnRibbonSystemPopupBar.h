
#ifndef QTN_RIBBONSYSTEMPOPUPBAR_H
#define QTN_RIBBONSYSTEMPOPUPBAR_H

#include <QMenu>
#include <QToolButton>

#include "QtRibbonDef.h"

namespace QtRibbon
{
    class RibbonBackstageView;
    class RibbonSystemButtonPrivate;

    /* RibbonSystemButton */
    class QTRIBBON_EXPORT RibbonSystemButton : public QToolButton
    {
        Q_OBJECT
        Q_PROPERTY(Qt::ToolButtonStyle toolButtonStyle READ toolButtonStyle WRITE setToolButtonStyle)
    public:
        RibbonSystemButton(QWidget* parent);
        virtual ~RibbonSystemButton();

    public:
        void setBackstage(RibbonBackstageView* backstage);
        RibbonBackstageView* backstage() const;

        void setBackgroundColor(const QColor& color);
        QColor backgroundColor() const;

    public:
        virtual QSize sizeHint() const;

    protected:
        virtual void paintEvent(QPaintEvent* event);
        virtual void mousePressEvent(QMouseEvent* event);

    private:
        Q_DISABLE_COPY(RibbonSystemButton)
        QTN_DECLARE_PRIVATE(RibbonSystemButton)
    };


    class RibbonPageSystemRecentFileList;
    class RibbonPageSystemPopup;
    class RibbonSystemPopupBarPrivate;

    /* RibbonSystemPopupBar */
    class QTRIBBON_EXPORT RibbonSystemPopupBar : public QMenu
    {
        Q_OBJECT
    public:
        RibbonSystemPopupBar(QWidget* parent);
        virtual ~RibbonSystemPopupBar();

    public:
        QAction* addPopupBarAction(const QString& text);
        void addPopupBarAction(QAction* action, Qt::ToolButtonStyle style = Qt::ToolButtonTextOnly);

        RibbonPageSystemRecentFileList* addPageRecentFile(const QString& caption);
        RibbonPageSystemPopup* addPageSystemPopup(const QString& caption, QAction* defaultAction, bool splitAction);

    public:
        virtual QSize sizeHint() const;
        
    protected:
        virtual void showEvent(QShowEvent* event);
        virtual void paintEvent(QPaintEvent* event);
        virtual void mousePressEvent(QMouseEvent* event);
        virtual void mouseReleaseEvent(QMouseEvent* event);
        virtual void keyPressEvent(QKeyEvent* event);
        virtual void resizeEvent(QResizeEvent* event);

    private:
        friend class RibbonPageSystemPopup;
        Q_DISABLE_COPY(RibbonSystemPopupBar)
        QTN_DECLARE_PRIVATE(RibbonSystemPopupBar)
    };

    /* RibbonSystemRecentFileList */
    class QTRIBBON_EXPORT RibbonPageSystemPopupListCaption : public QWidget
    {
        Q_OBJECT
    public:
        RibbonPageSystemPopupListCaption(const QString& caption);
        virtual ~RibbonPageSystemPopupListCaption();

    public:
        virtual QSize sizeHint() const;

    protected:
        virtual void drawEvent(QPaintEvent* event, QPainter* p);

    protected:
        virtual void paintEvent(QPaintEvent* event);

    private:
        friend class RibbonSystemPopupBar;
        Q_DISABLE_COPY(RibbonPageSystemPopupListCaption)
    };

    class RibbonPageSystemRecentFileListPrivate;
    /* RibbonSystemRecentFileList */
    class QTRIBBON_EXPORT RibbonPageSystemRecentFileList : public RibbonPageSystemPopupListCaption
    {
        Q_OBJECT
    public:
        RibbonPageSystemRecentFileList(const QString& caption);
        virtual ~RibbonPageSystemRecentFileList();

    public:
        void setSize(int size);
        int getSize() const; 

        QAction* getCurrentAction() const;

    public Q_SLOTS:
        void updateRecentFileActions(const QStringList& files);

    Q_SIGNALS:
        void openRecentFile(const QString& file);

    protected:
        virtual void drawEvent(QPaintEvent* event, QPainter* p);

    protected:
        virtual void mousePressEvent(QMouseEvent* event);
        virtual void mouseMoveEvent(QMouseEvent* event);
        virtual void mouseReleaseEvent(QMouseEvent* event);
        virtual void enterEvent(QEvent* event);
        virtual void leaveEvent(QEvent* event);
        virtual void keyPressEvent(QKeyEvent* event);

        virtual void resizeEvent(QResizeEvent* event);

    private:
        Q_DISABLE_COPY(RibbonPageSystemRecentFileList)
        QTN_DECLARE_PRIVATE(RibbonPageSystemRecentFileList)
    };

    class RibbonPageSystemPopupPrivate;
    /* RibbonPageSystemPopup */
    class QTRIBBON_EXPORT RibbonPageSystemPopup : public QMenu
    {
        Q_OBJECT
    public:
        RibbonPageSystemPopup(const QString& caption);
        virtual ~RibbonPageSystemPopup();

    protected:
        virtual bool event(QEvent* event);
        virtual void paintEvent(QPaintEvent* event);

    private:
        Q_DISABLE_COPY(RibbonPageSystemPopup)
        QTN_DECLARE_PRIVATE(RibbonPageSystemPopup)
    };

} // namespace QtRibbon

#endif // QTN_RIBBONSYSTEMPOPUPBAR_H
