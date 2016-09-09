
#ifndef QTN_OFFICEFRAMEHELPER_H
#define QTN_OFFICEFRAMEHELPER_H

#include <QPixmap>

#include "QtRibbonDef.h"

class QRect;
class QIcon;

namespace QtRibbon
{
    class ContextHeader;
    class StyleOptionTitleBar;
    /* OfficeFrameHelper */

/*!
    \class QtRibbon::OfficeFrameHelper
    \internal
*/
    class OfficeFrameHelper
    {
    public:
        virtual ~OfficeFrameHelper() 
        {
        }
    public:
        virtual void enableOfficeFrame(QWidget* parent) = 0;

        virtual bool isDwmEnabled() const = 0;
        virtual bool isActive() const = 0;

        virtual int getFrameBorder() const = 0;
        virtual int titleBarSize() const = 0;
        virtual int tabBarHeight() const = 0;

        virtual void setHeaderRect(const QRect& rcHeader) = 0;
        virtual void setContextHeader(QList<ContextHeader*>* listContextHeaders) = 0;

        virtual QPixmap getFrameSmallIcon() const = 0;
        virtual QString getSystemMenuString(uint item) const = 0;

        virtual void drawTitleBar(QPainter* painter, const StyleOptionTitleBar& opt) = 0;
        virtual void fillSolidRect(QPainter* painter, const QRect& rect, const QRegion& airRegion, QColor clr) = 0;
        virtual void drawDwmCaptionText(QPainter* painter, const QRect& rect, const QString& strWindowText, bool active) = 0;
        virtual QSize getSizeSystemIcon() const = 0;
#ifdef Q_OS_WIN
        virtual HICON getWindowIcon(const QIcon& icon) const = 0;
#endif // Q_OS_WIN
        virtual void resetWindowIcon() = 0;

    #ifdef Q_OS_WIN
        virtual bool winEvent(MSG* message, long* result) = 0;
    #endif // Q_OS_WIN

    };

} // namespace QtRibbon


#endif // QTN_OFFICEFRAMEHELPER_H
