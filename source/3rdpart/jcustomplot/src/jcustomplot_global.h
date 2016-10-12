#ifndef JCUSTOMPLOT_GLOBAL_H
#define JCUSTOMPLOT_GLOBAL_H

#include <qglobal.h>
#include <QString>

// JCUSTOMPLOT_VERSION is (major << 16) + (minor << 8) + patch.

#define JCUSTOMPLOT_VERSION       0x000001
#define JCUSTOMPLOT_VERSION_STR   "0.0.1"

#ifdef JCUSTOMPLOT_LIB
#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__MINGW32__)
#   ifdef JCUSTOMPLOT_BUILD
#       define JCUSTOMPLOT_EXPORT  Q_DECL_EXPORT
#   else
#       define JCUSTOMPLOT_EXPORT  Q_DECL_IMPORT
#       if defined(_DEBUG) || defined(DEBUG)
#           pragma comment(lib, "jcustomplotd.lib")
#       else
#           pragma comment(lib, "jcustomplot.lib")
#       endif
#   endif // !JCUSTOMPLOT_BUILD
#endif // _MSC_VER || ...
#endif // JCUSTOMPLOT_LIB

#ifndef JCUSTOMPLOT_EXPORT
#define JCUSTOMPLOT_EXPORT
#endif

// - namespace JCUSTOMPLOT -
#define JCUSTOMPLOT_NAMESPACE JCUSTOMPLOT
#define JCUSTOMPLOT_BEGIN_NAMESPACE namespace JCUSTOMPLOT_NAMESPACE {
#define JCUSTOMPLOT_END_NAMESPACE }
#define JCUSTOMPLOT_USE_NAMESPACE using namespace JCUSTOMPLOT_NAMESPACE;

// - private pointer
#ifndef J_DECLARE_PRIVATE
#define J_DECLARE_PRIVATE(name) \
    name ## Private *d_ptr; \
    Q_DECLARE_PRIVATE(name)
#endif

#ifndef J_DECLARE_PUBLIC
#define J_DECLARE_PUBLIC(name) \
    name *q_ptr; \
    Q_DECLARE_PUBLIC(name)
#endif

#ifndef J_DECLARE_SINGLE_INSTANCE
#define J_DECLARE_SINGLE_INSTANCE(Class) \
public: \
    static Class *instance(); \
    static void releaseInstance(); \
private: \
    static Class *_instance;
#endif

#ifndef J_IMPLEMENT_SINGLE_INSTANCE
#define J_IMPLEMENT_SINGLE_INSTANCE(Class) \
Class *Class::_instance = 0; \
\
Class *Class::instance() { \
    if (Class::_instance == 0) { \
        Class::_instance = new Class; \
    } \
    return Class::_instance; \
} \
\
void Class::releaseInstance() { \
    if (Class::_instance != 0) { \
        delete Class::_instance; \
        Class::_instance = 0; \
    } \
}
#endif

// - class JCustomPlotCore -

class JCustomPlotCorePrivate;

class JCUSTOMPLOT_EXPORT JCustomPlotCore
{
public:
    static JCustomPlotCore *instance();
    static void releaseInstance();

    void init();

private:
    explicit JCustomPlotCore();
    ~JCustomPlotCore();

private:
    JCustomPlotCorePrivate *d;
};

#endif // JCUSTOMPLOT_GLOBAL_H
