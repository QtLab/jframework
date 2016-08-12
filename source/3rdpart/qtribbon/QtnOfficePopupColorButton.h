
#ifndef QTN_POPUPCOLORBUTTON_H
#define QTN_POPUPCOLORBUTTON_H

#include <QToolButton>

#include "QtRibbonDef.h"


class QStyleOption;
namespace QtRibbon
{
    /* PopupColorButton */
    class QTRIBBON_EXPORT PopupColorButton : public QToolButton
    {
        Q_OBJECT
        Q_PROPERTY(QColor color READ color WRITE setColor)
    public:
        PopupColorButton(QWidget* parent = Q_NULL);
        virtual ~PopupColorButton();

    public:
        QColor color() const;
        void setColor(const QColor& color);

    Q_SIGNALS:
        void colorChanged(const QColor& color);

    protected:
        virtual void paintEvent     (QPaintEvent* event);
        virtual void mousePressEvent(QMouseEvent* event);

    protected:
        QColor m_color;
    };

} // namespace QtRibbon

#endif // QTN_POPUPCOLORBUTTON_H
