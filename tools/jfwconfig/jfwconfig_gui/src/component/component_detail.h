#ifndef COMPONENTDETAIL_H
#define COMPONENTDETAIL_H

#include <QWidget>

class QLineEdit;
class QCheckBox;
class QRadioButton;
class QComboBox;
class QPushButton;

class ComponentDetail : public QWidget
{
    Q_OBJECT
public:
    explicit ComponentDetail(QWidget *parent = 0);

    void clearContent();

signals:

public slots:
    void onButtonGroupComDirClicked(int id);
    void onButtonSaveClicked();

private:
    QLineEdit *q_editComponentName;
    QLineEdit *q_editComponentDesc;
    QCheckBox *q_checkBoxLoad;
    QCheckBox *q_checkBoxStay;
    QWidget *q_widgetComDir;
    QRadioButton *q_radioButtonFrame;
    QRadioButton *q_radioButtonThis;
    QRadioButton *q_radioButtonCustom;
    QLineEdit *q_editComDir;
    QComboBox *q_comboBoxType;
    QPushButton *q_buttonSave;
};

#endif // COMPONENTDETAIL_H
