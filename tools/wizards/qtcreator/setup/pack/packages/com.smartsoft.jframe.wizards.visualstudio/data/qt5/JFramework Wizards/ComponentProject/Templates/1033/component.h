#ifndef [!output COMPONENT_HEADER_GUARD]
#define [!output COMPONENT_HEADER_GUARD]

#include "kernel/jframe_core.h"
#include "factory/jnotifier.h"

[!if INCLUDE_IJCOMPONENTUI]
class [!output COMPONENT_UI_CLASS_NAME];

[!endif]
class [!output COMPONENT_CLASS_NAME]
    : public IJComponent
[!if INCLUDE_IJCOMPONENTUI]
    , public IJComponentUi
[!endif]
[!if INCLUDE_IJCOMMANDSINK]
    , public IJCommandSink
[!endif]
[!if INCLUDE_IJMESSAGESINK]
    , public IJMessageSink
[!endif]
[!if INCLUDE_JOBSERVER]
    , public JObserver
[!endif]
{
public:
    explicit [!output COMPONENT_CLASS_NAME](IJAttempter* attemper);
    virtual ~[!output COMPONENT_CLASS_NAME]();

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
[!if INCLUDE_DYNAMIC_LAYOUT]
    void attach();
    void detach();
[!endif]

[!if INCLUDE_IJCOMPONENTUI]
    // IJComponentUi interface
public:
    void *createWindow(void *parent, const std::string &objectName);

[!endif]
[!if INCLUDE_IJCOMMANDSINK]
    // IJCommandSink interface
public:
    bool commandSink(void *sender, const std::string &domain, const std::string &objectName,
                     const std::string &eventType, void *data);

[!endif]
[!if INCLUDE_IJMESSAGESINK]
    // IJMessageSink interface
public:
    bool messageSink(IJComponent *sender, const std::string &id, JWPARAM wParam, JLPARAM lParam);

[!endif]
[!if INCLUDE_JOBSERVER]
    // JObserver interface
public:
    std::string observerId() const;

[!endif]
protected:

private:
    IJAttempter *q_attempter;
[!if INCLUDE_IJCOMPONENTUI]
    [!output COMPONENT_UI_CLASS_NAME] *q_ui;
[!endif]
};

#endif // [!output COMPONENT_HEADER_GUARD]
