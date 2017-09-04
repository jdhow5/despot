/*	Author:		Jacob Howard
    Supervisor: Dr. Ryan Leduc

    Project created in conformity with the requirements for the Degree of Master of Engineering in Software Engineering,
    Computing and Software Department,
    McMaster University
    2017
*/

#ifndef REGTESTALGORITHMEDITOR_H
#define REGTESTALGORITHMEDITOR_H

#include <QDialog>
#include <QCheckBox>
#include <QMap>
#include <QRadioButton>
#include <QTreeWidgetItem>
#include <QLabel>
#include <QGridLayout>
#include <QFormLayout>
#include <QDialogButtonBox>

class RegTestAlgorithmEditor : public QDialog
{
    Q_OBJECT

public:
    RegTestAlgorithmEditor(QWidget *parent = 0, QString projType = NULL);
    RegTestAlgorithmEditor(QWidget *parent = 0, QList<std::wstring> subsysList = {}, QString crtAlgo = NULL);
    ~RegTestAlgorithmEditor();

    //if adding new algorithm add new algorithm object name to list below
    struct algorithmChoice
    {
        QCheckBox* algo;
        QRadioButton* tBtn;
        QRadioButton* fBtn;
        QButtonGroup* group;
        QString name;
    }
    flatCont, flatNonblock, bddFlatNonblock, untimedCont, sdCont, ptb, sspb, plantComplete, alf,
    hiscCont, hiscNonblock, subsysLDIConsistent, subsysLDNonblock, subsysLDCont, ldIConsistent, ldLWNonblock, ldLWCont,
    bddHiscCont, bddHiscNonblock, bddSubsysLDIConsistent, bddSubsysLDNonblock, bddSubsysLDCont, bddLDIConsistent, bddLDLWNonblock, bddLDLWCont, *subsys;

    void configureTabs(const QString& projType);

    QMap<QString, QString> algoMap;

    QList<algorithmChoice> algorithmList;
    QMap<QString, QString> getAlgoMap();
    QMap<QString, QString> getSubsysMap();


private slots:
    void on_m_algorithmBtnBox_accepted();
    void on_m_algorithmBtnBox_rejected();

private:
    const QString projectType;
    const QList<std::wstring> subsystemAlgoList;
    const QString subsysAlgo;
    void setupUi(QDialog* RegTestAlgorithmEditor);
    void setupSubsysUi(QDialog* RegTestAlgorithmEditor, const QString& algo);
    void createAlgorithms();
    void createAlgorithmList();
    void configureRadioButtons();
    void configureSubsysRadioButtons();

    //if adding new algorithm add checkbox, 2 radiobutton, buttongroup, label here
    //if adding subsystem-specific algorithm, exclude radiobuttons
    QVBoxLayout *verticalLayout;
    QTabWidget *m_algorithmTabWidg;
    QWidget *m_flatTab;
    QFormLayout *formLayout;
    QGridLayout *flatGrid;
    QLabel *flatAlgoLabel;
    QLabel *flatPassLabel;
    QLabel *flatFailLabel;
    QRadioButton *m_flatNonblockTBtn;
    QRadioButton *m_flatNonblockFBtn;
    QCheckBox *m_flatContChkBox;
    QRadioButton *m_flatContTBtn;
    QRadioButton *m_flatContFBtn;
    QGridLayout *flatBddLabelGrid;
    QLabel *flatBddLabel;
    QFrame *flatBddLine;
    QCheckBox *m_bddFlatNonblockChkBox;
    QRadioButton *m_bddFlatNonblockTBtn;
    QRadioButton *m_bddFlatNonblockFBtn;
    QCheckBox *m_untimedContChkBox;
    QRadioButton *m_untimedContTBtn;
    QRadioButton *m_untimedContFBtn;
    QCheckBox *m_sdContChkBox;
    QRadioButton *m_sdContTBtn;
    QRadioButton *m_sdContFBtn;
    QCheckBox *m_ptbChkBox;
    QRadioButton *m_ptbTBtn;
    QRadioButton *m_ptbFBtn;
    QCheckBox *m_sspbChkBox;
    QRadioButton *m_sspbTBtn;
    QRadioButton *m_sspbFBtn;
    QCheckBox *m_plantCompleteChkBox;
    QRadioButton *m_plantCompleteTBtn;
    QRadioButton *m_plantCompleteFBtn;
    QCheckBox *m_alfChkBox;
    QRadioButton *m_alfTBtn;
    QRadioButton *m_alfFBtn;
    QCheckBox *m_flatNonblockChkBox;
    QWidget *m_hiscTab;
    QFormLayout *formLayout_2;
    QGridLayout *hiscGrid;
    QCheckBox *m_bddLDLWContChkBox;
    QRadioButton *m_bddLDLWContTBtn;
    QRadioButton *m_bddLDLWContFBtn;
    QRadioButton *m_ldIConsistentTBtn;
    QRadioButton *m_ldIConsistentFBtn;
    QCheckBox *m_ldLWNonblockChkBox;
    QRadioButton *m_ldLWNonblockTBtn;
    QRadioButton *m_ldLWNonblockFBtn;
    QCheckBox *m_ldLWContChkBox;
    QCheckBox *m_bddLDIConsistentChkBox;
    QRadioButton *m_bddLDIConsistentTBtn;
    QRadioButton *m_bddLDIConsistentFBtn;
    QCheckBox *m_bddLDLWNonblockChkBox;
    QLabel *hiscAlgoLabel;
    QCheckBox *m_hiscNonblockChkBox;
    QCheckBox *m_bddHiscContChkBox;
    QRadioButton *m_bddHiscNonblockTBtn;
    QRadioButton *m_bddHiscNonblockFBtn;
    QCheckBox *m_ldIConsistentChkBox;
    QCheckBox *m_subsysLDIConsistentChkBox;
    QCheckBox *m_subsysLDNonblockChkBox;
    QRadioButton *m_bddLDLWNonblockTBtn;
    QRadioButton *m_bddLDLWNonblockFBtn;
    QGridLayout *hiscBddLabelGrid;
    QFrame *hiscBddLine;
    QLabel *hiscBddLabel;
    QRadioButton *m_bddHiscContTBtn;
    QRadioButton *m_bddHiscContFBtn;
    QRadioButton *m_ldLWContFBtn;
    QCheckBox *m_subsysLDContChkBox;
    QCheckBox *m_hiscContChkBox;
    QCheckBox *m_bddSubsysLDNonblockChkBox;
    QCheckBox *m_bddSubsysLDContChkBox;
    QRadioButton *m_hiscNonblockTBtn;
    QRadioButton *m_hiscContTBtn;
    QRadioButton *m_hiscContFBtn;
    QRadioButton *m_ldLWContTBtn;
    QLabel *hiscFailLabel;
    QCheckBox *m_bddHiscNonblockChkBox;
    QRadioButton *m_hiscNonblockFBtn;
    QLabel *hiscPassLabel;
    QCheckBox *m_bddSubsysLDIConsistentChkBox;
    QDialogButtonBox *m_algorithmBtnBox;
    QButtonGroup *m_bddLDLWNonblockGroup;
    QButtonGroup *m_untimedContGroup;
    QButtonGroup *m_bddHiscNonblockGroup;
    QButtonGroup *m_hiscNonblockGroup;
    QButtonGroup *m_bddFlatNonblockGroup;
    QButtonGroup *m_ldLWNonblockGroup;
    QButtonGroup *m_bddLDIConsistentGroup;
    QButtonGroup *m_bddLDLWContGroup;
    QButtonGroup *m_alfGroup;
    QButtonGroup *m_ptbGroup;
    QButtonGroup *m_hiscContGroup;
    QButtonGroup *m_flatNonblockGroup;
    QButtonGroup *m_ldIConsistentGroup;
    QButtonGroup *m_bddHiscContGroup;
    QButtonGroup *m_ldLWContGroup;
    QButtonGroup *m_flatContGroup;
    QButtonGroup *m_sdContGroup;
    QButtonGroup *m_plantCompleteGroup;
    QButtonGroup *m_sspbGroup;

    QWidget *m_subsysTab;
    QGridLayout *subsysGrid;
    QLabel *subsysLabel;
    QLabel *subsysPassLabel;
    QLabel *subsysFailLabel;
};

#endif // REGTESTALGORITHMEDITOR_H
