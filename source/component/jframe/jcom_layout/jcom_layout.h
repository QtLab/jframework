#ifndef JCOM_LAYOUT_H
#define JCOM_LAYOUT_H

#include "jframe_kernel.h"

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

private:
    IJAttempter *q_attempter;
    FrameFilter *q_frameFilter;
    INotifier *q_notifier;
};

#endif // JCOM_LAYOUT_H
