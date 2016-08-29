#include "precomp.h"
#include "%{UiHdrFileName}"
%{JS: Cpp.openNamespaces('%{UiClass}')}
@if '%{IncludeQSharedData}'
class %{UiClassName}Data : public QSharedData
{
    friend class %{UiClassName};
public:
    explicit %{UiClassName}Data(IJAttempter &attempter)
        : attempter(attempter)
    {

    }

private:
    IJAttempter &attempter;
};

@endif
@if '%{UiClassBase}' === 'QObject'
%{UiClassName}::%{UiClassName}(IJAttempter &attempter, QObject *parent)
    : QObject(parent)
@if '%{IncludeQSharedData}'
    , data(new %{UiClassName}Data(attempter)
@else
    , q_attempter(attempter)
@endif
@elsif '%{UiClassBase}' === 'QWidget' || '%{UiClassBase}' === 'QDialog' || '%{UiClassBase}' === 'QMainWindow'
%{UiClassName}::%{UiClassName}(IJAttempter &attempter, QWidget *parent)
    : %{UiClassBase}(parent)
@if '%{IncludeQSharedData}'
    , data(new %{UiClassName}Data(attempter)
@else
    , q_attempter(attempter)
@endif
@else
%{UiClassName}::%{UiClassName}(IJAttempter &attempter)
@if '%{IncludeQSharedData}'
    : data(new %{UiClassName}Data(attempter)
@else
    : q_attempter(attempter)
@endif
@endif
{

}
@if '%{IncludeQSharedData}'

%{UiClassName}::%{UiClassName}(const %{UiClassName} &rhs) : data(rhs.data)
{

}

%{UiClassName} &%{UiClassName}::operator=(const %{UiClassName} &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }

    return *this;
}

%{UiClassName}::~%{UiClassName}()
{

}
@endif
%{JS: Cpp.closeNamespaces('%{UiClass}')}\
