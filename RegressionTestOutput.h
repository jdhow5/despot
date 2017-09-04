/*	Author:		Jacob Howard
    Supervisor: Dr. Ryan Leduc

    Project created in conformity with the requirements for the Degree of Master of Engineering in Software Engineering,
    Computing and Software Department,
    McMaster University
    2017
*/

#ifndef REGRESSIONTESTOUTPUT_H
#define REGRESSIONTESTOUTPUT_H

#include <QMainWindow>
#include <QTreeWidget>

namespace Ui {
class RegressionTestOutput;
}

class RegressionTestOutput : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegressionTestOutput(QWidget *parent = 0);
    ~RegressionTestOutput();

    QTreeWidgetItem* addRoot(const QString& fileName, const QString& projType);
    void addVoidChild(QTreeWidgetItem *parent, const QString& algorithm, const QString& expected, const QString& actual);
    QTreeWidgetItem* addChild(QTreeWidgetItem *parent, const QString& algorithm, const QString& expected, const QString& actual);
    void createFalseOutputData();
    QTreeWidgetItem* repeatProject(const QString& curProj, const QString& curProjType);
    QTreeWidgetItem* repeatSubsysAlgo(QTreeWidgetItem* curProj, const QString& subsysAlgo);
    bool repeatSubsys(QTreeWidgetItem* subsysAlgo, QTreeWidgetItem* subsys);
    bool repeatAlgo(QTreeWidgetItem* projItem, const QString &algoName);
    void getTestErrors();
    void outputIntegrityError(QTreeWidgetItem* integFailItem, QTreeWidgetItem* parent);

    QMultiMap<QString, QString> errMap;
    QMap<QString, QString> integErrMap;

private slots:
    void closeOutput(bool& okToClose);

private:
    Ui::RegressionTestOutput *ui;
};

#endif // REGRESSIONTESTOUTPUT_H
