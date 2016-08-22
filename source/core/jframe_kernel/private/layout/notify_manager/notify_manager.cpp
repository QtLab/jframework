#include "precomp.h"
#include "notify_manager.h"
#include "../jframe_layout_p.h"

// class NotifyManager

NotifyManager::NotifyManager(JFrameLayout *frameLayout) :
    q_frameLayout(frameLayout),
    q_notifier(frameLayout->notifier())
{

}

NotifyManager::~NotifyManager()
{
}

bool NotifyManager::loadInterface()
{
    // 订阅消息
    q_notifier.beginGroup(this)
            .append("j_frame_try_exit", &NotifyManager::onTryExitFrame)
            .append("j_frame_exit", &NotifyManager::onExitFrame)
            .append("j_frame_restart", &NotifyManager::onRestartFrame)
            .endGroup();

    return true;
}

void NotifyManager::releaseInterface()
{
    // 取消订阅
    q_notifier.remove(this);
}

std::string NotifyManager::observerId() const
{
    return "jlayout.notify_manager";
}

JLRESULT NotifyManager::onTryExitFrame(const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(id);
    Q_UNUSED(wParam);
    Q_UNUSED(lParam);

    // 尝试退出软件（异步方式）
    jframeFacade()->invokeMethod("frame_try_exit");

    return 0;
}

JLRESULT NotifyManager::onExitFrame(const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(id);
    Q_UNUSED(wParam);
    Q_UNUSED(lParam);

    // 退出软件（同步方式）
    jframeFacade()->invokeMethod("frame_exit");

    return 0;
}

JLRESULT NotifyManager::onRestartFrame(const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(id);
    Q_UNUSED(wParam);
    Q_UNUSED(lParam);

    // 重启软件（同步方式）
    jframeFacade()->invokeMethod("frame_restart");

    return 0;
}
