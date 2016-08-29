#ifndef %{ComponentHeaderGuard}
#define %{ComponentHeaderGuard}

#include "kernel/jframe_core.h"
#include "factory/jnotifier.h"

@if '%{IncludeIJComponentUi}'
class %{UiClassName};

@endif
class %{ComponentClass}
        : public IJComponent
        @if '%{IncludeIJComponentUi}'
        , public %{IncludeIJComponentUi}
        @endif
        @if '%{IncludeIJCommandSink}'
        , public %{IncludeIJCommandSink}
        @endif
        @if '%{IncludeIJMessageSink}'
        , public %{IncludeIJMessageSink}
        @endif
        @if '%{IncludeJObserver}'
        , public %{IncludeJObserver}
        @endif
{
public:
    explicit %{ComponentClass}(IJAttempter* attemper);
    virtual ~%{ComponentClass}();

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
@if '%{IncludeDynamicLayout}'
    void attach();
    void detach();
@endif

@if '%{IncludeIJComponentUi}'
    // %{IncludeIJComponentUi} interface
public:
    void *createWindow(void *parent, const std::string &objectName);

@endif
@if '%{IncludeIJCommandSink}'
    // %{IncludeIJCommandSink} interface
public:
    bool commandSink(void *sender, const std::string &domain, const std::string &objectName,
                     const std::string &eventType, void *data);

@endif
@if '%{IncludeIJMessageSink}'
    // %{IncludeIJMessageSink} interface
public:
    bool messageSink(IJComponent *sender, const std::string &id, JWPARAM wParam, JLPARAM lParam);

@endif
@if '%{IncludeJObserver}'
    // %{IncludeJObserver} interface
public:
    std::string observerId() const;

@endif
protected:

private:
    IJAttempter *q_attempter;
@if '%{IncludeIJComponentUi}'
    TestWidget1 *q_ui;
@endif
};

#endif // %{ComponentHeaderGuard}
