
#ifndef QTN_RIBBONGROUP_H
#define QTN_RIBBONGROUP_H

#include <QWidget>
#include <QToolButton>

#include "QtRibbonDef.h"


class QMenu;

namespace QtRibbon
{
    class RibbonGroupPrivate;
    class OfficePopupMenu;
    /* RibbonGroup */
    class QTRIBBON_EXPORT RibbonGroup : public QWidget
    {
        Q_OBJECT
        Q_PROPERTY(bool isControlsGrouping READ isControlsGrouping WRITE setControlsGrouping)
        Q_PROPERTY(bool isControlsCentering READ isControlsCentering WRITE setControlsCentering)
        Q_PROPERTY(bool reduced READ isReduced)
        Q_PROPERTY(const QString& title READ title WRITE setTitle)
        Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
        Q_PROPERTY(bool isOptionButtonVisible READ isOptionButtonVisible WRITE setOptionButtonVisible DESIGNABLE false)

    public:
        explicit RibbonGroup(QWidget* parent, const QString& title);
        virtual ~RibbonGroup();

    public:
        bool isControlsGrouping() const;
        void setControlsGrouping(bool controlsGrouping = true);

        void setControlsCentering(bool controlsCentering = true);
        bool isControlsCentering() const;

        bool isReduced() const;

        void setTitle(const QString& title);
        const QString& title() const;
        
        void setIcon(const QIcon& icon);
        const QIcon& icon() const;

        void setOptionButtonVisible(bool visible = true);
        bool isOptionButtonVisible() const;

        QAction* getOptionButtonAction() const;
        void setOptionButtonAction(QAction* action);

    public:
        QAction* addAction(const QIcon& icon, const QString& text, Qt::ToolButtonStyle style, 
            QMenu* menu = Q_NULL, QToolButton::ToolButtonPopupMode mode = QToolButton::MenuButtonPopup);
        QAction* addAction(QAction* action, Qt::ToolButtonStyle style, QMenu* menu = Q_NULL, 
            QToolButton::ToolButtonPopupMode mode = QToolButton::MenuButtonPopup);
        QAction* addWidget(QWidget* widget);
        QAction* addWidget(const QIcon& icon, const QString& text, QWidget* widget);
        QAction* addWidget(const QIcon& icon, const QString& text, bool align, QWidget* widget);
        QMenu* addMenu(const QIcon& icon, const QString& text, Qt::ToolButtonStyle style = Qt::ToolButtonFollowStyle);
        QAction* addSeparator();

        void remove(QWidget* widget);
        void clear();

    public:
        virtual QSize sizeHint() const;
        using QWidget::addAction;

    Q_SIGNALS:
        /*! \internal */
        void released();
        /*! \internal */
        void actionTriggered(QAction*);

    protected slots:
        /*! \internal */
        void currentIndexChanged(int index);

    protected:
        virtual bool event(QEvent* event);
        virtual void paintEvent(QPaintEvent* event);
        virtual void actionEvent(QActionEvent* event);
        virtual void changeEvent(QEvent* event);
        virtual void enterEvent(QEvent* event);
        virtual void leaveEvent(QEvent* event);
        virtual void resizeEvent(QResizeEvent* event);
        virtual void mouseReleaseEvent(QMouseEvent* event);

    private:
        friend class RibbonStyle;
        friend class GroupDefaultLayout;
        friend class RibbonGroupWrapper;

        QTN_DECLARE_PRIVATE(RibbonGroup)
        Q_DISABLE_COPY(RibbonGroup)
    };

} // namespace QtRibbon

#endif // QTN_RIBBONGROUP_H
