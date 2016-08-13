#ifndef JCOM_LOGIN_H
#define JCOM_LOGIN_H

#include "jframe_core.h"

class JComLogin : public IJComponent
{
public:
    explicit JComLogin(IJAttempter *attempter);
    ~JComLogin();

    // IJUnknown interface
public:
    void releaseInterface();
    void *queryInterface(const char *iid, unsigned int ver);

    // IJComponent interface
public:
    std::string componentId() const;
    std::string componentDesc() const;
    bool initialize();
    void shutdown();
    void attach();
    void detach();

private:
    IJAttempter *q_attempter;
};

#endif // JCOM_LOGIN_H
