#ifndef COM_DEMO1_H
#define COM_DEMO1_H

#include "kernel/jframe_core.h"
#include "factory/jnotifier.h"

class QWidget;
class TestWidget1;

class ComDemo1 :
        public IJComponent,
        public IJComponentUi,
        public IJCommandSink,
        public IJMessageSink,
        public JObserver
{
public:
    explicit ComDemo1(IJAttempter* attemper);
    virtual ~ComDemo1();

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

    // IJCommandSink interface
public:
    bool commandSink(void *sender, const std::string &domain, const std::string &objectName,
                     const std::string &eventType, void *data);

    // IJMessageSink interface
public:
    bool messageSink(IJComponent *sender, const std::string &id, JWPARAM wParam, JLPARAM lParam);

    // JObserver interface
public:
    std::string observerId() const;

protected:

private:
    IJAttempter *q_attempter;
    TestWidget1 *q_testWidget1;
};

#endif // COM_DEMO1_H
