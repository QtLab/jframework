#ifndef JLOGMANAGER_P_H
#define JLOGMANAGER_P_H

#include "../jframe_factory.h"
#include "../jlogmanager.h"

// - class JLogManagerPri -

struct JLogManagerData;

class JLogManagerPri : public IJLogManager
{
public:
    explicit JLogManagerPri();
    ~JLogManagerPri();

    // IJUnknown interface
public:
    std::string interfaceIdentity() const;
    unsigned int interfaceVersion() const;
    void releaseInterface();

    // IJlogManager interface
public:
    void logging(MsgType type, const std::string &msg, int argc, ...);

public:
    LogType logType() const;
    void setLogType(LogType type);

    void emerge(const std::string &msg, int argc, va_list ap);
    void fatal(const std::string &msg, int argc, va_list ap);
    void alert(const std::string &msg, int argc, va_list ap);
    void crit(const std::string &msg, int argc, va_list ap);
    void error(const std::string &msg, int argc, va_list ap);
    void warn(const std::string &msg, int argc, va_list ap);
    void notice(const std::string &msg, int argc, va_list ap);
    void info(const std::string &msg, int argc, va_list ap);
    void debug(const std::string &msg, int argc, va_list ap);

private:
    void init();
    static std::string formatMessage(const std::string &msg, int argc, va_list ap);

private:
    JLogManagerData* data;
};

#endif // JLOGMANAGER_P_H
