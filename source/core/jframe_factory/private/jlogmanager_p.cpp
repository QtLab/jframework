#include "precomp.h"
#include "jlogmanager_p.h"
#include <sstream>
#if defined(__unix__)
#include <unistd.h>
#endif

// log4cpp
#include "log4cpp/convenience.h"
#include "log4cpp/Category.hh"
#ifdef _MSC_VER
#include "log4cpp/Win32DebugAppender.hh"
#else
//
#endif
#include "log4cpp/StringQueueAppender.hh"
#include "log4cpp/RollingFileAppender.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"
#include "log4cpp/PatternLayout.hh"

// struct JLogManagerData

struct JLogManagerData
{
    QString fileName;
    IJLogManager::LogType logType;

#ifdef _MSC_VER
    log4cpp::Win32DebugAppender *appenderWin32;         // 输出到Output窗口的日志目的实例
#else
    //
#endif
    log4cpp::RollingFileAppender *appenderRollingFile;  // 输出到日志文件的日志目的实例
    log4cpp::Category &category;

    JLogManagerData()
        : logType(IJLogManager::LogFile)
    #ifdef _MSC_VER
        , appenderWin32(0)
    #else
        //
    #endif
        , appenderRollingFile(0)
        , category(log4cpp::Category::getInstance("jframe"))
    {

    }
};

// class JLogManagerPri

JLogManagerPri::JLogManagerPri()
{
    data = new JLogManagerData;

    //
    setLogType(data->logType);

    //
    init();
}

JLogManagerPri::~JLogManagerPri()
{
    delete data;
}

std::string JLogManagerPri::interfaceIdentity() const
{
    return IID_IJLogManager;
}

unsigned int JLogManagerPri::interfaceVersion() const
{
    return VER_IJLogManager;
}

bool JLogManagerPri::loadInterface()
{
    bool result = true;

    //

    return result;
}

void JLogManagerPri::releaseInterface()
{
    // shutdown log4cpp - category
    data->category.shutdown();
}

void JLogManagerPri::logging(MsgType type, const std::string &msg, int argc, ...)
{
    va_list ap;
    va_start(ap, argc);

    switch (type) {
    case IJLogManager::EmergeMsg: emerge(msg, argc, ap); break;
    case IJLogManager::FatalMsg: fatal(msg, argc, ap); break;
    case IJLogManager::AlertMsg: alert(msg, argc, ap); break;
    case IJLogManager::CriticalMsg: crit(msg, argc, ap); break;
    case IJLogManager::ErrorMsg: error(msg, argc, ap); break;
    case IJLogManager::WarningMsg: warn(msg, argc, ap); break;
    case IJLogManager::NoticeMsg: notice(msg, argc, ap); break;
    case IJLogManager::DebugMsg: debug(msg, argc, ap); break;
    default:
        break;
    }

    va_end(ap);
}

IJLogManager::LogType JLogManagerPri::logType() const
{
    return data->logType;
}

void JLogManagerPri::setLogType(LogType type)
{
    switch (type) {
    case IJLogManager::LogConsole:
        break;
    case IJLogManager::LogFile:
        break;
    default:
        break;
    }
}

void JLogManagerPri::emerge(const std::string &msg, int argc, va_list ap)
{
    LOG4CPP_EMERG(data->category, formatMessage(msg, argc, ap));
}

void JLogManagerPri::fatal(const std::string &msg, int argc, va_list ap)
{
    LOG4CPP_FATAL(data->category, formatMessage(msg, argc, ap));
}

void JLogManagerPri::alert(const std::string &msg, int argc, va_list ap)
{
    LOG4CPP_ALERT(data->category, formatMessage(msg, argc, ap));
}

void JLogManagerPri::crit(const std::string &msg, int argc, va_list ap)
{
    LOG4CPP_CRIT(data->category, formatMessage(msg, argc, ap));
}

void JLogManagerPri::error(const std::string &msg, int argc, va_list ap)
{
    LOG4CPP_ERROR(data->category, formatMessage(msg, argc, ap));
}

void JLogManagerPri::warn(const std::string &msg, int argc, va_list ap)
{
    LOG4CPP_WARN(data->category, formatMessage(msg, argc, ap));
}

void JLogManagerPri::notice(const std::string &msg, int argc, va_list ap)
{
    LOG4CPP_NOTICE(data->category, formatMessage(msg, argc, ap));
}

void JLogManagerPri::info(const std::string &msg, int argc, va_list ap)
{
    LOG4CPP_INFO(data->category, formatMessage(msg, argc, ap));
}

void JLogManagerPri::debug(const std::string &msg, int argc, va_list ap)
{
    LOG4CPP_DEBUG(data->category, formatMessage(msg, argc, ap));
}

bool JLogManagerPri::init()
{
    // create folder
    const QString filePath = applicationDirPath().append("/log/");
    QDir dir(filePath);
    if (!dir.exists()) {
        dir.mkpath(filePath);
    }

#ifdef _MSC_VER
    // appender - win32
    data->appenderWin32 = new log4cpp::Win32DebugAppender("jframe.appender-win32");
    log4cpp::PatternLayout *patternLayoutWin32 = new log4cpp::PatternLayout();
    patternLayoutWin32->setConversionPattern("%d | %p %c %x | %m%n");
    data->appenderWin32->setLayout(patternLayoutWin32);
#else
    //
#endif
    // appender - rolling-file
    data->appenderRollingFile = new log4cpp::RollingFileAppender(
                "jframe.appender-rolling-file",
                QString("%1/jframe_%2.log")
                .arg(filePath)
                .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss-zzz"))
                .toLocal8Bit().data());
    log4cpp::PatternLayout *patternLayoutRollingFile = new log4cpp::PatternLayout();
    patternLayoutRollingFile->setConversionPattern("%d | %p %c %x | %m%n");
    data->appenderRollingFile->setLayout(patternLayoutRollingFile);

    // category
    data->category.setPriority(log4cpp::Priority::INFO);
#ifdef _MSC_VER
    data->category.addAppender(data->appenderWin32);
#else
    //
#endif
    data->category.addAppender(data->appenderRollingFile);

    return true;
}

QString JLogManagerPri::applicationDirPath()
{
    static QString _path("");
    if (_path.isEmpty()) {
        // 获取软件实体绝对路径
#ifdef _MSC_VER
        extern QString qAppFileName();
        _path = QFileInfo(qAppFileName()).canonicalPath();
#elif defined(__unix__)
        // Try looking for a /proc/<pid>/exe symlink first which points to
        // the absolute path of the executable
        QFileInfo fileInfo(QString::fromLatin1("/proc/%1/exe").arg(getpid()));
        //printf("%s\n", fileInfo.canonicalPath().toLatin1().data());
        if (fileInfo.exists() && fileInfo.isSymLink()) {
            _path = fileInfo.canonicalPath();
        }
#else
#pragma message("Not supported!")
#endif
    }

    return _path;
}

std::string JLogManagerPri::formatMessage(const std::string &msg, int argc, va_list ap)
{
    std::ostringstream oss;

    switch (argc) {
    case 0: break;
    case 1:
    {
        // file
        const char* file = va_arg(ap, char*);
        if (file) {
            oss << file;
        } else {
            oss << "<??file>";
        }
        oss << " : ";
        break;
    }
    case 2:
    {
        // file
        const char* file = va_arg(ap, char*);
        if (file) {
            oss << file;
        } else {
            oss << "<??file>";
        }
        // line
        oss << " (" << va_arg(ap, int) << ") : ";
        break;
    }
    case 3:
    {
        // file
        const char* file = va_arg(ap, char*);
        if (file) {
            oss << file;
        } else {
            oss << "<??file>";
        }
        // line
        oss << " (" << va_arg(ap, int) << ") !";
        // func
        const char* func = va_arg(ap, char*);
        if (func) {
            oss << func;
        } else {
            oss << "<??func>";
        }
        oss << " : ";
        break;
    }
    default:
        break;
    }

    oss << msg;

    return oss.str();
}
