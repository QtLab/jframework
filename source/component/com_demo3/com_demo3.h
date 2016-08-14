#ifndef COM_DEMO3_H
#define COM_DEMO3_H

#include "kernel/jframe_core.h"
#include "factory/jnotifier.h"

class QWidget;

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

    // IJComponentUi interface
public:
    void *createUi(void *parent, const char *windowName);

    // JObserver interface
public:
    std::string jobserverId() const;

protected:

private:
    IJAttempter *q_attempter;
    INotifier* q_notifier;
    QWidget *q_widget;
};

#endif // COM_DEMO3_H
