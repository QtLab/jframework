#ifndef JCOM_LOGIN_H
#define JCOM_LOGIN_H

#include "IGF_Kernel.h"

class JComLogin : public IGF_Component
{
public:
    explicit JComLogin(IGF_Attempter *gAttempter);
    ~JComLogin();

    // IUnknownEx interface
public:
    void Release();
    void *QueryInterface(const char *IID, unsigned int dwQueryVer);

    // IGF_Component interface
public:
    void Initialization();
    void Shutdown();
    const char *GetComponentID() const;
    const char *GetComponentName() const;

private:
    IGF_Attempter *q_gAttempter;
};

#endif // JCOM_LOGIN_H
