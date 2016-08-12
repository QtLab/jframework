
#ifndef QTN_COMMONSTYLEPRIVATE_H
#define QTN_COMMONSTYLEPRIVATE_H

#include <QTime>
#include <QEvent>
#include <QWidget>
#include <QColor>

class QPainter;
class QSettings;
class QStyleOption;


namespace QtRibbon
{
    /* StyleHelper */
    class StyleHelper : public QObject
    {
    public:
        StyleHelper(QObject* pParent);
        virtual ~StyleHelper();

    public:
        void openIniFile(const QString& fileName);

    public:
        QString getValue(const QString& prefix, const QString& key) const;
        QColor getColor(const QString& prefix, const QString& key, QColor color = QColor()) const;
        int getInteger(const QString& prefix, const QString& key, int val) const;

    public:
        static QPixmap alphaBlend(const QPixmap& src);

    protected:
        QSettings* m_pSettings;
    };

    /* QtitanAnimation */ 
    class QtitanAnimation : public QObject
    {
    public :
        QtitanAnimation() : _widget(0), _running(true){}
        virtual ~QtitanAnimation();
    public :
        QWidget* widget() const { return _widget; }
        bool running() const { return _running; }
        const QTime& startTime() const { return _startTime; }
        void setRunning(bool val) { _running = val; }
        void setWidget(QWidget* widget);
        void setStartTime(const QTime& startTime) { _startTime = startTime; }
    public :
        virtual void paint(QPainter* painter, const QStyleOption* option);

    protected:
        void drawBlendedImage(QPainter* painter, QRect rect, float value);
        virtual bool eventFilter(QObject* obj, QEvent* event);

    protected:
        QTime _startTime;
        QWidget* _widget;
        QImage _primaryImage;
        QImage _secondaryImage;
        QImage _tempImage;
        bool _running;
    };

    /* QtitanTransition */
    class QtitanTransition : public QtitanAnimation
    {
    public :
        QtitanTransition() : QtitanAnimation() {}
        virtual ~QtitanTransition() {}
    public :
        void setDuration(int duration) { _duration = duration; }
        void setStartImage(const QImage& image) { _primaryImage = image; }
        void setEndImage(const QImage& image) { _secondaryImage = image; }
        virtual void paint(QPainter* painter, const QStyleOption* option);
        int duration() const { return _duration; }
    public :
        int _duration;
    };
} // namespace QtRibbon

#endif // QTN_COMMONSTYLEPRIVATE_H
