#ifndef JCOM_LAYOUT_H
#define JCOM_LAYOUT_H

#include "IGF_Kernel.h"
#include "jframe_factory.h"
#include "kernel/jnotifier.h"

class FrameFilter;

class JComLayout :
        public IGF_Component,
        public IGF_ComponentUI,
        public JObserver
{
public:
    explicit JComLayout(IGF_Attempter *gAttempter);
    ~JComLayout();

    // IUnknownEx interface
public:
    void Release();
    void *QueryInterface(const char *IID, unsigned int dwQueryVer);

    // IGF_Component interface
public:
    void Initialization();
    void Shutdown();
    const char *GetComponentID();
    const char *GetComponentName();

    // IGF_ComponentUI interface
public:
    void *CreateUI(void *parent, const char *windowName);

    // JObserver interface
public:
    std::string jobserverId() const;

private:
    IGF_Attempter *q_gAttempter;
    FrameFilter q_frameFilter;
    INotifier *q_notifier;
};

#endif // JCOM_LAYOUT_H
