/*	Author:		Jacob Howard
    Supervisor: Dr. Ryan Leduc

    Project created in conformity with the requirements for the Degree of Master of Engineering in Software Engineering,
    Computing and Software Department,
    McMaster University
    2017
*/

#include <QFileDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QCloseEvent>
#include "RegressionTestEditor.h"
#include "ui_RegressionTestEditor.h"
#include "DesHierProject.h"
#include "DesSubsystem.h"
#include "RegressionTestSerializer.h"

RegressionTestEditor::RegressionTestEditor(DESpot::MainForm *pMainForm) :
    DespotForm(pMainForm, this, pMainForm, eRegressionTestEditor),
    ui(new Ui::RegressionTestEditor)
{
    ui->setupUi(this);
    ui->m_regressionTestWidg->setColumnCount(2);
    ui->m_regressionTestWidg->setHeaderLabels(QStringList() << "Name" << "Result");
    createActions();
    setupMenu();
    configureConnections();
    updateWindowTitle(true);
    disableOptions();
    lastUsedDir = qApp->applicationDirPath();
    setAttribute(Qt::WA_DeleteOnClose);
}

RegressionTestEditor::RegressionTestEditor(DESpot::MainForm *pMainForm, RegressionTestEditor* regressionTestEditorOwner) :
    DespotForm(pMainForm, this, regressionTestEditorOwner, eRegressionTestEditor),
    ui(new Ui::RegressionTestEditor)
{
    ui->setupUi(this);
    ui->m_regressionTestWidg->setColumnCount(2);
    ui->m_regressionTestWidg->setHeaderLabels(QStringList() << "Name" << "Result");
    createActions();
    setupMenu();
    configureConnections();
    enableOptions();
    makeEditOnly();
    connect(regressionTestEditorOwner, SIGNAL(closingForm(bool&)), this, SLOT(closeEditors(bool&)));
    setAttribute(Qt::WA_DeleteOnClose);
}

RegressionTestEditor::~RegressionTestEditor()
{
    delete ui;
}

void RegressionTestEditor::createActions()
{
    m_newRegTestAct = new QAction(QIcon(":/resources/sim_new.png"), tr("&New Regression Test"), this);
    m_newRegTestAct->setShortcut(tr("Ctrl+N"));
    m_newRegTestAct->setToolTip(tr("New Regression Test"));
    m_newRegTestAct->setStatusTip(tr("Create a new regression test"));
    connect(m_newRegTestAct, SIGNAL(triggered()), m_pMainForm, SLOT(onNewRegressionTest()));

    m_saveTestAct = new QAction(QIcon(":/resources/saveProject.png"), tr("&Save"), this);
    m_saveTestAct->setShortcut(tr("Ctrl+S"));
    m_saveTestAct->setToolTip(tr("Save Regression Test"));
    m_saveTestAct->setStatusTip(tr("Save the current regression test"));
    connect(m_saveTestAct, SIGNAL(triggered()), this, SLOT(onSaveTest()));

    m_saveTestAsAct = new QAction(tr("&Save As..."), this);
    m_saveTestAsAct->setToolTip(tr("Save Regression Test"));
    m_saveTestAsAct->setStatusTip(tr("Save the current regression test"));
    connect(m_saveTestAsAct, SIGNAL(triggered()), this, SLOT(onSaveTestAs()));

    m_exitAct = new QAction(tr("E&xit"), this);
    m_exitAct->setStatusTip(tr("Exit the current regression test"));
    connect(m_exitAct, SIGNAL(triggered()), this, SLOT(close()));

    m_addProjectAct = new QAction(QIcon(":/resources/openProject.png"), tr("Add &Project"), this);
    m_addProjectAct->setShortcut(tr("Ctrl+O"));
    m_addProjectAct->setToolTip(tr("Add DES Project"));
    m_addProjectAct->setStatusTip(tr("Add a DES project to the current regression test"));
    connect(m_addProjectAct, SIGNAL(triggered()), this, SLOT(onAddProject()));

    m_addTestAct = new QAction(QIcon(":/resources/openDes.png"), tr("Add &Test"), this);
    m_addTestAct->setShortcut(tr("Ctrl+T"));
    m_addTestAct->setToolTip(tr("Add Existing Regression Test"));
    m_addTestAct->setStatusTip(tr("Add an existing test suite to the current regression test"));
    connect(m_addTestAct, SIGNAL(triggered()), this, SLOT(onAddTest()));

    m_addAlgoAct = new QAction(tr("Add &Algorithm"), this);
    m_addAlgoAct->setShortcut(tr("Ctrl+A"));
    m_addAlgoAct->setStatusTip(tr("Add an algorithm to the selected project"));
    connect(m_addAlgoAct, SIGNAL(triggered()), this, SLOT(onAddAlgorithms()));

    m_removeProjectAct = new QAction(tr("&Remove Project"), this);
    m_removeProjectAct->setShortcut(tr("Ctrl+Alt+O"));
    m_removeProjectAct->setStatusTip(tr("Remove the selected project from the regression test"));
    connect(m_removeProjectAct, SIGNAL(triggered()), this, SLOT(onRemoveProject()));

    m_removeTestAct = new QAction(tr("&Remove Test"), this);
    m_removeTestAct->setShortcut(tr("Ctrl+Alt+T"));
    m_removeTestAct->setStatusTip(tr("Remove the selected test suite from the regression test"));
    connect(m_removeTestAct, SIGNAL(triggered()), this, SLOT(onRemoveTest()));

    m_removeAlgoAct = new QAction(tr("&Remove Algorithm"), this);
    m_removeAlgoAct->setShortcut(tr("Ctrl+Alt+A"));
    m_removeAlgoAct->setStatusTip(tr("Remove the selected test from the regression test"));
    connect(m_removeAlgoAct, SIGNAL(triggered()), this, SLOT(onRemoveAlgorithm()));

    m_clearAlgosAct = new QAction(tr("&Clear Algorithms"), this);
    m_clearAlgosAct->setToolTip(tr("Clear Project's Algorithms"));
    m_clearAlgosAct->setStatusTip(tr("Clear all algorithms from the selected project"));
    connect(m_clearAlgosAct, SIGNAL(triggered()), this, SLOT(onClearProjectAlgorithms()));

    m_clearAllAct = new QAction(tr("&Clear All"), this);
    m_clearAllAct->setToolTip(tr("Clear Regression Test"));
    m_clearAllAct->setStatusTip(tr("Clear all projects, tests, and algorithms from regression test"));
    connect(m_clearAllAct, SIGNAL(triggered()), this, SLOT(onClearAll()));

    m_runTestAct = new QAction(QIcon(":/resources/sim_run.png"), tr("&Run Test"), this);
    m_runTestAct->setShortcut(tr("Ctrl+R"));
    m_runTestAct->setStatusTip(tr("Run the regression test"));
    connect(m_runTestAct, SIGNAL(triggered()), this, SLOT(onRunTest()));

    m_openHelpFileAct = new QAction(tr("&Help Contents"), this);
    m_openHelpFileAct->setStatusTip(tr("Open the application help file"));
    connect(m_openHelpFileAct, SIGNAL(triggered()), m_pMainForm, SLOT(onOpenHelpFile()));

    m_aboutDespotAct = new QAction(tr("&About"), this);
    m_aboutDespotAct->setStatusTip(tr("Display information about the DESpot application"));
    connect(m_aboutDespotAct, SIGNAL(triggered()), m_pMainForm, SLOT(onAboutDespot()));
}

void RegressionTestEditor::setupMenu()
{
    m_fileMenu = this->menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_newRegTestAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_saveTestAct);
    m_fileMenu->addAction(m_saveTestAsAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAct);

    m_toolsMenu = this->menuBar()->addMenu(tr("&Tools"));
    m_toolsMenu->addAction(m_addProjectAct);
    m_toolsMenu->addAction(m_addTestAct);
    m_toolsMenu->addAction(m_addAlgoAct);
    m_toolsMenu->addSeparator();
    m_toolsMenu->addAction(m_removeProjectAct);
    m_toolsMenu->addAction(m_removeTestAct);
    m_toolsMenu->addAction(m_removeAlgoAct);
    m_toolsMenu->addSeparator();
    m_toolsMenu->addAction(m_clearAlgosAct);
    m_toolsMenu->addAction(m_clearAllAct);

    m_runMenu = this->menuBar()->addMenu(tr("&Run"));
    m_runMenu->addAction(m_runTestAct);

    m_helpMenu = this->menuBar()->addMenu(tr("&Help"));
    m_helpMenu->addAction(m_openHelpFileAct);
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(m_aboutDespotAct);
}

void RegressionTestEditor::configureConnections()
{
    connect(ui->m_addRTProjectBtn, SIGNAL(clicked()), this, SLOT(onAddProject()));
    connect(ui->m_removeRTProjectBtn, SIGNAL(clicked()), this, SLOT(onRemoveProject()));
    connect(ui->m_addTestBtn, SIGNAL(clicked()), this, SLOT(onAddTest()));
    connect(ui->m_removeTestBtn, SIGNAL(clicked()), this, SLOT(onRemoveTest()));
    connect(ui->m_addRTAlgorithmsBtn, SIGNAL(clicked()), this, SLOT(onAddAlgorithms()));
    connect(ui->m_removeRTAlgorithmsBtn, SIGNAL(clicked()), this, SLOT(onRemoveAlgorithm()));
    connect(ui->m_runTestBtn, SIGNAL(clicked()), this, SLOT(onRunTest()));
    connect(ui->m_regressionTestWidg, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(handleDoubleClickEvent(QTreeWidgetItem*,int)));
}

//called when tree widget is empty
void RegressionTestEditor::disableOptions()
{
    ui->m_removeRTProjectBtn->setDisabled(true);
    ui->m_removeTestBtn->setDisabled(true);
    ui->m_addRTAlgorithmsBtn->setDisabled(true);
    ui->m_runTestBtn->setDisabled(true);
    ui->m_removeRTAlgorithmsBtn->setDisabled(true);
    m_addAlgoAct->setDisabled(true);
    m_removeAlgoAct->setDisabled(true);
    m_removeProjectAct->setDisabled(true);
    m_removeTestAct->setDisabled(true);
}

//called when item is added to tree widget
void RegressionTestEditor::enableOptions()
{
    ui->m_removeRTProjectBtn->setEnabled(true);
    ui->m_removeTestBtn->setEnabled(true);
    ui->m_addRTAlgorithmsBtn->setEnabled(true);
    ui->m_removeRTAlgorithmsBtn->setEnabled(true);
    ui->m_runTestBtn->setEnabled(true);
    m_addAlgoAct->setEnabled(true);
    m_removeAlgoAct->setEnabled(true);
    m_removeProjectAct->setEnabled(true);
    m_removeTestAct->setEnabled(true);
}

void RegressionTestEditor::closeEvent(QCloseEvent *event)
{
    if(closeAttempt())
    {
        if(isModified)
        {
            QString titleString = "Confirm Close - ";
            if(curSaveFile.isEmpty()) titleString += "*.desr";
            else titleString += QFileInfo(curSaveFile).fileName();
            QString closeString = "Close without saving changes?";
            switch(QMessageBox::question(this, titleString, closeString, QMessageBox::Yes|QMessageBox::Save|QMessageBox::Cancel, QMessageBox::Cancel))
            {
                case QMessageBox::Yes:
                    event->accept();
                    break;
                case QMessageBox::Save:
                    onSaveTest();
                    event->accept();
                    break;
                case QMessageBox::Cancel:
                    event->ignore();
                    return;
                default:
                    event->ignore();
                    return;
            }
        }
        else event->accept();
    }
    else {event->ignore(); return;}
    if(!DespotForm::onCloseForm())
    {
        event->ignore();
        return;
    }
}

bool RegressionTestEditor::closeAttempt()
{
    bool okToClose = true;
    emit closingForm(okToClose);
    return okToClose;
}

//used by edit-only editors after main editor emits closingForm() signal
void RegressionTestEditor::closeEditors(bool &okToClose)
{
    if(isModified)
    {
        QString titleString = "Confirm Close - ";
        if(curSaveFile.isEmpty()) titleString += "*.desr";
        else titleString += QFileInfo(curSaveFile).fileName();
        QString closeString = "Close without saving changes?";
        switch(QMessageBox::question(this, titleString, closeString, QMessageBox::Yes|QMessageBox::Save|QMessageBox::Cancel, QMessageBox::Cancel))
        {
            case QMessageBox::Yes:
                okToClose = true;
                isModified = false;
                this->close();
                return;
            case QMessageBox::Save:
                onSaveTest();
                okToClose = true;
                this->close();
                return;
            case QMessageBox::Cancel:
                okToClose = false;
                return;
            default:
                okToClose = false;
                return;
        }
    }
    else
    {
        okToClose = true;
        this->close();
        return;
    }
}

void RegressionTestEditor::handleDoubleClickEvent(QTreeWidgetItem* item,int col)
{
    ui->m_regressionTestWidg->setCurrentItem(item, col);
    if(isTest())
    {
        //read test info and populate new edit-only editor with it
        QMap<std::wstring, std::wstring> editTestMap;
        QString fileName = QString::fromStdWString(findTest(m_projectMap));
        RegressionTestEditor *preTestWidg = new RegressionTestEditor(m_pMainForm, this);
        RegressionTestSerializer *openTest = new RegressionTestSerializer(preTestWidg->ui->m_regressionTestWidg, editTestMap);
        editTestMap = openTest->getMapToTest(fileName);
        preTestWidg->m_projectMap = editTestMap;
        delete openTest;
        preTestWidg->show();
        preTestWidg->setCurFile(fileName);
        preTestWidg->updateWindowTitle(false);
    }
    else onAddAlgorithms();
}

void RegressionTestEditor::setCurFile(const QString& file)
{
    curSaveFile = file;
    lastUsedDir = QFileInfo(file).dir().absolutePath();
}

void RegressionTestEditor::resizeToFit()
{
    ui->m_regressionTestWidg->resizeColumnToContents(0);
    ui->m_regressionTestWidg->resizeColumnToContents(1);
}

//add projects and tests to the tree widget
void RegressionTestEditor::addRoot(const QString& projName, const QString& projType)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->m_regressionTestWidg);
    item->setText(0, projName);
    item->setText(1, projType);
    ui->m_regressionTestWidg->addTopLevelItem(item);
    ui->m_regressionTestWidg->setCurrentItem(item);
    item->setSelected(true);
    updateWindowTitle(true);
    resizeToFit();
}

//add algorithms and subsystems to the tree widget
void RegressionTestEditor::addChild(QTreeWidgetItem *parent, const QString& algorithm, const QString& result)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, algorithm);
    item->setText(1, result);
    parent->addChild(item);
    updateWindowTitle(true);
    resizeToFit();
}

//check if project has already been added to tree widget
void RegressionTestEditor::repeatedFile(const DESpot::DesProject* project, const QString& projType)
{
    if(m_projectMap.contains(project->getName()))
    {
        QMessageBox *msgBox = new QMessageBox();
        msgBox->setText(tr("That project has already been added."));
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->exec();
        return;
    }
    else m_projectMap.insert(project->getName(), project->getFileName());
    addRoot(QString::fromStdWString(project->getName()), projType);
    enableOptions();
}

//check if test has already been added to tree widget
void RegressionTestEditor::repeatedTest(const QString& fileName)
{
    if(fileName.isEmpty()) return;
    QString testName = QFileInfo(fileName).baseName();
    if(m_projectMap.contains(testName.toStdWString()))
    {
        QMessageBox *msgBox = new QMessageBox();
        msgBox->setText(tr("That test has already been added."));
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->exec();
        return;
    }
    else m_projectMap.insert(testName.toStdWString(), fileName.toStdWString());
    addRoot(testName, "Test");
    enableOptions();
}

void RegressionTestEditor::onAddProject()
{
    onOpenProject();
}

//remove project from project map and tree widget
void RegressionTestEditor::onRemoveProject()
{
    if(ui->m_regressionTestWidg->currentItem()->isSelected())
    {
        if(isProject())
        {
            QString projName = ui->m_regressionTestWidg->currentItem()->text(0);
            if(m_projectMap.contains(projName.toStdWString()))
            {
                m_projectMap.remove(projName.toStdWString());
            }

            if(ui->m_regressionTestWidg->topLevelItemCount() == 1)
            {
                ui->m_regressionTestWidg->clear();
                disableOptions();
            }
            else delete ui->m_regressionTestWidg->currentItem();
            updateWindowTitle(true);
        }
        else return;
    }
    else return;
}

//just add the name of the test to widget. Double click event will call to create new window and add test data.
void RegressionTestEditor::onAddTest()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Regression Test", lastUsedDir, "DESpot Regression Test File (*.desr)");
    lastUsedDir = QFileInfo(fileName).dir().absolutePath();
    repeatedTest(fileName);
}

//remove test from project map and tree widget
void RegressionTestEditor::onRemoveTest()
{
    if(ui->m_regressionTestWidg->currentItem()->isSelected())
    {
        if(isTest())
        {
            m_projectMap.remove(ui->m_regressionTestWidg->currentItem()->text(0).toStdWString());

            if(ui->m_regressionTestWidg->topLevelItemCount() == 1)
            {
                ui->m_regressionTestWidg->clear();
                disableOptions();
            }
            else delete ui->m_regressionTestWidg->currentItem();
            updateWindowTitle(true);
        }
        else return;
    }
    else return;
}

void RegressionTestEditor::onOpenProject()
{
    try
    {
        QString fileName = QFileDialog::getOpenFileName(this, "Open Project", lastUsedDir, "DESpot Project Files (*.desp)");
        if(fileName.isEmpty()) return;
        else
        {
            //get project information to populate tree widget
            DESpot::DesProject* project = DESpot::DesProject::load(fileName.toStdWString());
            switch(project->getType())
            {
                case DESpot::eFlatProject:
                    repeatedFile(project, "Flat");
                    break;

                case DESpot::eHierProject:
                    repeatedFile(project, "HISC");
                    break;

                default:
                    assert(false);
                    return;
            }
            lastUsedDir = QFileInfo(fileName).dir().absolutePath();
        }
    }
    catch_display_ex()
}

//clears all algorithms from a selected project
void RegressionTestEditor::onClearProjectAlgorithms()
{
    if(ui->m_regressionTestWidg->topLevelItemCount() == 0) return;
    QTreeWidgetItem* item = ui->m_regressionTestWidg->currentItem();
    if(isProject())
    {
        if(item->childCount() == 0) return;
        while(item->childCount() > 0)
        {
            ui->m_regressionTestWidg->setCurrentItem(item->child(0));
            onRemoveAlgorithm();
        }
    }
    else if(isTest()) return;
    else
    {
        if(!item->parent()->parent())
        {
            ui->m_regressionTestWidg->setCurrentItem(item->parent());
            onClearProjectAlgorithms();
        }
        else
        {
            ui->m_regressionTestWidg->setCurrentItem(item->parent()->parent());
            onClearProjectAlgorithms();
        }
    }
    updateWindowTitle(true);
}

//clear all items from the tree widget
void RegressionTestEditor::onClearAll()
{
    while(ui->m_regressionTestWidg->topLevelItemCount() > 0)
    {
        ui->m_regressionTestWidg->setCurrentItem(ui->m_regressionTestWidg->topLevelItem(0));
        if(isProject()) onRemoveProject();
        else onRemoveTest();
    }
    updateWindowTitle(true);
}

void RegressionTestEditor::onSaveTest()
{
    if(!checkSave()) return;
    if(curSaveFile.isEmpty()) onSaveTestAs();
    else
    {
        if(!repeatTestSaveCheck(curSaveFile)) return;
        RegressionTestSerializer *saveTest = new RegressionTestSerializer(ui->m_regressionTestWidg, m_projectMap);
        if(saveTest->writeRegTestXml(curSaveFile)) updateWindowTitle(false);
        delete saveTest;
    }
}

void RegressionTestEditor::onSaveTestAs()
{
    if(!checkSave()) return;
    if(curSaveFile.isEmpty())
    {
        QString filePath = QFileInfo(lastUsedDir).dir().absolutePath();
        QString fileName = QFileDialog::getSaveFileName(this, "Save Regression Test", filePath, "DESpot Regression Test File (*.desr)");
        QString ext = ".desr";
        if (!fileName.endsWith(ext))
        {
            fileName.append(ext);
        }
        if(!repeatTestSaveCheck(fileName)) return;
        RegressionTestSerializer *saveTest = new RegressionTestSerializer(ui->m_regressionTestWidg, m_projectMap);
        if(saveTest->writeRegTestXml(fileName))
        {
            curSaveFile = fileName;
            lastUsedDir = QFileInfo(curSaveFile).dir().absolutePath();
            updateWindowTitle(false);
        }
        delete saveTest;
    }
    else
    {
        QString filePath = QFileInfo(curSaveFile).dir().absolutePath();
        QString fileName = QFileDialog::getSaveFileName(this, "Save Regression Test", filePath, "DESpot Regression Test File (*.desr)");
        QString ext = ".desr";
        if (!fileName.endsWith(ext))
        {
            fileName.append(ext);
        }
        if(!repeatTestSaveCheck(fileName)) return;
        RegressionTestSerializer *saveTest = new RegressionTestSerializer(ui->m_regressionTestWidg, m_projectMap);
        if(saveTest->writeRegTestXml(fileName))
        {
            curSaveFile = fileName;
            lastUsedDir = QFileInfo(curSaveFile).dir().absolutePath();
            updateWindowTitle(false);
        }
        delete saveTest;
    }
}

bool RegressionTestEditor::checkSave()
{
    if(ui->m_regressionTestWidg->topLevelItemCount() == 0)
    {
        QMessageBox* errEmptyBox = new QMessageBox();
        errEmptyBox->setText("Please add an existing test or project before saving.");
        errEmptyBox->setIcon(QMessageBox::Warning);
        errEmptyBox->exec();
        return false;
    }
    for(int i=0; i<ui->m_regressionTestWidg->topLevelItemCount(); ++i)
    {
        ui->m_regressionTestWidg->setCurrentItem(ui->m_regressionTestWidg->topLevelItem(i));
        QTreeWidgetItem* curItem = ui->m_regressionTestWidg->currentItem();
        if(isProject() && curItem->childCount() == 0)
        {
            QMessageBox* errNoAlgosBox = new QMessageBox();
            errNoAlgosBox->setText("Please add an algorithm to each project being saved");
            errNoAlgosBox->setIcon(QMessageBox::Warning);
            errNoAlgosBox->exec();
            return false;
        }
    }
    return true;
}

//ensure user is not saving a test to itself
bool RegressionTestEditor::repeatTestSaveCheck(const QString& file)
{
    for(int i=0; i<ui->m_regressionTestWidg->topLevelItemCount(); ++i)
    {
        ui->m_regressionTestWidg->setCurrentItem(ui->m_regressionTestWidg->topLevelItem(i));
        QTreeWidgetItem* curItem = ui->m_regressionTestWidg->currentItem();
        if(file == QString::fromStdWString(findTest(m_projectMap)))
        {
            QMessageBox* sameTestSaveBox = new QMessageBox();
            QString errTxt = "Please remove " + curItem->text(0) + " before saving as " + curItem->text(0) + ".desr";
            sameTestSaveBox->setText(errTxt);
            sameTestSaveBox->setIcon(QMessageBox::Warning);
            sameTestSaveBox->exec();
            return false;
        }
    }
    return true;
}

void RegressionTestEditor::updateWindowTitle(bool modified)
{
    if(curSaveFile.isEmpty())
    {
        this->setWindowTitle("Regression Test Editor - *.desr");
        isModified = modified;
    }
    else
    {
        isModified = modified;
        QString testName = QFileInfo(curSaveFile).fileName();
        if(isModified)
        {
            QString windowTitle = "Regression Test Editor - " + testName + "*";
            this->setWindowTitle(windowTitle);
        }
        else
        {
            QString windowTitle = "Regression Test Editor - " + testName;
            this->setWindowTitle(windowTitle);
        }
    }
}

void RegressionTestEditor::makeEditOnly()
{
    ui->m_runTestBtn->hide();
    m_newRegTestAct->setDisabled(true);
    m_runTestAct->setDisabled(true);
}

bool RegressionTestEditor::isProject()
{
    QTreeWidgetItem* item = ui->m_regressionTestWidg->currentItem();
    if(!item->parent() && (item->text(1) == "Flat" || item->text(1) == "HISC"))
        return true;
    else return false;
}

bool RegressionTestEditor::isTest()
{
    QTreeWidgetItem* item = ui->m_regressionTestWidg->currentItem();
    if(!item->parent() && item->text(1) == "Test")
        return true;
    else return false;
}

//returns the absolute file path to the test if found
std::wstring RegressionTestEditor::findTest(const QMap<std::wstring, std::wstring> &projMap)
{
    QString testName = ui->m_regressionTestWidg->currentItem()->text(0);
    if(projMap.contains(testName.toStdWString()))
    {
        return projMap.value(testName.toStdWString());
    }
    else return NULL;
}

//iterates through current tree widget. if test then calls itself on test's tree widget
//if numEmptyChildren == 0 at end of test, user did not add any algorithms
int RegressionTestEditor::startTesting(QTreeWidget *treeWidget, int numEmptyChildren, RegressionTestOutput *output, QMap<std::wstring, std::wstring> &projMap, QList<QString> &repeatTests)
{
    for(int i=0; i<treeWidget->topLevelItemCount(); ++i)
    {
        treeWidget->setCurrentItem(treeWidget->topLevelItem(i));
        QTreeWidgetItem* currentItem = treeWidget->currentItem();
        if(isProject() && currentItem->childCount() > 0)
        {
            createOutputData(currentItem, output, projMap);
        }
        else if(isTest())
        {
            if(!repeatTests.contains(currentItem->text(0)))
            {
                repeatTests.append(currentItem->text(0));
                QString fileName = QString::fromStdWString(findTest(projMap));
                RegressionTestEditor *preTestWidg = new RegressionTestEditor(m_pMainForm);
                RegressionTestSerializer *openTest = new RegressionTestSerializer(preTestWidg->ui->m_regressionTestWidg, projMap);
                projMap = openTest->getMapToTest(fileName);
                delete openTest;
                numEmptyChildren = preTestWidg->startTesting(preTestWidg->ui->m_regressionTestWidg, numEmptyChildren, output, projMap, repeatTests);
                delete preTestWidg;
            }
        }
        else ++numEmptyChildren;
    }
    return numEmptyChildren;
}

//iterates through project algorithms. Adds project item and result to output tree widget
void RegressionTestEditor::createOutputData(QTreeWidgetItem *currentItem, RegressionTestOutput *output, const QMap<std::wstring, std::wstring> &projMap)
{
    QTreeWidgetItem *curItem = currentItem;
    QString projName = curItem->text(0);
    QString projType = curItem->text(1);
    QTreeWidgetItem* parent = output->repeatProject(projName, projType);
    for(int j=0; j<curItem->childCount(); ++j)
    {
        QTreeWidgetItem* currentAlgo = curItem->child(j);
        QString currentAlgoName = currentAlgo->text(0);
        QString currentAlgoResult = currentAlgo->text(1);
        if(projMap.contains(projName.toStdWString()))
        {
            if(currentAlgoName.contains("Subsystem") && currentAlgo->childCount() > 0)
            {
                QTreeWidgetItem* subsysParent = output->repeatSubsysAlgo(parent, currentAlgoName);
                for(int k=0; k<currentAlgo->childCount(); ++k)
                {
                    QTreeWidgetItem* subsysToTest = currentAlgo->child(k);
                    if(!output->repeatSubsys(subsysParent, subsysToTest))
                    {
                        DESpot::DesProject* currentProject = DESpot::DesProject::load(projMap.find(projName.toStdWString()).value());
                        int algoResult = getAlgorithmResults(currentProject, currentAlgoName, subsysToTest->text(0));
                        switch(algoResult)
                        {
                        case 0: //subsystem test returns true
                        {
                            output->addVoidChild(subsysParent, subsysToTest->text(0), subsysToTest->text(1), "Pass");
                            break;
                        }
                        case 1: //subsystem test returns false
                        {
                            output->addVoidChild(subsysParent, subsysToTest->text(0), subsysToTest->text(1), "Fail");
                            break;
                        }
                        case 2: //subsystem test returns error
                        {
                            QString subsysErr = currentAlgoName + " - " + subsysToTest->text(0);
                            output->errMap.insert(projName, subsysErr);
                            output->addVoidChild(subsysParent, subsysToTest->text(0), subsysToTest->text(1), "Error");
                            break;
                        }
                        case 3: //integrity test fail
                        {
                            output->outputIntegrityError(curItem, parent);
                            j = curItem->childCount();
                            k = currentAlgo->childCount();
                            break;
                        }
                        default:
                            break;
                        }
                        delete currentProject;
                    }
                }
                if(subsysParent->childCount() == 0) delete subsysParent;
            }
            else
            {
                if(!output->repeatAlgo(parent, currentAlgoName))
                {
                    DESpot::DesProject* currentProject = DESpot::DesProject::load(projMap.find(projName.toStdWString()).value());
                    int algoResult = getAlgorithmResults(currentProject, currentAlgoName, "");

                    switch(algoResult)
                    {
                    case 0: //test returns true
                    {
                        output->addVoidChild(parent, currentAlgoName, currentAlgoResult, "Pass");
                        break;
                    }
                    case 1: //test returns false
                    {
                        output->addVoidChild(parent, currentAlgoName, currentAlgoResult, "Fail");
                        break;
                    }
                    case 2: //test returns error
                    {
                        output->errMap.insert(projName, currentAlgoName);
                        output->addVoidChild(parent, currentAlgoName, currentAlgoResult, "Error");
                        break;
                    }
                    case 3: //integrity test fail
                    {
                        output->outputIntegrityError(curItem, parent);
                        j = curItem->childCount();
                        break;
                    }
                    default:
                        break;
                    }
                    delete currentProject;
                }
            }
        }
    }
}

//gets the results from algorithm test and returns to createOutputData()
int RegressionTestEditor::getAlgorithmResults(DESpot::DesProject* project, const QString& algorithm, const QString& subsys)
{
    switch(project->getType())
    {
        case DESpot::eFlatProject:
        {
            DESpot::RegressionTestAlgorithms* flatAlgoTest = new DESpot::RegressionTestAlgorithms(project);
            int flatTestResult = flatAlgoTest->runFlatAlgo(algorithm);
            delete flatAlgoTest;
            return flatTestResult;
        }
        case DESpot::eHierProject:
        {
            DESpot::RegressionTestAlgorithms* hierAlgoTest = new DESpot::RegressionTestAlgorithms(project);
            int hierTestResult = hierAlgoTest->runHierAlgo(algorithm, subsys);
            delete hierAlgoTest;
            return hierTestResult;
        }
        default:
        {
            assert(false);
            return 2;
        }
    }
    return 2;
}

//gets users chosen algorithms from algorithm editor and adds to tree widget
//if algorithm already assigned to project, delete old one and add new one
void RegressionTestEditor::getAlgorithms()
{
    QMap<QString, QString> copyAlgoMap = algorithmEditor->getAlgoMap();
    if(copyAlgoMap.isEmpty()) return;
    QTreeWidgetItem* crtProj = ui->m_regressionTestWidg->currentItem();
    QMap<QString, QString>::iterator iter;

    for(int i=0; i<crtProj->childCount(); ++i)
    {
        iter = copyAlgoMap.begin();
        if(!iter.key().contains("Subsystem"))
        {
            while(iter != copyAlgoMap.end())
            {
                if(crtProj->childCount() == 0) break;
                if(crtProj->child(i)->text(0) == iter.key())
                {
                    delete crtProj->child(i);
                }
                ++iter;
            }
        }
    }
    iter = copyAlgoMap.begin();
    while(iter != copyAlgoMap.end())
    {
        if(!iter.key().contains("Subsystem"))
        {
        addChild(crtProj, iter.key(), iter.value());
        }
        else addSubsystems(iter.key(), crtProj->text(0));
        ++iter;
    }
}

//called if subsystem-specific algorithm added
//gets subsystems from project and passes to subsystem editor
void RegressionTestEditor::addSubsystems(const QString&  crtAlgo, const QString& crtProj)
{
    DESpot::DesProject* proj = DESpot::DesProject::load(m_projectMap.find(crtProj.toStdWString()).value());
    DESpot::DesHierProject* subsysProj=(DESpot::DesHierProject*)proj;
    QList<std::wstring> subsysList;
    DESpot::DesHierProject::SubsysIteratorPtr subsysIt = subsysProj->createSubsysIterator();
    for(subsysIt->first(); subsysIt->notDone(); subsysIt->next())
    {
        subsysList.append(subsysIt->currentItem().getName());
    }
    delete subsysProj;

    crtSubsysAlgo = crtAlgo;
    subsysEditor = new RegTestAlgorithmEditor(this, subsysList, crtAlgo);
    connect(subsysEditor, SIGNAL(accepted()), this, SLOT(getSubsystems()));
    subsysEditor->exec();
}

void RegressionTestEditor::getSubsystems()
{
    //get chosen subsystems and results
    QMap<QString, QString> copySubsysMap = subsysEditor->getSubsysMap();
    if(copySubsysMap.isEmpty()) return;
    QMap<QString, QString>::iterator subsysIter;

    //remove chosen subsystems from algorithm if they already exist
    QTreeWidgetItem* subsysAlgo = getSubsysAlgo();
    for(int i=0; i<subsysAlgo->childCount(); ++i)
    {
        subsysIter = copySubsysMap.begin();
        while(subsysIter != copySubsysMap.end())
        {
            if(subsysAlgo->childCount() == 0) break;
            if(subsysAlgo->child(i)->text(0) == subsysIter.key())
            {
                delete subsysAlgo->child(i);
            }
            ++subsysIter;
        }
    }

    //add subsystems to tree widget
    subsysIter = copySubsysMap.begin();
    while(subsysIter != copySubsysMap.end())
    {
        addChild(subsysAlgo, subsysIter.key(), subsysIter.value());
        ++subsysIter;
    }
}

//locate algorithm in tree widget and return it, otherwise add it and call again to return it
QTreeWidgetItem* RegressionTestEditor::getSubsysAlgo()
{
    QTreeWidgetItem* currentItem = ui->m_regressionTestWidg->currentItem();
    for(int i=0; i<currentItem->childCount(); ++i)
    {
        if(currentItem->child(i)->text(0) == crtSubsysAlgo) return currentItem->child(i);
    }
    addChild(currentItem, crtSubsysAlgo, "");
    return getSubsysAlgo();
}

void RegressionTestEditor::onAddAlgorithms()
{
    if(isProject())
    {
        QString projType = ui->m_regressionTestWidg->currentItem()->text(1);
        algorithmEditor = new RegTestAlgorithmEditor(this, projType);
    }
    else if(isTest())
    {
        QMessageBox *testErr = new QMessageBox();
        testErr->setText("Algorithms can not be added to an existing regression test.");
        testErr->setIcon(QMessageBox::Warning);
        testErr->exec();
        return;
    }
    else if(ui->m_regressionTestWidg->currentItem()->parent()->parent())
    {
        ui->m_regressionTestWidg->setCurrentItem(ui->m_regressionTestWidg->currentItem()->parent()->parent());
        QString projType = ui->m_regressionTestWidg->currentItem()->text(1);
        algorithmEditor = new RegTestAlgorithmEditor(this, projType);
    }
    else
    {
        ui->m_regressionTestWidg->setCurrentItem(ui->m_regressionTestWidg->currentItem()->parent());
        QString projType = ui->m_regressionTestWidg->currentItem()->text(1);
        algorithmEditor = new RegTestAlgorithmEditor(this, projType);
    }
    connect(algorithmEditor, SIGNAL(accepted()), this, SLOT(getAlgorithms()));
    algorithmEditor->exec();
}

void RegressionTestEditor::onRemoveAlgorithm()
{
    if(isProject() || isTest()) return;
    else delete ui->m_regressionTestWidg->currentItem();
    updateWindowTitle(true);
}

void RegressionTestEditor::onRunTest()
{
    regTestOutput = new RegressionTestOutput();
    int emptyChildCount = 0;
    QList<QString> testedList;
    QMap<std::wstring, std::wstring> runProjMap = m_projectMap;
    emptyChildCount = startTesting(ui->m_regressionTestWidg, emptyChildCount, regTestOutput, runProjMap, testedList);
    if(ui->m_regressionTestWidg->topLevelItemCount() == emptyChildCount)
    {
        QMessageBox* errTestBox = new QMessageBox();
        errTestBox->setIcon(QMessageBox::Warning);
        errTestBox->setText("Please add an algorithm to run on a project.");
        errTestBox->exec();
        delete regTestOutput;
        return;
    }
    regTestOutput->createFalseOutputData();
    regTestOutput->getTestErrors();
    regTestOutput->show();
    connect(this, SIGNAL(closingForm(bool&)), regTestOutput, SLOT(closeOutput(bool&)));
}
