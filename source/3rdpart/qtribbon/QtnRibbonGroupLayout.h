
#ifndef QTN_RIBBONGROUPLAYOUT_H
#define QTN_RIBBONGROUPLAYOUT_H

#include <QLayout>

#include "QtRibbonDef.h"

class QAction;
namespace QtRibbon
{
    /* GroupLayout */
    class QTRIBBON_EXPORT GroupLayout : public QLayout
    {
        Q_OBJECT
    public:
        GroupLayout(QWidget* parent = Q_NULL);
        virtual ~GroupLayout();
    public:
        virtual void insertAction(int index, QAction* action) = 0;
        virtual int indexOf(QAction* action) const = 0;
        virtual int indexOf(QWidget* widget) const { return QLayout::indexOf(widget); }
    };

} // namespace QtRibbon

#endif // QTN_RIBBONGROUPLAYOUT_H
