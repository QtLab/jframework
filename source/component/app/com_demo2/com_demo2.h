#ifndef COM_DEMO2_H
#define COM_DEMO2_H

#include "kernel/jframe_core.h"
#include "factory/jnotifier.h"

class TestWidget1;

class ComDemo2 :
        public IJComponent,
        public IJComponentUi,
        public IJCommandSink,
        public IJMessageSink,
        public JObserver
{
public:
    explicit ComDemo2(IJAttempter* attemper);
    virtual ~ComDemo2();

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

    // IJMessageSink interface
public:
    bool messageSink(IJComponent *sender, const std::string &id, JWPARAM wParam, JLPARAM lParam);

    // IJCommandSink interface
public:
    bool commandSink(void *sender, const std::string &domain, const std::string &objectName,
                     const std::string &eventType, void *data);

    // JObserver interface
public:
    std::string observerId() const;

protected:
    JLRESULT onShowText(IJComponent *component, const std::string &id, JWPARAM wParam, JLPARAM lParam);

private:
    IJAttempter *q_attempter;
    TestWidget1 *q_testWidget1;
};

#endif // COM_DEMO2_H
