#ifndef COM_DEMO3_H
#define COM_DEMO3_H

#include "kernel/jframe_core.h"
#include "factory/jnotifier.h"

class QWidget;
class TestWidget1;

class ComDemo3 :
        public IJComponent,
        public IJComponentUi,
        public JObserver
{
public:
    explicit ComDemo3(IJAttempter* attemper);
    virtual ~ComDemo3();

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

    // IJComponentUi interface
public:
    void *createWindow(void *parent, const std::string &objectName);

    // JObserver interface
public:
    std::string observerId() const;

protected:

private:
    IJAttempter *q_attempter;
    TestWidget1 *q_testWidget1;
};

#endif // COM_DEMO3_H
