#ifndef NOTIFY_MANAGER_H
#define NOTIFY_MANAGER_H

#include <QWidget>
#include "kernel/jnotifier.h"

// class NotifyManager

class JFrameLayout;

class NotifyManager : public JObserver
{
public:
    explicit NotifyManager(JFrameLayout *frameLayout);
    ~NotifyManager();

    bool init();

    // JObserver interface
public:
    std::string jobserverId() const;

protected:
    // jframe
    JLRESULT onTryExitFrame(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onExitFrame(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onRestartFrame(const std::string &id, JWPARAM wParam, JLPARAM lParam);

private:
    JFrameLayout *q_frameLayout;
    INotifier *q_notifier;
};

#endif // NOTIFY_MANAGER_H
