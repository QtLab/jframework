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
    bool loadInterface();
    void releaseInterface();

    // IJlogManager interface
public:
    void logging(MsgType type, const std::string &msg, const std::string &where, int argc, ...);

public:
    std::string config() const;
    void setConfig(const std::string &filePath);

    void loggingWhere(MsgType type, const std::string &where, const std::string &msg);

private:
    static QString applicationDirPath();
    static std::string formatMessage(const std::string &msg, int argc, va_list ap);

private:
    JLogManagerData* data;
};

#endif // JLOGMANAGER_P_H
