#include "precomp.h"
#include "jwt_global.h"

// - class JwtCorePrivate -

class JwtCorePrivate
{
public:
    static QMutex _instance_mutex;
    static JwtCore *_instance;
    QList<QTranslator *> translators;
};

QMutex JwtCorePrivate::_instance_mutex;
JwtCore *JwtCorePrivate::_instance = 0;

// - class JwtCore -

JwtCore *JwtCore::instance()
{
    // 双重检测（防止多线程竞争）
    if (JwtCorePrivate::_instance == 0) {
        JwtCorePrivate::_instance_mutex.lock();
        if (JwtCorePrivate::_instance == 0) {
            JwtCore *tmpInstance = new JwtCore;
            JwtCorePrivate::_instance = tmpInstance;
        }
        JwtCorePrivate::_instance_mutex.unlock();
    }

    return JwtCorePrivate::_instance;
}

void JwtCore::releaseInstance()
{
    // 双重检测（防止多线程竞争）
    if (JwtCorePrivate::_instance != 0) {
        JwtCorePrivate::_instance_mutex.lock();
        if (JwtCorePrivate::_instance != 0) {
            delete JwtCorePrivate::_instance;
            JwtCorePrivate::_instance = 0;
        }
        JwtCorePrivate::_instance_mutex.unlock();
    }
}

void JwtCore::init()
{
    Q_INIT_RESOURCE(jwt_resource);
}

bool JwtCore::loadSystemLang(const QString &systemName)
{
    QString name = systemName.isEmpty() ? QLocale::system().name() : systemName;

    //
    d->translators.append(new QTranslator());
    bool result = d->translators.last()->load(
                ":/jwt/lang/qt_" + name + ".qm");
    if (result) {
        qApp->installTranslator(d->translators.last());
        return true;
    }

    return false;
}

JwtCore::JwtCore() :
    d(new JwtCorePrivate())
{
    init();
}

JwtCore::~JwtCore()
{
    Q_ASSERT(qApp);

    // remove translators
    Q_FOREACH (QTranslator *translator, d->translators) {
        qApp->removeTranslator(translator);
        delete translator;
    }

    Q_CLEANUP_RESOURCE(jwt_resource);

    delete d;
}

// - class JAutoCursor -


JAutoCursor::JAutoCursor(Qt::CursorShape shape)
{
    QApplication::setOverrideCursor(shape);
}

JAutoCursor::~JAutoCursor()
{
    QApplication::restoreOverrideCursor();
}
