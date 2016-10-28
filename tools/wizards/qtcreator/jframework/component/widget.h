#ifndef %{UiHeaderGuard}
#define %{UiHeaderGuard}

%{JS: QtSupport.qtIncludes([ ( '%{IncludeQWidget}' )          ? 'QtGui/%{IncludeQWidget}'                  : '',
                             ( '%{IncludeQDialog}' )          ? 'QtGui/%{IncludeQDialog}'                  : '',
                             ( '%{IncludeQMainWindow}' )      ? 'QtGui/%{IncludeQMainWindow}'              : '',
                             ( '%{IncludeQSharedData}' )      ? 'QtCore/QSharedDataPointer'                : '' ],
                           [ ( '%{IncludeQWidget}' )          ? 'QtWidgets/%{IncludeQWidget}'              : '',
                             ( '%{IncludeQDialog}' )          ? 'QtWidgets/%{IncludeQDialog}'              : '',
                             ( '%{IncludeQMainWindow}' )      ? 'QtWidgets/%{IncludeQMainWindow}'          : '',
                             ( '%{IncludeQSharedData}' )      ? 'QtCore/QSharedDataPointer'                : '' ])}\
%{JS: Cpp.openNamespaces('%{UiClass}')}
@if '%{IncludeQSharedData}'
class %{UiClassName}Data;
@endif
class IJAttempter;

@if '%{UiClassBase}'
class %{UiClassName} : public %{UiClassBase}
@else
class %{UiClassName}
@endif
{
@if %{isQObject}
    Q_OBJECT
@endif
public:
@if '%{UiClassBase}' === 'QObject'
    explicit %{UiClassName}(IJAttempter &attempter, QObject *parent = 0);
@elsif '%{UiClassBase}' === "QWidget" || '%{UiClassBase}' === 'QDialog' || '%{UiClassBase}' === 'QMainWindow'
    explicit %{UiClassName}(IJAttempter &attempter, QWidget *parent = 0);
@else
    %{UiClassName}(IJAttempter &attempter);
@endif
@if '%{IncludeQSharedData}'
    %{UiClassName}(const %{UiClassName} &);
    %{UiClassName} &operator=(const %{UiClassName} &);
@endif
    ~%{UiClassName}();

@if %{isQObject}
Q_SIGNALS:

public Q_SLOTS:

@endif
private:
@if '%{IncludeQSharedData}'
    QSharedDataPointer<%{UiClassName}Data> data;
@else
    IJAttempter &d_attempter;
@endif
};
%{JS: Cpp.closeNamespaces('%{UiClass}')}
#endif // %{UiHeaderGuard}\
