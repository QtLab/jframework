#ifndef MAINWINDOWCONFIG_H
#define MAINWINDOWCONFIG_H

#include <QWidget>

class QScrollArea;
class QGroupBox;
class QComboBox;
class QLineEdit;
class QCheckBox;
class QRadioButton;

class MainWindowConfig : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindowConfig(QWidget *parent = 0);

    Q_INVOKABLE bool init(const QString &path);

signals:

public slots:
    void onToolBarTypeChanged(int index);
    void onButtonSaveClicked();

private:
    QString q_appDir;
    QScrollArea *q_scrollArea;
    QGroupBox *q_groupBoxGenerate;
    QComboBox *q_comboBoxWindowTheme;
    QLineEdit *q_editWindowIcon;
    QLineEdit *q_editWindowTitle;
    QCheckBox *q_checkBoxCloseKilled;
    QGroupBox *q_groupBoxSplash;
    QLineEdit *q_editImageStart;
    QLineEdit *q_editImageFinish;
    QCheckBox *q_checkBoxFullScreen;
    QRadioButton *q_radioButtonAlignLeft;
    QRadioButton *q_radioButtonAlignTop;
    QRadioButton *q_radioButtonAlignRight;
    QRadioButton *q_radioButtonAlignBottom;
    QRadioButton *q_radioButtonAlignHCenter;
    QRadioButton *q_radioButtonAlignVCenter;
    QGroupBox *q_groupBoxToolBar;
    QComboBox *q_comboBoxToolBarType;
    QGroupBox *q_groupBoxMenuBar;
    QGroupBox *q_groupBoxToolButton;
    QGroupBox *q_groupBoxRibbonBar;
    QCheckBox *q_checkBoxRibbonBarMinimized;
    QCheckBox *q_checkBoxRibbonBarVisible;
    QGroupBox *q_groupBoxLayout;
    QComboBox *q_comboBoxLayoutType;
};

#endif // MAINWINDOWCONFIG_H
