#ifndef JWT_GLOBAL_H
#define JWT_GLOBAL_H

#include <qglobal.h>

// JWT_VERSION is (major << 16) + (minor << 8) + patch.

#define JWT_VERSION       0x000001
#define JWT_VERSION_STR   "0.0.1"

#ifdef _MSC_VER

// MSVC Compiler
// template-class specialization 'identifier' is already instantiated
#pragma warning(disable: 4660)
#pragma warning(disable: 4819)  // warning : 936

#ifdef JWT_DLL

#if defined(JWT_MAKEDLL)
#define JWT_EXPORT  Q_DECL_EXPORT
#else
#define JWT_EXPORT  Q_DECL_IMPORT

#if defined(_DEBUG) || defined(DEBUG)
#pragma comment(lib, "jwtd.lib")
#else
#pragma comment(lib, "jwt.lib")
#endif

#endif

#endif // JWT_DLL

#endif // _MSC_VER

#ifndef JWT_EXPORT
#define JWT_EXPORT
#endif

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

// - namespace JWT -
#define JWT_NAMESPACE JWT
#define JWT_BEGIN_NAMESPACE namespace JWT_NAMESPACE {
#define JWT_END_NAMESPACE }
#define JWT_USE_NAMESPACE using namespace JWT_NAMESPACE;

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

namespace JWT_NAMESPACE {
    static const double jDoubleEpsion = 1E-6;
    static const double jFDoubleEpsion = 1E-6f;

    // ==
    static inline bool jEqual(double a, double b)
    { return (a - b > -jDoubleEpsion || a - b < jDoubleEpsion) ? true : false; }
    static inline bool fEqual(float a, float b)
    { return (a - b > -jFDoubleEpsion || a - b < jFDoubleEpsion) ? true : false; }

    // >
    static inline bool fGreater(double a, double b)
    { return (a - b > jDoubleEpsion) ? true : false; }
    static inline bool fGreater(float a, float b)
    { return (a - b > jFDoubleEpsion) ? true : false; }

    // <
    static inline bool fLess(double a, double b)
    { return (a - b < -jDoubleEpsion) ? true : false; }
    static inline bool fLess(float a, float b)
    { return (a - b < -jFDoubleEpsion) ? true : false; }

    // >=
    static inline bool fGreaterOrEqual(double a, double b)
    { return ((a - b > jDoubleEpsion) ||  (a - b >= -jDoubleEpsion && a - b <= jDoubleEpsion)) ? true : false; }
    static inline bool fGreaterOrEqual(float a, float b)
    { return ((a - b > jFDoubleEpsion) ||  (a - b >= -jFDoubleEpsion && a - b <= jFDoubleEpsion)) ? true : false; }

    // <=
    static inline bool fLessOrEqual(double a, double b)
    { return ((a - b < -jDoubleEpsion) || (a - b >= -jDoubleEpsion && a - b <= jDoubleEpsion)) ? true : false; }
    static inline bool fLessOrEqual(float a, float b)
    { return ((a - b < -jFDoubleEpsion) || (a - b >= -jFDoubleEpsion && a - b <= jFDoubleEpsion)) ? true : false; }
}

// - class JwtCore -

class JwtCorePrivate;

class JWT_EXPORT JwtCore
{
public:
    static JwtCore *instance();
    static void releaseInstance();

    void init();

    bool loadSystemLang(const QString &systemName = QString());

private:
    explicit JwtCore();
    ~JwtCore();

private:
    JwtCorePrivate *d;
};

// - class JAutoCursor -

class JWT_EXPORT JAutoCursor
{
public:
    explicit JAutoCursor(Qt::CursorShape shape);
    ~JAutoCursor();
};

#endif // JWT_GLOBAL_H
