#ifndef FRAME_FILTER_H
#define FRAME_FILTER_H

#include <QObject>
#include <QRect>

// class FrameFilter

struct FrameFilterData;
class INotifier;
class IGF_Attempter;
class QWidget;

class FrameFilter : public QObject
{
public:
    explicit FrameFilter(INotifier *nofitier,
                         IGF_Attempter *gAttempter,
                         QObject *parent = 0);
    ~FrameFilter();

    bool init();

    QRect mainWindowRect() const;

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    QWidget *parseMainWindow(IGF_Attempter *gAttempter);
    bool loadConfig();

private:
    FrameFilterData *data;
};

#endif // FRAME_FILTER_H
