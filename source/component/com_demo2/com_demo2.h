#ifndef COM_DEMO2_H
#define COM_DEMO2_H

#include "kernel/jframe_core.h"
#include "factory/jnotifier.h"

class QWidget;

class ComDemo2 :
        public IJComponent,
        public IJComponentUi,
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

    // JObserver interface
public:
    std::string observerId() const;

protected:

private:
    IJAttempter *q_attempter;
    INotifier* q_notifier;
    QWidget *q_widget;
};

#endif // COM_DEMO2_H
