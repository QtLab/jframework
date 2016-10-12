#include "precomp.h"
#include "jcustomplot_global.h"

// - class JCustomPlotCorePrivate -

class JCustomPlotCorePrivate
{
public:
    static QMutex _instance_mutex;
    static JCustomPlotCore *_instance;
    QList<QTranslator *> translators;
};

QMutex JCustomPlotCorePrivate::_instance_mutex;
JCustomPlotCore *JCustomPlotCorePrivate::_instance = 0;

// - class JCustomPlotCore -

JCustomPlotCore *JCustomPlotCore::instance()
{
    // 双重检测（防止多线程竞争）
    if (JCustomPlotCorePrivate::_instance == 0) {
        JCustomPlotCorePrivate::_instance_mutex.lock();
        if (JCustomPlotCorePrivate::_instance == 0) {
            JCustomPlotCore *tmpInstance = new JCustomPlotCore;
            JCustomPlotCorePrivate::_instance = tmpInstance;
        }
        JCustomPlotCorePrivate::_instance_mutex.unlock();
    }

    return JCustomPlotCorePrivate::_instance;
}

void JCustomPlotCore::releaseInstance()
{
    // 双重检测（防止多线程竞争）
    if (JCustomPlotCorePrivate::_instance != 0) {
        JCustomPlotCorePrivate::_instance_mutex.lock();
        if (JCustomPlotCorePrivate::_instance != 0) {
            JCustomPlotCorePrivate::_instance_mutex.unlock();
            delete JCustomPlotCorePrivate::_instance;
            JCustomPlotCorePrivate::_instance_mutex.lock();
            JCustomPlotCorePrivate::_instance = 0;
        }
        JCustomPlotCorePrivate::_instance_mutex.unlock();
    }
}

void JCustomPlotCore::init()
{
    Q_INIT_RESOURCE(jcustomplot_resource);
}

JCustomPlotCore::JCustomPlotCore() :
    d(new JCustomPlotCorePrivate())
{
    init();
}

JCustomPlotCore::~JCustomPlotCore()
{
    Q_ASSERT(qApp);

    // remove translators
    Q_FOREACH (QTranslator *translator, d->translators) {
        qApp->removeTranslator(translator);
        delete translator;
    }

    Q_CLEANUP_RESOURCE(jcustomplot_resource);

    delete d;
}
