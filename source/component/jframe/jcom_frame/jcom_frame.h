#ifndef JCOM_FRAME_H
#deifne JCOM_FRAME_H

#include "IGF_Kernel.h"

class JComFrame : public IGF_Component
{
public:
    explicit JComFrame(IGF_Attempter *gAttempter);
    ~JComFrame();

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

private:
    IGF_Attempter *q_gAttempter;
};

#endif // JCOM_FRAME_H
