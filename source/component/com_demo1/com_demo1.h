#ifndef COM_DEMO1_H
#define COM_DEMO1_H

#include "kernel/jframe_core.h"
#include "factory/jnotifier.h"

class QWidget;
class TestWidget1;

class ComDemo1 :
        public IJComponent,
        public IJComponentUi,
        public JObserver
{
public:
    explicit ComDemo1(IJAttempter* attemper);
    virtual ~ComDemo1();

    // IJUnknown interface
public:
    bool loadInterface();
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
    INotifier* q_notifier;
    TestWidget1 *q_testWidget1;
};

#endif // COM_DEMO1_H
