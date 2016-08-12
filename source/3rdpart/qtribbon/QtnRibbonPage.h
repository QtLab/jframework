
#ifndef QTN_RIBBONPAGE_H
#define QTN_RIBBONPAGE_H

#include <QWidget>

#include "QtRibbonDef.h"

namespace QtRibbon
{
    class RibbonGroup;
    class RibbonPagePrivate;

    enum ContextColor
    { 
        ContextColorNone,
        ContextColorGreen,
        ContextColorBlue,
        ContextColorRed,
        ContextColorYellow,
        ContextColorCyan,
        ContextColorPurple,
        ContextColorOrange
    };

    /* RibbonPage */
    class QTRIBBON_EXPORT RibbonPage : public QWidget
    {
        Q_OBJECT
        Q_PROPERTY(const QString& title READ title WRITE setTitle)
        Q_PROPERTY(const QString& contextTitle READ contextTitle WRITE setContextTitle)
        Q_PROPERTY(int groupCount READ groupCount)
        Q_PROPERTY(ContextColor getContextColor READ getContextColor WRITE setContextPage)
        Q_ENUMS(ContextColor)

    public:
        RibbonPage(QWidget* parent = Q_NULL);
        RibbonPage(QWidget* parent, const QString& title);
        virtual ~RibbonPage();

    public:
        bool isVisible() const;

    public:
        RibbonGroup* addGroup(const QString& title);
        RibbonGroup* addGroup(const QIcon& icon, const QString& title);
        RibbonGroup* insertGroup(int index, const QString& title);
        RibbonGroup* insertGroup(int index, const QIcon& icon, const QString& title);

        void removeGroup(RibbonGroup* group);
        void removeGroup(int index);

        QAction* defaultAction() const;

        int groupCount() const;
        RibbonGroup* getGroup(int index) const;

        void clearGroups();

        void setContextPage(ContextColor color);
        ContextColor getContextColor() const;

        const QString& title() const;
        const QString& contextTitle() const;

    public Q_SLOTS:
        virtual void setVisible(bool visible);
        void setTitle(const QString& title);
        void setContextTitle(const QString& title);

    Q_SIGNALS:
        void titleChanged(const QString& title);
        void activated();
        void activating(bool& allow);

    protected slots:
        void released();
        void actionTriggered(QAction* action);

    private:
        void setAssociativeTab(QWidget* widget);
        void setPageVisible(bool visible);
        void popup();
        void setRibbonMinimized(bool minimized);

    protected:
        void activatingPage(bool& allow);
        void setGroupsHeight(int height);

    public:
        virtual QSize sizeHint() const;

    protected:
        virtual bool event(QEvent* event);
        virtual void paintEvent(QPaintEvent* event);
        virtual void changeEvent(QEvent* event);
        virtual void showEvent(QShowEvent* event);
        virtual void resizeEvent(QResizeEvent* event);

#ifdef Q_OS_WIN
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result);
#else
        virtual bool winEvent(MSG* message, long* result);
#endif
#endif // Q_OS_WIN

    private:
        friend class RibbonBar;
        friend class RibbonBarPrivate;
        friend class RibbonTabBar;
        QTN_DECLARE_PRIVATE(RibbonPage)
        Q_DISABLE_COPY(RibbonPage)
    };

} // namespace QtRibbon

#endif // QTN_RIBBONPAGE_H
