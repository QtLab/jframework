
#ifndef QTN_RIBBONMAINWINDOW_H
#define QTN_RIBBONMAINWINDOW_H

#include <QMainWindow>

#include "QtRibbonDef.h"

namespace QtRibbon
{
    class StatusBar;
    class RibbonBar;
    class OfficeFrameHelper;
    class RibbonMainWindowPrivate;

    /* RibbonMainWindow */
    class QTRIBBON_EXPORT RibbonMainWindow : public QMainWindow
    {
        Q_OBJECT
    public:
        RibbonMainWindow(QWidget* parent = Q_NULL, Qt::WindowFlags flags = 0);
        ~RibbonMainWindow();

    public:
        RibbonBar* ribbonBar() const;
        void setRibbonBar(RibbonBar* ribbonBar);

    protected:
        void setFrameHelper(OfficeFrameHelper* helper);

    protected:
        virtual void paintEvent(QPaintEvent* event);

#ifdef Q_OS_WIN
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        virtual bool nativeEvent(const QByteArray& eventType, void *message, long *result);
#else
        virtual bool winEvent(MSG* message, long* result);
#endif
#endif // Q_OS_WIN

    private:
        friend class OfficeFrameHelperWin;
        QTN_DECLARE_PRIVATE(RibbonMainWindow)
        Q_DISABLE_COPY(RibbonMainWindow)
    };

} // namespace QtRibbon



#endif // QTN_RIBBONMAINWINDOW_H
