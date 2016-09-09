#if defined(PRO_CORE)
#include "precomp.h"
#endif
#include <QStylePainter>
#include <QStyleOption>
#include <QMouseEvent>
#include <QColorDialog>

#include "QtnOfficePopupColorButton.h"

using namespace QtRibbon;

/*!
    \class QtRibbon::PopupColorButton
    \inmodule QtitanRibbon
    \brief PopupColorButton class implements a button for the color selection.
    \image ribbon_colorbutton.png
*/

/*!
\property QtRibbon::PopupColorButton::color
*/

/* PopupColorButton */
PopupColorButton::PopupColorButton(QWidget* parent)
    : QToolButton(parent)
{
    setPopupMode(MenuButtonPopup);
    m_color = Qt::color0;
}

PopupColorButton::~PopupColorButton()
{
}

/*!
\brief Returns the color that has been selected.
*/ 
QColor PopupColorButton::color() const
{
    return m_color;
}

/*!
\brief Sets the specified \a color and select specified color button.
*/ 
void PopupColorButton::setColor(const QColor& color)
{
    if (color != m_color)
    {
        m_color = color;
        update();
    }
}

/*! \reimp */
void PopupColorButton::paintEvent(QPaintEvent* painter)
{
    QToolButton::paintEvent(painter);

    QStylePainter p(this);

    QStyleOptionToolButton opt;
    initStyleOption(&opt);

    if (opt.icon.isNull())
        return;
    if (!m_color.isValid())
        return;
    QRect rect(opt.rect);
    QSize sizeIcon = opt.iconSize;

    int widthIndicator = style()->pixelMetric(QStyle::PM_MenuButtonIndicator, &opt, this);
    rect.setHeight(sizeIcon.height()/5);
    rect.adjust(2, 0, -(widthIndicator+4), 0);
    rect.translate(0, opt.rect.bottom()-(rect.height()+2));
    p.fillRect(rect, m_color);
}

/*! \reimp */
void PopupColorButton::mousePressEvent(QMouseEvent* e)
{
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    if (e->button() == Qt::LeftButton && (popupMode() == MenuButtonPopup))
    {
        QRect popupr = style()->subControlRect(QStyle::CC_ToolButton, &opt,
            QStyle::SC_ToolButtonMenu, this);
        if (popupr.isValid() && popupr.contains(e->pos())) 
        {
            QColor col = QColorDialog::getColor(m_color, this);
            if (!col.isValid())
                return;

            if (col != m_color)
            {
                m_color = col;
                update();
                emit colorChanged(m_color);
            }
            return;
        }
    }
    QToolButton::mousePressEvent(e);
}

