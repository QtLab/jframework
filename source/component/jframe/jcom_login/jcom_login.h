#ifndef JCOM_LOGIN_H
#define JCOM_LOGIN_H

#include "kernel/jframe_core.h"

class JComLogin : public IJComponent
{
public:
    explicit JComLogin(IJAttempter *attempter);
    ~JComLogin();

    // IJUnknown interface
public:
    bool loadInterface();
    bool updateInterface();
    void releaseInterface();
    void *queryInterface(const std::string &iid, unsigned int ver);

    // IJComponent interface
public:
    std::string componentName() const;
    std::string componentDesc() const;
    void attach();
    void detach();

private:
    IJAttempter *q_attempter;
};

#endif // JCOM_LOGIN_H
