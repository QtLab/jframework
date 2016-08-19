#include "precomp.h"
#include "jlogmanager_p.h"
#include <sstream>
#if defined(__unix__)
#include <unistd.h>
#endif
//
#include "log4cpp/Category.hh"
#include "log4cpp/PropertyConfigurator.hh"
#include "log4cpp/convenience.h"

// struct JLogManagerData

struct JLogManagerData
{
    std::string filePath;                   //
    log4cpp::Category &categoryRoot;        //
    log4cpp::Category &categoryAll;         //
    log4cpp::Category &categoryConsole;     //
    log4cpp::Category &categoryFile;        //
    log4cpp::Category &categoryRollingFile; //

    JLogManagerData()
        : filePath("")
        , categoryRoot(log4cpp::Category::getRoot())
        , categoryAll(log4cpp::Category::getInstance("jframe_all"))
    #ifdef _MSC_VER
        , categoryConsole(log4cpp::Category::getInstance("jframe_console_win32"))
    #elif defined(__unix__)
        , categoryConsole(log4cpp::Category::getInstance("jframe_console_unix"))
    #else
    #pragma message("Not supported!")
    #endif
        , categoryFile(log4cpp::Category::getInstance("jframe_file"))
        , categoryRollingFile(log4cpp::Category::getInstance("jframe_rollingFile"))
    {

    }
};

// class JLogManagerPri

JLogManagerPri::JLogManagerPri()
{
    data = new JLogManagerData;
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
    data->categoryAll.shutdown();
    data->categoryConsole.shutdown();
    data->categoryFile.shutdown();
    data->categoryRollingFile.shutdown();
}

void JLogManagerPri::logging(MsgType type, const std::string &msg, const std::string &where, int argc, ...)
{
    va_list ap;
    va_start(ap, argc);

    loggingWhere(type, where, formatMessage(msg, argc, ap));

    va_end(ap);
}

std::string JLogManagerPri::config() const
{
    return data->filePath;
}

void JLogManagerPri::setConfig(const std::string &filePath)
{
    if (filePath == data->filePath) {
        return;     // the same
    }

    //
    data->filePath = filePath;

    // 加载日志配置文件
    try {
        log4cpp::PropertyConfigurator::configure(filePath);
    } catch (log4cpp::ConfigureFailure cf) {
#if defined(__unix__)
        if (QString::fromLatin1(cf.what()).startsWith(
                    "Appender 'console_win32' has unknown type 'Win32DebugAppender'")) {
            //
        } else
#endif
        {
            //
            Q_ASSERT_X(false, "Warning",
                       QStringLiteral("加载日志配置文件失败！[配置文件：%1] [错误原因：%2]")
                       .arg(filePath.c_str())
                       .arg(cf.what())
                       .toUtf8().data());
            return;     // 设置失败
        }
    }
}

void JLogManagerPri::loggingWhere(MsgType type, const std::string &where, const std::string &msg)
{
    //
    log4cpp::Category *category = 0;
    if (where == "all") {
        category = &data->categoryAll;
    } else if (where == "console") {
        category = &data->categoryConsole;
    } else if (where == "file") {
        category = &data->categoryFile;
    } else {
        category = &data->categoryConsole;
    }

    //
    switch (type) {
    case IJLogManager::EmergeMsg: LOG4CPP_EMERG((*category), msg); break;
    case IJLogManager::FatalMsg: LOG4CPP_FATAL((*category), msg); break;
    case IJLogManager::AlertMsg: LOG4CPP_ALERT((*category), msg); break;
    case IJLogManager::CriticalMsg: LOG4CPP_CRIT((*category), msg); break;
    case IJLogManager::ErrorMsg: LOG4CPP_ERROR((*category), msg); break;
    case IJLogManager::WarningMsg: LOG4CPP_WARN((*category), msg); break;
    case IJLogManager::NoticeMsg: LOG4CPP_NOTICE((*category), msg); break;
    case IJLogManager::DebugMsg: LOG4CPP_DEBUG((*category), msg); break;
    default:
        break;
    }
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
