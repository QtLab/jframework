
#ifndef QTITAN_DEF_H
#define QTITAN_DEF_H

#include <QtCore/QProcess>

namespace QtRibbon
{
    #ifndef QTRIBBON_LIBRARY_STATIC
    #ifdef QTRIBBON_LIBRARY
    #define QTRIBBON_EXPORT Q_DECL_EXPORT
    #else
    #define QTRIBBON_EXPORT Q_DECL_IMPORT

#ifndef _QTRIBBON_HIDE_AUTOLINK_OUTPUT_
	#ifdef _DEBUG
    #pragma comment(lib, "qtribbond.lib")
	#else
    #pragma comment(lib, "qtribbon.lib")
	#endif
#endif  // _QTRIBBON_HIDE_AUTOLINK_OUTPUT_

    #endif
    #else
    #define QTRIBBON_EXPORT

#ifndef _QTRIBBON_HIDE_AUTOLINK_OUTPUT_
    #ifdef _DEBUG
    #pragma comment(lib, "qtribbond.lib")
	#else
    #ragma comment(lib, "qtribbon.lib")
	#endif
#endif  // _QTRIBBON_HIDE_AUTOLINK_OUTPUT_

    #endif

    #define Q_NULL    0
    #define Q_MAXINT INT_MAX
    #define Q_DELETE_AND_NULL(x) delete x; x = Q_NULL;

    #define QTN_DECLARE_PRIVATE(Class) friend class Class##Private; \
    Class##Private *qtn_d_ptr; \
    Class##Private& qtn_d() { return *qtn_d_ptr; } \
    const Class##Private& qtn_d() const { return *qtn_d_ptr; }

    #define QTN_DECLARE_PUBLIC(Class) \
    friend class Class; \
    Class *qtn_p_ptr; \
    inline void setPublic(Class *ptr) { qtn_p_ptr = ptr; } \
    Class& qtn_p() { return *qtn_p_ptr; } \
    const Class& qtn_p() const { return *qtn_p_ptr; }
    #define QTN_INIT_PRIVATE(Class) \
    qtn_d_ptr = new Class##Private(); qtn_d_ptr->setPublic(this);
    #define QTN_D(Class) Class##Private& d = qtn_d();
    #define QTN_P(Class) Class& p = qtn_p();
    #define QTN_FINI_PRIVATE() \
    delete qtn_d_ptr; qtn_d_ptr = Q_NULL;

    #define QTN_DECLARE_EX_PRIVATE(Class) friend class Class##Private; \
    Class##Private& qtn_d() { return reinterpret_cast<Class##Private &>(*qtn_d_ptr); } \
    const Class##Private& qtn_d() const { return reinterpret_cast<const Class##Private &>(*qtn_d_ptr); }

    #define QTN_DECLARE_EX_PUBLIC(Class) \
    friend class Class; \
    Class& qtn_p() { return static_cast<Class &>(*qtn_p_ptr); } \
    const Class& qtn_p() const { return static_cast<Class &>(*qtn_p_ptr); }

    /* BaseEventArgs */
    class QTRIBBON_EXPORT BaseEventArgs : public QObject
    {
    public:
        BaseEventArgs() : m_handled(false) {}
        bool isHandled() const { return m_handled; }
        void setHandled(bool handled) { m_handled = handled; }
    private:
        bool m_handled;
    };

    inline QString getSystemEnvironmentVariable(const QString& key)
    {
        QStringList all = QProcess::systemEnvironment();
        for (QStringList::const_iterator it = all.constBegin(); it != all.constEnd(); ++it)
        {
            QStringList pair = it->split("=");
            if (pair.size() != 2)
                continue;
            if (key.compare(pair[0], Qt::CaseInsensitive) == 0)
                return pair[1];
        }
        return QString();
    }

    inline QString getEnvironmentVariable(QProcess* process, const QString& key)
    {
        QStringList all = process->environment();
        for (QStringList::const_iterator it = all.constBegin(); it != all.constEnd(); ++it)
        {
            QStringList pair = it->split("=");
            if (pair.size() != 2)
                continue;
            if (key.compare(pair[0], Qt::CaseInsensitive) == 0)
                return pair[1];
        }
        return QString();
    }

    inline void setEnvironmentVariable(QProcess* process, const QString& key, const QString& value)
    {
        QStringList all = process->environment();
        QStringList::iterator it = all.begin();
        for (; it != all.end(); ++it)
        {
            QStringList pair = it->split("=");
            if (pair.size() != 2)
                continue;
            if (key.compare(pair[0], Qt::CaseInsensitive) == 0)
                break;
        }
        if (it != all.end())
            *it = QString("%1=%2").arg(key).arg(value);
        else
            all.push_back(QString("%1=%2").arg(key).arg(value));
        process->setEnvironment(all);
    }
} // namespace QtRibbon

#endif //QTITAN_DEF_H

