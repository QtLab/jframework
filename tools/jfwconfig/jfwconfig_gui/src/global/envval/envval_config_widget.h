#ifndef ENVVALCONFIGWIDGET_H
#define ENVVALCONFIGWIDGET_H

#include <QWidget>

class JFilterTableView;
class QRadioButton;
class QLineEdit;
class QComboBox;

class EnvvalConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EnvvalConfigWidget(QWidget *parent = 0);

    Q_INVOKABLE bool init(const QString &path);

signals:

public slots:
    void onTablePathsCellChanged(int row);

private:
    QString q_appDir;
    JFilterTableView *q_tablePaths;
    QWidget *q_widgetDetail;
    QLineEdit *q_editBase;
    QComboBox *q_comboBoxType;
    JFilterTableView *q_tableItems;
};

#endif // ENVVALCONFIGWIDGET_H
