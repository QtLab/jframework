#ifndef COMCONFIGWIDGET_H
#define COMCONFIGWIDGET_H

#include <QWidget>

class JFilterTableView;
class ComponentDetail;

class ComConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ComConfigWidget(QWidget *parent = 0);

signals:

public slots:
    bool appDirChanged(const QString &path);
    void onTableUsrCellPressed(int row);
    void onTableViewDefComCellFocus(int row);
    void onTableViewUsrComCellFocus(int row);
    void onButtonAddClicked();

    void onNewComponentAdded(const QString &componentName, const QString &componentDesc, bool def);
    void onComponentModify(const QString &componentName, const QString &componentDesc, bool def);
    void onTableUsrSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);

    void onActionDeleteRow();

private:
    QString componentConfigPath();

private:
    QString q_appDir;
    JFilterTableView *q_tableViewDefCom;
    JFilterTableView *q_tableViewUsrCom;
    ComponentDetail *q_componentDetail;
};

#endif // COMCONFIGWIDGET_H
