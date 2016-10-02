#ifndef COMPONENTDETAIL_H
#define COMPONENTDETAIL_H

#include <QWidget>

class QLineEdit;
class QCheckBox;
class QButtonGroup;
class QRadioButton;
class QComboBox;
class QPushButton;

class ComponentDetail : public QWidget
{
    Q_OBJECT
public:
    explicit ComponentDetail(QWidget *parent = 0);

    void setComConfigPath(const QString &filePath);
    void clearContents();
    void focusComponent(const QString &componentName, bool def);

signals:
    void newComponentAdded(const QString &componentName, const QString &componentDesc, bool def);
    void componentModify(const QString &componentName, const QString &componentDesc, bool def);

public slots:
    void onButtonGroupComDirToggled(int id, bool checked);
    void onButtonSaveClicked();

    void newComponent();

private:
    QString q_comConfigPath;
    QString q_currComName;
    bool q_bNewCom;
    QLineEdit *q_editComponentName;
    QLineEdit *q_editComponentDesc;
    QCheckBox *q_checkBoxLoad;
    QCheckBox *q_checkBoxStay;
    QWidget *q_widgetComDir;
    QButtonGroup *q_buttonGroupComDir;
    QRadioButton *q_radioButtonFrame;
    QRadioButton *q_radioButtonThis;
    QRadioButton *q_radioButtonCustom;
    QLineEdit *q_editComDir;
    QComboBox *q_comboBoxType;
    QPushButton *q_buttonSave;
};

#endif // COMPONENTDETAIL_H
