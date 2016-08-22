#ifndef NOTIFY_MANAGER_H
#define NOTIFY_MANAGER_H

#include "factory/jnotifier.h"

// class NotifyManager

class JFrameLayout;

class NotifyManager :
        public IJUnknown,
        public JObserver
{
public:
    explicit NotifyManager(JFrameLayout *frameLayout);
    ~NotifyManager();

    // IJUnknown interface
public:
    bool loadInterface();
    void releaseInterface();

    // JObserver interface
public:
    std::string observerId() const;

protected:
    // jframe
    JLRESULT onTryExitFrame(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onExitFrame(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onRestartFrame(const std::string &id, JWPARAM wParam, JLPARAM lParam);

private:
    JFrameLayout *q_frameLayout;
    INotifier &q_notifier;
};

#endif // NOTIFY_MANAGER_H
