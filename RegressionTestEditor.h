/*	Author:		Jacob Howard
    Supervisor: Dr. Ryan Leduc

    Project created in conformity with the requirements for the Degree of Master of Engineering in Software Engineering,
    Computing and Software Department,
    McMaster University
    2017
*/

#ifndef REGRESSIONTESTEDITOR_H
#define REGRESSIONTESTEDITOR_H

#include <QMainWindow>
#include <QMap>
#include <QList>
#include <QTreeWidget>
#include "RegTestAlgorithmEditor.h"
#include "RegressionTestOutput.h"
#include "RegressionTestAlgorithms.h"
#include "MainForm.h"
#include "DesProject.h"
#include "DespotForm.h"

namespace Ui {
class RegressionTestEditor;
}

class RegressionTestEditor : public QMainWindow, public DESpot::DespotForm
{
    Q_OBJECT

public:
    RegressionTestEditor(DESpot::MainForm *pMainForm);
    RegressionTestEditor(DESpot::MainForm *pMainForm, RegressionTestEditor *regressionTestEditorOwner);
    ~RegressionTestEditor();
    int startTesting(QTreeWidget* treeWidget, int numEmptyChildren, RegressionTestOutput *output, QMap<std::wstring, std::wstring> &projMap, QList<QString> &repeatTests);
    void setCurFile(const QString &file);
    void updateWindowTitle(bool modified);
    void load(const QString & testName);

signals:
    void closingForm(bool&);

private slots:

    void handleDoubleClickEvent(QTreeWidgetItem*,int);
    void enableOptions();
    void getAlgorithms();
    void getSubsystems();
    void onAddProject();
    void onAddTest();
    void onRemoveProject();
    void onRemoveTest();
    void onAddAlgorithms();
    void onRemoveAlgorithm();
    void onClearProjectAlgorithms();
    void onClearAll();
    void onRunTest();
    void onSaveTest();
    void onSaveTestAs();
    void closeEditors(bool& okToClose);

private:
    Ui::RegressionTestEditor *ui;
    RegTestAlgorithmEditor* algorithmEditor;
    RegTestAlgorithmEditor* subsysEditor;
    RegressionTestOutput* regTestOutput;
    QString crtSubsysAlgo;
    QString curSaveFile;
    QString lastUsedDir;
    bool isModified;

    //Map with project name as key and file location as value
    //used to avoid having projects/tests added twice to regression test
    //and keep track of projects/tests to run
    QMap<std::wstring, std::wstring> m_projectMap;

    void repeatedFile(const DESpot::DesProject* project, const QString& projType);
    void repeatedTest(const QString& fileName);
    std::wstring multipleNames(std::wstring &name, int &count);
    void createActions();
    void setupMenu();
    void configureConnections();
    void resizeToFit();
    bool checkSave();
    bool repeatTestSaveCheck(const QString& file);
    void closeEvent(QCloseEvent *event);
    bool closeAttempt();
    void createOutputData(QTreeWidgetItem *currentItem, RegressionTestOutput *output, const QMap<std::wstring, std::wstring> &projMap);
    int getAlgorithmResults(DESpot::DesProject* project, const QString& algorithm, const QString& subsys);
    void addSubsystems(const QString&  crtAlgo, const QString& crtProj);
    QTreeWidgetItem* getSubsysAlgo();
    void populateAlgorithmEditor();
    void populateSubsystemEditor();

    void onOpenProject();

    bool isProject();
    bool isTest();
    std::wstring findTest(const QMap<std::wstring, std::wstring> &projMap);
    void addRoot(const QString& projName, const QString& projType);
    void addChild(QTreeWidgetItem *parent, const QString& algorithm, const QString& result);

    void disableOptions();

    void makeEditOnly();

    QMenu* m_fileMenu;
    QMenu* m_toolsMenu;
    QMenu* m_runMenu;
    QMenu* m_helpMenu;
    QAction* m_newRegTestAct;
    QAction* m_saveTestAct;
    QAction* m_saveTestAsAct;
    QAction* m_exitAct;
    QAction* m_addProjectAct;
    QAction* m_addTestAct;
    QAction* m_addAlgoAct;
    QAction* m_removeProjectAct;
    QAction* m_removeTestAct;
    QAction* m_removeAlgoAct;
    QAction* m_clearAlgosAct;
    QAction* m_clearAllAlgosAct;
    QAction* m_clearAllAct;
    QAction* m_runTestAct;
    QAction* m_openHelpFileAct;
    QAction* m_aboutDespotAct;
};

#endif // REGRESSIONTESTEDITOR_H
