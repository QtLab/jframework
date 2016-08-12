#include "precomp.h"
#include "notify_manager.h"
#include "../jframe_layout_p.h"
#include "IGF_Kernel.h"

// class NotifyManager

NotifyManager::NotifyManager(JFrameLayout *frameLayout) :
    q_frameLayout(frameLayout),
    q_notifier(frameLayout->notifier())
{

}

NotifyManager::~NotifyManager()
{
    // ȡ������
    q_notifier->pop(this);
}

bool NotifyManager::init()
{
    // ������Ϣ
    q_notifier->begin(this)
            .push("j_frame_try_exit", &NotifyManager::onTryExitFrame)
            .push("j_frame_exit", &NotifyManager::onExitFrame)
            .push("j_frame_restart", &NotifyManager::onRestartFrame)
            .end();

    return true;
}

std::string NotifyManager::jobserverId() const
{
    return "jlayout.notify_manager";
}

JLRESULT NotifyManager::onTryExitFrame(const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(id);
    Q_UNUSED(wParam);
    Q_UNUSED(lParam);

    // �����˳�������첽��ʽ��
    jframeFacade()->invoke("frame_try_exit");

    return 0;
}

JLRESULT NotifyManager::onExitFrame(const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(id);
    Q_UNUSED(wParam);
    Q_UNUSED(lParam);

    // �˳������ͬ����ʽ��
    jframeFacade()->invoke("frame_exit");

    return 0;
}

JLRESULT NotifyManager::onRestartFrame(const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(id);
    Q_UNUSED(wParam);
    Q_UNUSED(lParam);

    // ���������ͬ����ʽ��
    jframeFacade()->invoke("frame_restart");

    return 0;
}
