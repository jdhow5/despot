/*	Author:		Jacob Howard
    Supervisor: Dr. Ryan Leduc

    Project created in conformity with the requirements for the Degree of Master of Engineering in Software Engineering,
    Computing and Software Department,
    McMaster University
    2017
*/

#include "RegressionTestOutput.h"
#include "ui_RegressionTestOutput.h"


RegressionTestOutput::RegressionTestOutput(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegressionTestOutput)
{
    ui->setupUi(this);

    ui->m_regTestOutputWidg->setColumnCount(3);
    ui->m_regTestOutputWidg->setHeaderLabels(QStringList() << "Name" << "Expected" << "Actual");
    ui->m_regTestFalseOutputWidg->setColumnCount(3);
    ui->m_regTestFalseOutputWidg->setHeaderLabels(QStringList() << "Name" << "Expected" << "Actual");
    ui->m_regTestOutputTabWidg->setCurrentIndex(0);
}

RegressionTestOutput::~RegressionTestOutput()
{
    delete ui;
}

//returns item to regression test editor so algorithms can be added to it
QTreeWidgetItem* RegressionTestOutput::addRoot(const QString& fileName, const QString& projType)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->m_regTestOutputWidg);
    item->setText(0, fileName);
    item->setText(1, projType);
    ui->m_regTestOutputWidg->addTopLevelItem(item);
    return item;
}

//returns item to regression test editor so subsystems can be added to it
QTreeWidgetItem* RegressionTestOutput::addChild(QTreeWidgetItem *parent, const QString& algorithm, const QString& expected, const QString& actual)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, algorithm);
    item->setText(1, expected);
    item->setText(2, actual);
    parent->addChild(item);
    return item;
}

//used to add subsystems to tree widget. Subsystems can't have child items
void RegressionTestOutput::addVoidChild(QTreeWidgetItem *parent, const QString& algorithm, const QString& expected, const QString& actual)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, algorithm);
    item->setText(1, expected);
    item->setText(2, actual);
    parent->addChild(item);
}

//slot connected to closingForm() signal from regression test editor
void RegressionTestOutput::closeOutput(bool& okToClose)
{
    okToClose = true;
    this->close();
}

//iterate through all tabs tree widget and add to unexpected tab if results differ
//track number of tests for summary
void RegressionTestOutput::createFalseOutputData()
{
    int numTests = 0;
    int numExTests = 0;
    int numUnexTests = 0;
    for(int i=0; i<ui->m_regTestOutputWidg->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *parentItem = new QTreeWidgetItem(ui->m_regTestFalseOutputWidg);
        parentItem->setText(0, ui->m_regTestOutputWidg->topLevelItem(i)->text(0));
        parentItem->setText(1, ui->m_regTestOutputWidg->topLevelItem(i)->text(1));
        ui->m_regTestFalseOutputWidg->addTopLevelItem(parentItem);

        for(int j=0; j<ui->m_regTestOutputWidg->topLevelItem(i)->childCount(); ++j)
        {
            QTreeWidgetItem *currentAlgo = ui->m_regTestOutputWidg->topLevelItem(i)->child(j);
            if(currentAlgo->text(0).contains("Subsystem"))
            {
                QTreeWidgetItem *childItem = addChild(parentItem, currentAlgo->text(0), currentAlgo->text(1), currentAlgo->text(2));

                for(int k=0; k<currentAlgo->childCount(); ++k)
                {
                    if(currentAlgo->child(k)->text(1) != currentAlgo->child(k)->text(2))
                    {
                        addVoidChild(childItem, currentAlgo->child(k)->text(0), currentAlgo->child(k)->text(1), currentAlgo->child(k)->text(2));
                        ++numUnexTests;
                    }
                    ++numTests;
                }
                if(childItem->childCount() == 0) delete childItem;
            }
            else
            {
                if(currentAlgo->text(1) != currentAlgo->text(2))
                {
                    addVoidChild(parentItem, currentAlgo->text(0), currentAlgo->text(1), currentAlgo->text(2));
                    ++numUnexTests;
                }
                ++numTests;
            }
            numExTests = numTests - numUnexTests;
            ui->m_numTestLabel->setText(QString::number(numTests));
            ui->m_expectedTestLabel->setText(QString::number(numExTests));
            ui->m_unexpectedTestLabel->setText(QString::number(numUnexTests));
        }
        if(parentItem->childCount() == 0) delete parentItem;
    }
}

//check all results tree widget for project.
//If found return it to editor so more algorithms can be added, otherwise add it to tree widget
QTreeWidgetItem* RegressionTestOutput::repeatProject(const QString& curProjName, const QString& curProjType)
{
    for(int i=0; i<ui->m_regTestOutputWidg->topLevelItemCount(); ++i)
    {
        if(curProjName == ui->m_regTestOutputWidg->topLevelItem(i)->text(0)) return ui->m_regTestOutputWidg->topLevelItem(i);
    }
    return addRoot(curProjName, curProjType);
}

//check all results tree widget for subsystem algorithm.
//If found return it to editor so more subsystems can be added, otherwise add it to tree widget
QTreeWidgetItem* RegressionTestOutput::repeatSubsysAlgo(QTreeWidgetItem *curProj, const QString& subsysAlgo)
{
    for(int i=0; i<curProj->childCount(); ++i)
    {
        if(curProj->child(i)->text(0) == subsysAlgo) return curProj->child(i);
    }
    return addChild(curProj, subsysAlgo, "", "");
}

//iterates through algorithm subsystems in all results tree widget
//if subsystem already exists on specific project and algorithm, don't test it
bool RegressionTestOutput::repeatSubsys(QTreeWidgetItem* subsysAlgo, QTreeWidgetItem* subsys)
{
    for(int i=0; i<subsysAlgo->childCount(); ++i)
    {
        if(subsysAlgo->child(i)->text(0) == subsys->text(0)) return true;
    }
    return false;
}

//iterates through project algorithms in all results tree widget
//if algorithm already tested on project, don't test it
bool RegressionTestOutput::repeatAlgo(QTreeWidgetItem* projItem, const QString &algoName)
{
    for(int i=0; i<projItem->childCount(); ++i)
    {
        if(projItem->child(i)->text(0) == algoName) return true;
    }
    return false;
}

//write errors to errors tab
void RegressionTestOutput::getTestErrors()
{
    int numErrors = 0;
    if(!integErrMap.isEmpty())
    {
        QString integErrStr = "The following project(s) failed the integrity check and could not be tested:";
        ui->m_errorWidg->addItem(integErrStr);
        QMap<QString, QString>::iterator integErrIter = integErrMap.begin();
        while(integErrIter != integErrMap.end())
        {
            QString integErr = "\t" + integErrIter.key() + " (" + integErrIter.value() + ")";
            ui->m_errorWidg->addItem(integErr);
            ++integErrIter;
            ++numErrors;
        }
    }

    if(!errMap.isEmpty())
    {
        QString errStr = "The following test(s) encountered an error and could not complete:";
        ui->m_errorWidg->addItem(errStr);
        QMultiMap<QString, QString>::iterator errIter = errMap.begin();
        while(errIter != errMap.end())
        {
            int i = std::distance(errMap.begin(), errIter);
            QString projErrs = "\t" + errIter.key() + " - " + errMap.values().at(i);
            ui->m_errorWidg->addItem(projErrs);
            ++errIter;
            ++numErrors;
        }
    }
    ui->m_errorLabel->setText(QString::number(numErrors));
}

//if project fails integrity check, all algorithms labelled with integrity error
void RegressionTestOutput::outputIntegrityError(QTreeWidgetItem *integFailItem, QTreeWidgetItem *parent)
{
    integErrMap.insert(integFailItem->text(0), integFailItem->text(1));
    for(int i=0; i<integFailItem->childCount(); ++i)
    {
        QString currentAlgo = integFailItem->child(i)->text(0);
        QString currentAlgoResult = integFailItem->child(i)->text(1);
        addVoidChild(parent, currentAlgo, currentAlgoResult, "Integrity Error");
    }
}
