#ifndef COM_DEMO5_H
#define COM_DEMO5_H

#include "kernel/jframe_core.h"
#include "factory/jnotifier.h"

class CMfcDemo1Container;

class ComDemo5 :
        public IJComponent,
        public IJComponentUi,
        public JObserver
{
public:
    explicit ComDemo5(IJAttempter* attemper);
    virtual ~ComDemo5();

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
	std::string componentType() const;
    void attach();
    void detach();

    // IJComponentUi interface
public:
    void *createWindow(void *parent, const std::string &objectName);

    // JObserver interface
public:
    std::string observerId() const;

protected:

private:
    IJAttempter *q_attempter;
    CMfcDemo1Container *q_widget;
};

#endif // COM_DEMO5_H
