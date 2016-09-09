#ifndef JCOM_LAYOUT_H
#define JCOM_LAYOUT_H

#include "kernel/jframe_core.h"
#include "factory/jnotifier.h"

class FrameFilter;

class JComLayout :
        public IJComponent,
        public IJComponentUi,
        public JObserver
{
public:
    explicit JComLayout(IJAttempter *attempter);
    ~JComLayout();

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

private:
    IJAttempter *q_attempter;
    FrameFilter *q_frameFilter;
};

#endif // JCOM_LAYOUT_H
