#ifndef JICENOTIFY_H
#define JICENOTIFY_H

#include <QObject>
#include "../jnotifier.h"
#include "slice/IceNotify.h"

class JNotifier;

class JIceNotify : public QObject, public IIceNotify
{
    Q_OBJECT
public:
    explicit JIceNotify(JNotifier &notifier, QObject *parent = 0);
    ~JIceNotify();

    // IIceNotify interface
public:
    bool isConnected();
    bool initialize(const std::string &service);
    bool setCurrentService(const std::string &service);

    int sendBuffer(const std::string &domain, const char *buffer, int size);
    int sendString(const std::string &domain, const std::string &msg);
    int sendList(const std::string &domain, const std::list<std::string> &data);
    int sendVariant(const QString &domain, const QVariant &variant);

    void postBuffer(const std::string &domain, const char *buffer, int size);
    void postString(const std::string &domain, const std::string &msg);
    void postList(const std::string &domain, const std::list<std::string> &data);
    void postVariant(const QString &domain, const QVariant &variant);

signals:

public slots:

private:
    JNotifier &q_notifier;
    ::Notify::JIceNotify::ProxyType q_proxy;
    QString q_currentService;
};

#endif // JICENOTIFY_H
