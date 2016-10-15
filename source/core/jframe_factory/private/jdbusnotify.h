#ifndef JDBUSNOTIFY_H
#define JDBUSNOTIFY_H

#include <QObject>
#include "../jnotifier.h"

class JNotifier;
class QDBusVariant;

/**
 * @brief IDBusNotify 实现
 */
class JDBusNotify : public QObject, public IDBusNotify
{
    Q_OBJECT
public:
    explicit JDBusNotify(JNotifier &notifier, QObject* parent = 0);
    ~JDBusNotify();

    void shutdown();

    // IDBusNotify interface
public:
    bool isConnected();
    bool initialize(const std::string &service, const std::string &path, const std::string &iface);

    int sendBuffer(const std::string &domain, const char *buffer, int size);
    int sendString(const std::string &domain, const std::string &msg);
    int sendList(const std::string &domain, const std::list<std::string> &data);
    int sendVariant(const QString &domain, const QVariant &variant);

    void postBuffer(const std::string &domain, const char *buffer, int size);
    void postString(const std::string &domain, const std::string &msg);
    void postList(const std::string &domain, const std::list<std::string> &data);
    void postVariant(const QString &domain, const QVariant &variant);

public slots:
    int recvData(bool customType, const QString &domain, const QDBusVariant &data);

private:
    int sendData(bool customType, const QString &domain, const QVariant &data);
    void postData(bool customType, const QString &domain, const QVariant &data);

private:
    JNotifier &q_notifier;
    QString q_service;
    QString q_path;
    QString q_interface;
};

#endif // JDBUSNOTIFY_H
