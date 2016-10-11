#include "precomp.h"
#include "jdbusnotify.h"
#include "jnotifier_p.h"
#include <QtDBus>

// class JDBusNotify

JDBusNotify::JDBusNotify(JNotifier &notifier, QObject *parent)
    : QObject(parent)
    , q_notifier(notifier)
{

}

bool JDBusNotify::isConnected()
{
#ifdef QT_DBUS_LIB
    return QDBusConnection::sessionBus().isConnected();
#else
    return false;
#endif
}

bool JDBusNotify::initialize(const std::string &service, const std::string &path,
                             const std::string &iface)
{
    bool result = true;

#ifdef QT_DBUS_LIB
    //
    if (!QDBusConnection::sessionBus().isConnected()) {
        qDebug() << "Cannot connect to the D-Bus session bus.\n"
                    "To start it, run:\n"
                    "\teval `dbus-launch --auto-syntax`\n";
        return false;   //
    }
    //
    QDBusConnectionInterface *connectionInterface = QDBusConnection::sessionBus().interface();
    if (!connectionInterface) {
        return false;   //
    }
    //
    q_service = QString::fromStdString(service);
    if (connectionInterface->isServiceRegistered(q_service)) {
        return false;    // registered
    } else {
        result = QDBusConnection::sessionBus().registerService(q_service);
        if (!result) {
            return false;   //
        }
    }
    //
    q_path = QString::fromStdString(path);
    q_interface = QString::fromStdString(iface);
    //
    result = QDBusConnection::sessionBus().registerObject(
                q_path, q_interface, this, QDBusConnection::ExportAllSignals | QDBusConnection::ExportAllSlots);
    if (!result) {
        qDebug() << QDBusConnection::sessionBus().lastError();
        return false;   //
    }
    result = QDBusConnection::sessionBus().connect(
                QString(),
                QString(),
                q_interface,
                "recvData",
                this,
                SLOT(recvData(bool,QString,QDBusVariant)));
    if (!result) {
        qDebug() << QDBusConnection::sessionBus().lastError();
        return false;   //
    }
#endif

    return result;
}

int JDBusNotify::sendBuffer(const std::string &domain, const char *buffer, int size)
{
    return sendData(true, QString::fromStdString(domain), QByteArray(buffer, size));
}

int JDBusNotify::sendString(const std::string &domain, const std::string &msg)
{
    return sendData(true, QString::fromStdString(domain), QString::fromStdString(msg));
}

int JDBusNotify::sendList(const std::string &domain, const std::list<std::string> &data)
{
    QList<QVariant> _data;
    std::list<std::string>::const_iterator citer = data.begin();
    for (; citer != data.end(); ++citer) {
        _data.append(QString::fromStdString(*citer));
    }
    return sendData(true, QString::fromStdString(domain), _data);
}

int JDBusNotify::sendVariant(const QString &domain, const QVariant &variant)
{
    return sendData(false, domain, variant);
}

void JDBusNotify::postBuffer(const std::string &domain, const char *buffer, int size)
{
    postData(true, QString::fromStdString(domain), QByteArray(buffer, size));
}

void JDBusNotify::postString(const std::string &domain, const std::string &msg)
{
    postData(true, QString::fromStdString(domain), QString::fromStdString(msg));
}

void JDBusNotify::postList(const std::string &domain, const std::list<std::string> &data)
{
    QList<QVariant> _data;
    std::list<std::string>::const_iterator citer = data.begin();
    for (; citer != data.end(); ++citer) {
        _data.append(QString::fromStdString(*citer));
    }
    postData(true, QString::fromStdString(domain), _data);
}

void JDBusNotify::postVariant(const QString &domain, const QVariant &variant)
{
    postData(false, domain, variant);
}

int JDBusNotify::recvData(bool customType, const QString &domain, const QDBusVariant &data)
{
    //
    QString subDomain = domain.section('@', 0, 1, QString::SectionSkipEmpty).trimmed();
    if (subDomain.endsWith('*')) {
        subDomain = subDomain.section('@', 0, 0, QString::SectionSkipEmpty).trimmed();
    }
    const std::string _domain = subDomain.toStdString();
    //
    JLRESULT result;
    const QVariant &variant = data.variant();

    //
    if (customType) {
        switch (variant.type()) {
        case QVariant::ByteArray:
        {
            const QByteArray arr = variant.toByteArray();
            result = q_notifier.sendMessage(_domain, (JWPARAM)arr.constData(), (JLPARAM)arr.size());
            break;
        }
        case QVariant::String:
        {
            const std::string _data = variant.toString().toStdString();
            result = q_notifier.sendMessage(_domain, (JWPARAM)&_data, 0);
            break;
        }
        case QVariant::List:
        {
            const QList<QVariant> lv = variant.toList();
            std::list<std::string> _data;
            QListIterator<QVariant> citer(lv);
            while (citer.hasNext()) {
                _data.push_back(citer.next().toString().toStdString());
            }
            result = q_notifier.sendMessage(_domain, (JWPARAM)&_data, 0);
            break;
        }
        default:
            result = q_notifier.sendMessage(_domain, (JWPARAM)&variant, 0);
            break;
        }
    } else {
        result = q_notifier.sendMessage(_domain, (JWPARAM)&variant, 0);
    }
    // reply
    return (int)result;
}

int JDBusNotify::sendData(bool customType, const QString &domain, const QVariant &data)
{
    // 获取...
    const QString service = domain.section('@', 2, 2, QString::SectionSkipEmpty).trimmed();
    if (service.isEmpty()) {
        return IDBusNotify::DomainFormatInvalid;        // 域名参数格式无效（[id]@[observerid]@[service]）
    }
    //
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    if (!sessionBus.isConnected()) {
        return IDBusNotify::SessionBusNotConnected;     // not connected
    }
    //
    QDBusConnectionInterface *iface = sessionBus.interface();
    if (!iface) {
        return IDBusNotify::ConnectionInterfaceInvalid; // invalid
    }
    //
    if (service == "*") {
        // broadcast
        QDBusReply<QStringList> services = iface->registeredServiceNames();
        QStringListIterator citer(services.value());
        while (citer.hasNext()) {
            QDBusMessage msg = QDBusMessage::createMethodCall(citer.next(), q_path, q_interface, "recvData");
            if (msg.type() == QDBusMessage::InvalidMessage) {
                continue;   // invalid
            }
            msg << customType << domain << QVariant::fromValue(QDBusVariant(data));
            //
            QDBusPendingCall penddingCall = sessionBus.asyncCall(msg/*, 5000*/);
            penddingCall.waitForFinished();     // wait 5s
            if (!penddingCall.isFinished()) {
                continue;   //
            }
            msg = penddingCall.reply();
            if (msg.type() == QDBusMessage::ReplyMessage) {
                QList<QVariant> arguments = msg.arguments();
                //
                if (arguments.isEmpty()) {
                    continue;
                } else if (arguments.first().toInt() == IDBusNotify::InterruptBroadCast) {
                    break;
                }
            }
        }
    } else {
        //
        QDBusMessage msg = QDBusMessage::createMethodCall(service, q_path, q_interface, "recvData");
       if (msg.type() == QDBusMessage::InvalidMessage) {
            return IDBusNotify::CallError;   // invalid
        }
        msg << customType << domain << QVariant::fromValue(QDBusVariant(data));
        QDBusPendingCall penddingCall = sessionBus.asyncCall(msg/*, 5000*/);
        penddingCall.waitForFinished();     // wait 5s
        msg = penddingCall.reply();
        if (msg.type() == QDBusMessage::ReplyMessage) {
            QList<QVariant> arguments = msg.arguments();
            //
            if (arguments.isEmpty()) {
                //
            } else {
                return arguments.first().toInt();
            }
        }
    }

    return IDBusNotify::CallSuccess;
}

void JDBusNotify::postData(bool customType, const QString &domain, const QVariant &data)
{
    // 获取
    const QString service = domain.section('@', 2, 2, QString::SectionSkipEmpty).trimmed();
    if (service.isEmpty()) {
        return;     // 域名参数格式无效（[id]@[observerid]@[service]）
    }
    //
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    if (!sessionBus.isConnected()) {
        return;     // not connected
    }
    //
    QDBusConnectionInterface *iface = sessionBus.interface();
    if (!iface) {
        return;     // invalid
    }
    //
    if (service == "*") {
        // broadcast
        QDBusMessage msg = QDBusMessage::createSignal(q_path, q_interface, "recvData");
        if (msg.type() == QDBusMessage::InvalidMessage) {
            return;   // invalid
        }
        msg << customType << domain << QVariant::fromValue(QDBusVariant(data));
        //
        bool result = sessionBus.send(msg);
        if (!result) {
            return;     //
        }
    } else {
        //
        QDBusMessage msg = QDBusMessage::createMethodCall(service, q_path, q_interface, "recvData");
        if (msg.type() == QDBusMessage::InvalidMessage) {
            return;     // invalid
        }
        msg << customType << domain << QVariant::fromValue(QDBusVariant(data));
        const QDBusPendingCall penddingCall = sessionBus.asyncCall(msg);
        //penddingCall.waitForFinished();     // wait 5s
        msg = penddingCall.reply();
        if (msg.type() == QDBusMessage::ReplyMessage) {
            return;     //
        }
    }
}
