#ifndef COM_DEMO4_H
#define COM_DEMO4_H

#include "IGF_Kernel.h"
#include "jframe_facade.h"
#include "kernel/jnotifier.h"

class QWidget;

class ComDemo4 :
        public IGF_Component,
        public IGF_ComponentUI,
        public JObserver
{
public:
    explicit ComDemo4(IGF_Attempter* gAttemper);
    virtual ~ComDemo4();

    // IUnknownEx interface
public:
    void Release();
    void* QueryInterface(const char* IID, unsigned int dwQueryVer);

    // IGF_Component interface
public:
    void Initialization();
    void Shutdown();
    const char* GetComponentID() const;
    const char* GetComponentName() const;

    // IGF_ComponentUI interface
public:
    void* CreateUI(void* parent, const char* windowName);

    // JObserver interface
public:
    std::string jobserverId() const;

protected:

private:
    IGF_Attempter* q_gAttempter;
    INotifier* q_notifier;
    QWidget *q_widget;
};

#endif // COM_DEMO4_H
