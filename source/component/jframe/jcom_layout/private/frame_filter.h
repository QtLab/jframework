#ifndef FRAME_FILTER_H
#define FRAME_FILTER_H

#include <QObject>
#include <QRect>

// class FrameFilter

struct FrameFilterData;
class INotifier;
class IJAttempter;
class QWidget;

class FrameFilter : public QObject
{
public:
    explicit FrameFilter(INotifier *nofitier,
                         IJAttempter *attempter,
                         QObject *parent = 0);
    ~FrameFilter();

    bool init();

    QRect mainWindowRect() const;

    void attachEventFilter();

    void detachEventFilter();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    QWidget *parseMainWindow(IJAttempter *attempter);
    bool loadConfig();

private:
    FrameFilterData *data;
};

#endif // FRAME_FILTER_H
