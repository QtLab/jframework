#ifndef CUSTOMPLOTTEST_H
#define CUSTOMPLOTTEST_H

#include "kernel/jframe_core.h"
#include "factory/jnotifier.h"

class CustomPlotTestUi;

class CustomPlotTest
        : public IJComponent
        , public IJComponentUi
        , public JObserver
{
public:
    explicit CustomPlotTest(IJAttempter* attemper);
    virtual ~CustomPlotTest();

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
    CustomPlotTestUi *q_ui;
};

#endif // CUSTOMPLOTTEST_H
