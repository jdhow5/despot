/*	Author:		Jacob Howard
    Supervisor: Dr. Ryan Leduc

    Project created in conformity with the requirements for the Degree of Master of Engineering in Software Engineering,
    Computing and Software Department,
    McMaster University
    2017
*/

#include "RegTestAlgorithmEditor.h"
#include "ui_RegTestAlgorithmEditor.h"
#include "RegressionTestEditor.h"
#include <QApplication>
#include <QVariant>
#include <QAction>

RegTestAlgorithmEditor::RegTestAlgorithmEditor(QWidget *parent, QString projType) :
    QDialog(parent),
    projectType(projType)
{
    setupUi(this);
    configureRadioButtons();
}

RegTestAlgorithmEditor::RegTestAlgorithmEditor(QWidget *parent, QList<std::wstring> subsysList, QString crtAlgo) :
    QDialog(parent),
    subsystemAlgoList(subsysList),
    subsysAlgo(crtAlgo)
{
    setupSubsysUi(this, subsysAlgo);
}

RegTestAlgorithmEditor::~RegTestAlgorithmEditor()
{
}

//create the algorithm editor dialog
void RegTestAlgorithmEditor::setupUi(QDialog* RegTestAlgorithmEditor)
{
    createAlgorithms();
    if (RegTestAlgorithmEditor->objectName().isEmpty())
        RegTestAlgorithmEditor->setObjectName(QString::fromUtf8("RegTestAlgorithmEditor"));

    RegTestAlgorithmEditor->setWindowTitle("Algorithm Editor");
    RegTestAlgorithmEditor->resize(387, 638);
    verticalLayout = new QVBoxLayout(RegTestAlgorithmEditor);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    m_algorithmBtnBox = new QDialogButtonBox(RegTestAlgorithmEditor);
    m_algorithmBtnBox->setObjectName(QString::fromUtf8("m_algorithmBtnBox"));
    m_algorithmBtnBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    m_algorithmTabWidg = new QTabWidget(RegTestAlgorithmEditor);
    m_algorithmTabWidg->setObjectName(QString::fromUtf8("m_algorithmTabWidg"));

    verticalLayout->addWidget(m_algorithmTabWidg);
    verticalLayout->addWidget(m_algorithmBtnBox);

    if(projectType == "Flat")
    {
        m_flatTab = new QWidget();
        m_flatTab->setObjectName(QString::fromUtf8("m_flatTab"));
        m_algorithmTabWidg->addTab(m_flatTab, QString());
        m_algorithmTabWidg->setTabText(m_algorithmTabWidg->indexOf(m_flatTab),"Flat");
        formLayout = new QFormLayout(m_flatTab);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        flatGrid = new QGridLayout();
        flatGrid->setObjectName(QString::fromUtf8("flatGrid"));

        flatAlgoLabel = new QLabel(m_flatTab);
        flatAlgoLabel->setObjectName(QString::fromUtf8("flatAlgoLabel"));
        flatAlgoLabel->setText(QApplication::translate("RegTestAlgorithmEditor",
                               "Algorithms:", 0, QApplication::UnicodeUTF8));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(flatAlgoLabel->sizePolicy().hasHeightForWidth());
        flatAlgoLabel->setSizePolicy(sizePolicy);

        flatPassLabel = new QLabel(m_flatTab);
        flatPassLabel->setObjectName(QString::fromUtf8("flatPassLabel"));
        flatPassLabel->setText(QApplication::translate("RegTestAlgorithmEditor",
                               "Pass", 0, QApplication::UnicodeUTF8));
        sizePolicy.setHeightForWidth(flatPassLabel->sizePolicy().hasHeightForWidth());
        flatPassLabel->setSizePolicy(sizePolicy);

        flatFailLabel = new QLabel(m_flatTab);
        flatFailLabel->setObjectName(QString::fromUtf8("flatFailLabel"));
        flatFailLabel->setText(QApplication::translate("RegTestAlgorithmEditor",
                               "Fail", 0, QApplication::UnicodeUTF8));
        sizePolicy.setHeightForWidth(flatFailLabel->sizePolicy().hasHeightForWidth());
        flatFailLabel->setSizePolicy(sizePolicy);

        flatBddLabelGrid = new QGridLayout();
        flatBddLabelGrid->setObjectName(QString::fromUtf8("flatBddLabelGrid"));
        flatBddLabel = new QLabel(m_flatTab);
        flatBddLabel->setObjectName(QString::fromUtf8("flatBddLabel"));
        flatBddLabel->setText(QApplication::translate("RegTestAlgorithmEditor",
                              "BDD:", 0, QApplication::UnicodeUTF8));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(flatBddLabel->sizePolicy().hasHeightForWidth());
        flatBddLabel->setSizePolicy(sizePolicy1);
        flatBddLabel->setAutoFillBackground(true);

        flatBddLine = new QFrame(m_flatTab);
        flatBddLine->setObjectName(QString::fromUtf8("flatBddLine"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(flatBddLine->sizePolicy().hasHeightForWidth());
        flatBddLine->setSizePolicy(sizePolicy2);
        flatBddLine->setFrameShape(QFrame::HLine);
        flatBddLine->setFrameShadow(QFrame::Sunken);

        flatBddLabelGrid->addWidget(flatBddLabel, 0, 0, 1, 1);
        flatBddLabelGrid->addWidget(flatBddLine, 0, 1, 1, 1);

        flatGrid->addWidget(flatAlgoLabel, 0, 0, 1, 1);
        flatGrid->addWidget(flatPassLabel, 0, 1, 1, 1);
        flatGrid->addWidget(flatFailLabel, 0, 2, 1, 1);
        flatGrid->addLayout(flatBddLabelGrid, 3, 0, 1, 3);

        int flatRow = 1;
        for(int i=0; i<algorithmList.size(); ++i)
        {
            //if adding new non-BDD Flat algorithm, increase row value below by 1
            if(flatRow == 3) ++flatRow;

            algorithmList[i].algo = new QCheckBox(m_flatTab);
            algorithmList[i].algo->setObjectName(QString(algorithmList[i].name + "ChkBox"));
            algorithmList[i].algo->setText(algorithmList[i].name);

            algorithmList[i].tBtn = new QRadioButton(m_flatTab);
            algorithmList[i].tBtn->setObjectName(QString(algorithmList[i].name + "TBtn"));
            algorithmList[i].tBtn->setText(QString());
            algorithmList[i].tBtn->setEnabled(false);
            algorithmList[i].fBtn = new QRadioButton(m_flatTab);
            algorithmList[i].fBtn->setObjectName(QString(algorithmList[i].name + "FBtn"));
            algorithmList[i].fBtn->setText(QString());
            algorithmList[i].fBtn->setEnabled(false);

            algorithmList[i].group = new QButtonGroup(RegTestAlgorithmEditor);
            algorithmList[i].group->setObjectName(algorithmList[i].name + "Group");
            algorithmList[i].group->addButton(algorithmList[i].tBtn);
            algorithmList[i].group->addButton(algorithmList[i].fBtn);

            flatGrid->addWidget(algorithmList[i].algo, flatRow, 0, 1, 1);
            flatGrid->addWidget(algorithmList[i].tBtn, flatRow, 1, 1, 1);
            flatGrid->addWidget(algorithmList[i].fBtn, flatRow, 2, 1, 1);

            ++flatRow;
        }
        formLayout->setLayout(0, QFormLayout::LabelRole, flatGrid);
    }

    else if(projectType == "HISC")
    {
        m_hiscTab = new QWidget();
        m_hiscTab->setObjectName(QString::fromUtf8("m_hiscTab"));
        m_algorithmTabWidg->addTab(m_hiscTab, QString());
        m_algorithmTabWidg->setTabText(m_algorithmTabWidg->indexOf(m_hiscTab), "HISC");
        formLayout_2 = new QFormLayout(m_hiscTab);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        hiscGrid = new QGridLayout();
        hiscGrid->setObjectName(QString::fromUtf8("hiscGrid"));

        hiscAlgoLabel = new QLabel(m_hiscTab);
        hiscAlgoLabel->setObjectName(QString::fromUtf8("hiscAlgoLabel"));
        hiscAlgoLabel->setText(QApplication::translate("RegTestAlgorithmEditor",
                               "Algorithms:", 0, QApplication::UnicodeUTF8));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(hiscAlgoLabel->sizePolicy().hasHeightForWidth());
        hiscAlgoLabel->setSizePolicy(sizePolicy3);

        hiscPassLabel = new QLabel(m_hiscTab);
        hiscPassLabel->setObjectName(QString::fromUtf8("hiscPassLabel"));
        hiscPassLabel->setText(QApplication::translate("RegTestAlgorithmEditor",
                               "Pass", 0, QApplication::UnicodeUTF8));
        sizePolicy3.setHeightForWidth(hiscPassLabel->sizePolicy().hasHeightForWidth());
        hiscPassLabel->setSizePolicy(sizePolicy3);

        hiscFailLabel = new QLabel(m_hiscTab);
        hiscFailLabel->setObjectName(QString::fromUtf8("hiscFailLabel"));
        hiscFailLabel->setText(QApplication::translate("RegTestAlgorithmEditor",
                               "Fail", 0, QApplication::UnicodeUTF8));
        sizePolicy3.setHeightForWidth(hiscFailLabel->sizePolicy().hasHeightForWidth());
        hiscFailLabel->setSizePolicy(sizePolicy3);

        hiscBddLabelGrid = new QGridLayout();
        hiscBddLabelGrid->setObjectName(QString::fromUtf8("hiscBddLabelGrid"));
        hiscBddLabel = new QLabel(m_hiscTab);
        hiscBddLabel->setObjectName(QString::fromUtf8("hiscBddLabel"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(hiscBddLabel->sizePolicy().hasHeightForWidth());
        hiscBddLabel->setSizePolicy(sizePolicy4);
        hiscBddLabel->setAutoFillBackground(true);

        hiscBddLine = new QFrame(m_hiscTab);
        hiscBddLine->setObjectName(QString::fromUtf8("hiscBddLine"));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(hiscBddLine->sizePolicy().hasHeightForWidth());
        hiscBddLine->setSizePolicy(sizePolicy5);
        hiscBddLine->setFrameShape(QFrame::HLine);
        hiscBddLine->setFrameShadow(QFrame::Sunken);

        hiscBddLabelGrid->addWidget(hiscBddLine, 0, 1, 1, 1);
        hiscBddLabelGrid->addWidget(hiscBddLabel, 0, 0, 1, 1);

        hiscGrid->addWidget(hiscAlgoLabel, 0, 0, 1, 1);
        hiscGrid->addWidget(hiscPassLabel, 0, 1, 1, 1);
        hiscGrid->addWidget(hiscFailLabel, 0, 2, 1, 1);
        hiscGrid->addLayout(hiscBddLabelGrid, 9, 0, 1, 3);

        int hiscRow = 1;
        for(int i=0; i<algorithmList.size(); ++i)
        {
            //if adding new non-BDD HISC algorithm, increase row value below by 1
            if(hiscRow == 9) ++hiscRow;

            algorithmList[i].algo = new QCheckBox(m_hiscTab);
            algorithmList[i].algo->setObjectName(QString(algorithmList[i].name + "ChkBox"));
            algorithmList[i].algo->setText(algorithmList[i].name);

            if(!algorithmList[i].name.contains("Subsystem"))
            {
                algorithmList[i].tBtn = new QRadioButton(m_hiscTab);
                algorithmList[i].tBtn->setObjectName(QString(algorithmList[i].name + "TBtn"));
                algorithmList[i].tBtn->setText(QString());
                algorithmList[i].tBtn->setEnabled(false);
                algorithmList[i].fBtn = new QRadioButton(m_hiscTab);
                algorithmList[i].fBtn->setObjectName(QString(algorithmList[i].name + "FBtn"));
                algorithmList[i].fBtn->setText(QString());
                algorithmList[i].fBtn->setEnabled(false);

                algorithmList[i].group = new QButtonGroup(RegTestAlgorithmEditor);
                algorithmList[i].group->setObjectName(algorithmList[i].name + "Group");
                algorithmList[i].group->addButton(algorithmList[i].tBtn);
                algorithmList[i].group->addButton(algorithmList[i].fBtn);

                hiscGrid->addWidget(algorithmList[i].tBtn, hiscRow, 1, 1, 1);
                hiscGrid->addWidget(algorithmList[i].fBtn, hiscRow, 2, 1, 1);
            }

            hiscGrid->addWidget(algorithmList[i].algo, hiscRow, 0, 1, 1);

            ++hiscRow;
        }
        formLayout_2->setLayout(0, QFormLayout::LabelRole, hiscGrid);
    }
    QMetaObject::connectSlotsByName(RegTestAlgorithmEditor);
}

//create the subsystem editor dialog
void RegTestAlgorithmEditor::setupSubsysUi(QDialog* RegTestAlgorithmEditor, const QString& algo)
{
    if (RegTestAlgorithmEditor->objectName().isEmpty())
        RegTestAlgorithmEditor->setObjectName(QString::fromUtf8("RegTestAlgorithmEditor"));

    RegTestAlgorithmEditor->setWindowTitle("Subsystem Editor");
    RegTestAlgorithmEditor->resize(387, 638);
    verticalLayout = new QVBoxLayout(RegTestAlgorithmEditor);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    m_algorithmBtnBox = new QDialogButtonBox(RegTestAlgorithmEditor);
    m_algorithmBtnBox->setObjectName(QString::fromUtf8("m_algorithmBtnBox"));
    m_algorithmBtnBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    m_algorithmTabWidg = new QTabWidget(RegTestAlgorithmEditor);
    m_algorithmTabWidg->setObjectName(QString::fromUtf8("m_algorithmTabWidg"));

    verticalLayout->addWidget(m_algorithmTabWidg);
    verticalLayout->addWidget(m_algorithmBtnBox);

    m_subsysTab = new QWidget();
    m_subsysTab->setObjectName(QString::fromUtf8("m_subsysTab"));
    m_algorithmTabWidg->addTab(m_subsysTab, QString());
    m_algorithmTabWidg->setTabText(m_algorithmTabWidg->indexOf(m_subsysTab), algo);
    subsysGrid = new QGridLayout();
    subsysGrid->setObjectName(QString::fromUtf8("subsysGrid"));

    subsysLabel = new QLabel(m_subsysTab);
    subsysLabel->setObjectName(QString::fromUtf8("subsysLabel"));
    subsysLabel->setText(QApplication::translate("RegTestAlgorithmEditor",
                           "Subsystems:", 0, QApplication::UnicodeUTF8));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(subsysLabel->sizePolicy().hasHeightForWidth());
    subsysLabel->setSizePolicy(sizePolicy);

    subsysPassLabel = new QLabel(m_subsysTab);
    subsysPassLabel->setObjectName(QString::fromUtf8("subsysPassLabel"));
    subsysPassLabel->setText(QApplication::translate("RegTestAlgorithmEditor",
                           "Pass", 0, QApplication::UnicodeUTF8));
    sizePolicy.setHeightForWidth(subsysPassLabel->sizePolicy().hasHeightForWidth());
    subsysPassLabel->setSizePolicy(sizePolicy);

    subsysFailLabel = new QLabel(m_subsysTab);
    subsysFailLabel->setObjectName(QString::fromUtf8("flatFailLabel"));
    subsysFailLabel->setText(QApplication::translate("RegTestAlgorithmEditor",
                           "Fail", 0, QApplication::UnicodeUTF8));
    sizePolicy.setHeightForWidth(subsysFailLabel->sizePolicy().hasHeightForWidth());
    subsysFailLabel->setSizePolicy(sizePolicy);

    subsysGrid->addWidget(subsysLabel, 0, 0, 1, 1);
    subsysGrid->addWidget(subsysPassLabel, 0, 1, 1, 1);
    subsysGrid->addWidget(subsysFailLabel, 0, 2, 1, 1);

    int subsysRow = 1;
    subsys = (algorithmChoice*) malloc(subsystemAlgoList.size() * sizeof(algorithmChoice));
    for(int i=0; i<subsystemAlgoList.size(); ++i)
    {
        subsys[i].algo = new QCheckBox(m_subsysTab);
        subsys[i].algo->setText(QString::fromStdWString(subsystemAlgoList[i]));
        subsys[i].algo->setObjectName(QString(QString::fromStdWString(subsystemAlgoList[i]) + "ChkBox"));
        subsys[i].tBtn = new QRadioButton(m_subsysTab);
        subsys[i].tBtn->setObjectName(QString(QString::fromStdWString(subsystemAlgoList[i]) + "tBtn"));
        subsys[i].tBtn->setEnabled(false);
        subsys[i].fBtn = new QRadioButton(m_subsysTab);
        subsys[i].fBtn->setObjectName(QString(QString::fromStdWString(subsystemAlgoList[i]) + "fBtn"));
        subsys[i].fBtn->setEnabled(false);
        subsys[i].group = new QButtonGroup(RegTestAlgorithmEditor);
        subsys[i].group->setObjectName(QString(QString::fromStdWString(subsystemAlgoList[i]) + "Group"));
        subsys[i].group->addButton(subsys[i].tBtn);
        subsys[i].group->addButton(subsys[i].fBtn);

        subsysGrid->addWidget(subsys[i].algo, subsysRow, 0, 1, 1);
        subsysGrid->addWidget(subsys[i].tBtn, subsysRow, 1, 1, 1);
        subsysGrid->addWidget(subsys[i].fBtn, subsysRow, 2, 1, 1);

        ++subsysRow;
    }

    formLayout = new QFormLayout(m_subsysTab);
    formLayout->setObjectName(QString::fromUtf8("formLayout"));
    formLayout->setLayout(0, QFormLayout::LabelRole, subsysGrid);
    QMetaObject::connectSlotsByName(RegTestAlgorithmEditor);

    for(int i=0; i<subsystemAlgoList.size(); ++i)
    {
        connect(subsys[i].algo, SIGNAL(toggled(bool)), subsys[i].tBtn, SLOT(setEnabled(bool)));
        connect(subsys[i].algo, SIGNAL(toggled(bool)), subsys[i].fBtn, SLOT(setEnabled(bool)));
    }
}

//assign ui objects to items in algorithmChoice struct
//new algorithm ui objects added here
void RegTestAlgorithmEditor::createAlgorithms()
{
    if(projectType == "Flat")
    {
        flatCont.algo = m_flatContChkBox;
        flatCont.tBtn = m_flatContTBtn;
        flatCont.fBtn = m_flatContFBtn;
        flatCont.group = m_flatContGroup;
        flatCont.name = "Controllability";
        flatNonblock.algo = m_flatNonblockChkBox;
        flatNonblock.tBtn = m_flatNonblockTBtn;
        flatNonblock.fBtn = m_flatNonblockFBtn;
        flatNonblock.group = m_flatNonblockGroup;
        flatNonblock.name = "Nonblocking";
        bddFlatNonblock.algo = m_bddFlatNonblockChkBox;
        bddFlatNonblock.tBtn = m_bddFlatNonblockTBtn;
        bddFlatNonblock.fBtn = m_bddFlatNonblockFBtn;
        bddFlatNonblock.group = m_bddFlatNonblockGroup;
        bddFlatNonblock.name = "BDD Nonblocking";
        untimedCont.algo = m_untimedContChkBox;
        untimedCont.tBtn = m_untimedContTBtn;
        untimedCont.fBtn = m_untimedContFBtn;
        untimedCont.group = m_untimedContGroup;
        untimedCont.name = "Untimed Controllability";
        sdCont.algo = m_sdContChkBox;
        sdCont.tBtn = m_sdContTBtn;
        sdCont.fBtn = m_sdContFBtn;
        sdCont.group = m_sdContGroup;
        sdCont.name = "SD Controllability";
        ptb.algo = m_ptbChkBox;
        ptb.tBtn = m_ptbTBtn;
        ptb.fBtn = m_ptbFBtn;
        ptb.group = m_ptbGroup;
        ptb.name = "Proper Time Behavior";
        sspb.algo = m_sspbChkBox;
        sspb.tBtn = m_sspbTBtn;
        sspb.fBtn = m_sspbFBtn;
        sspb.group = m_sspbGroup;
        sspb.name = "S-Singular Prohibitable Behavior";
        plantComplete.algo = m_plantCompleteChkBox;
        plantComplete.tBtn = m_plantCompleteTBtn;
        plantComplete.fBtn = m_plantCompleteFBtn;
        plantComplete.group = m_plantCompleteGroup;
        plantComplete.name = "Plant Completeness";
        alf.algo = m_alfChkBox;
        alf.tBtn = m_alfTBtn;
        alf.fBtn = m_alfFBtn;
        alf.group = m_alfGroup;
        alf.name = "Activity Loop Free";
        createAlgorithmList();
    }

    else if(projectType == "HISC")
    {
        hiscCont.algo = m_hiscContChkBox;
        hiscCont.tBtn = m_hiscContTBtn;
        hiscCont.fBtn = m_hiscContFBtn;
        hiscCont.group = m_hiscContGroup;
        hiscCont.name = "Controllability";
        hiscNonblock.algo = m_hiscNonblockChkBox;
        hiscNonblock.tBtn = m_hiscNonblockTBtn;
        hiscNonblock.fBtn = m_hiscNonblockFBtn;
        hiscNonblock.group = m_hiscNonblockGroup;
        hiscNonblock.name = "Nonblocking";
        subsysLDIConsistent.algo = m_subsysLDIConsistentChkBox;
        subsysLDIConsistent.name = "Subsystem LD I-Consistent";
        subsysLDCont.algo = m_subsysLDContChkBox;
        subsysLDCont.name = "Subsystem LD Controllable";
        subsysLDNonblock.algo = m_subsysLDNonblockChkBox;
        subsysLDNonblock.name = "Subsystem LD Nonblocking";
        ldIConsistent.algo = m_ldIConsistentChkBox;
        ldIConsistent.tBtn = m_ldIConsistentTBtn;
        ldIConsistent.fBtn = m_ldIConsistentFBtn;
        ldIConsistent.group = m_ldIConsistentGroup;
        ldIConsistent.name = "LD Interface Consistent";
        ldLWCont.algo = m_ldLWContChkBox;
        ldLWCont.tBtn = m_ldLWContTBtn;
        ldLWCont.fBtn = m_ldLWContFBtn;
        ldLWCont.group = m_ldLWContGroup;
        ldLWCont.name = "LD Level-Wise Controllability";
        ldLWNonblock.algo = m_ldLWNonblockChkBox;
        ldLWNonblock.tBtn = m_ldLWNonblockTBtn;
        ldLWNonblock.fBtn = m_ldLWNonblockFBtn;
        ldLWNonblock.group = m_ldLWNonblockGroup;
        ldLWNonblock.name = "LD Level-Wise Nonblocking";


        bddHiscCont.algo = m_bddHiscContChkBox;
        bddHiscCont.tBtn = m_bddHiscContTBtn;
        bddHiscCont.fBtn = m_bddHiscContFBtn;
        bddHiscCont.group = m_bddHiscContGroup;
        bddHiscCont.name = "BDD Controllability";
        bddHiscNonblock.algo = m_bddHiscNonblockChkBox;
        bddHiscNonblock.tBtn = m_bddHiscNonblockTBtn;
        bddHiscNonblock.fBtn = m_bddHiscNonblockFBtn;
        bddHiscNonblock.name = "BDD Nonblocking";
        bddHiscNonblock.group = m_bddHiscNonblockGroup;
        bddSubsysLDIConsistent.algo = m_bddSubsysLDIConsistentChkBox;
        bddSubsysLDIConsistent.name = "BDD Subsystem LD I-Consistent";
        bddSubsysLDCont.algo = m_bddSubsysLDContChkBox;
        bddSubsysLDCont.name = "BDD Subsystem LD Controllable";
        bddSubsysLDNonblock.algo = m_bddSubsysLDNonblockChkBox;
        bddSubsysLDNonblock.name = "BDD Subsystem LD Nonblocking";
        bddLDIConsistent.algo = m_bddLDIConsistentChkBox;
        bddLDIConsistent.tBtn = m_bddLDIConsistentTBtn;
        bddLDIConsistent.fBtn = m_bddLDIConsistentFBtn;
        bddLDIConsistent.group = m_bddLDIConsistentGroup;
        bddLDIConsistent.name = "BDD LD Interface Consistent";
        bddLDLWCont.algo = m_bddLDLWContChkBox;
        bddLDLWCont.tBtn = m_bddLDLWContTBtn;
        bddLDLWCont.fBtn = m_bddLDLWContFBtn;
        bddLDLWCont.group = m_bddLDLWContGroup;
        bddLDLWCont.name = "BDD LD Level-Wise Controllability";
        bddLDLWNonblock.algo = m_bddLDLWNonblockChkBox;
        bddLDLWNonblock.tBtn = m_bddLDLWNonblockTBtn;
        bddLDLWNonblock.fBtn = m_bddLDLWNonblockFBtn;
        bddLDLWNonblock.group = m_bddLDLWNonblockGroup;
        bddLDLWNonblock.name = "BDD LD Level-Wise Nonblocking";
        createAlgorithmList();
    }
}

//create list of algorithms to add to dialog
//if adding new algorithm, add object name here in order it should be on form
void RegTestAlgorithmEditor::createAlgorithmList()
{
    if(projectType == "Flat")
    {
        algorithmList.append(flatCont);
        algorithmList.append(flatNonblock);
        //BDD algorithms
        algorithmList.append(bddFlatNonblock);
        algorithmList.append(untimedCont);
        algorithmList.append(sdCont);
        algorithmList.append(ptb);
        algorithmList.append(sspb);
        algorithmList.append(plantComplete);
        algorithmList.append(alf);
    }
    else if(projectType == "HISC")
    {
        algorithmList.append(hiscCont);
        algorithmList.append(hiscNonblock);
        algorithmList.append(subsysLDIConsistent);
        algorithmList.append(subsysLDCont);
        algorithmList.append(subsysLDNonblock);
        algorithmList.append(ldIConsistent);
        algorithmList.append(ldLWCont);
        algorithmList.append(ldLWNonblock);
        //BDD algorithms
        algorithmList.append(bddHiscCont);
        algorithmList.append(bddHiscNonblock);
        algorithmList.append(bddSubsysLDIConsistent);
        algorithmList.append(bddSubsysLDCont);
        algorithmList.append(bddSubsysLDNonblock);
        algorithmList.append(bddLDIConsistent);
        algorithmList.append(bddLDLWCont);
        algorithmList.append(bddLDLWNonblock);
    }
}

void RegTestAlgorithmEditor::configureRadioButtons()
{
    for(int i=0; i<algorithmList.size(); ++i)
    {
        if(!algorithmList[i].name.contains("Subsystem"))
        {
            connect(algorithmList[i].algo, SIGNAL(toggled(bool)), algorithmList[i].tBtn, SLOT(setEnabled(bool)));
            connect(algorithmList[i].algo, SIGNAL(toggled(bool)), algorithmList[i].fBtn, SLOT(setEnabled(bool)));
        }
    }
}

//get users chosen algorithms with results and return map to regression test editor
QMap<QString, QString> RegTestAlgorithmEditor::getAlgoMap()
{
    for(int i=0; i<algorithmList.size(); ++i)
    {
        if(!algorithmList[i].name.contains("Subsystem"))
        {
            if(algorithmList[i].tBtn->isChecked() && algorithmList[i].algo->isChecked())
            {
                algoMap.insert(algorithmList[i].name, "Pass");
            }
            else if(algorithmList[i].fBtn->isChecked() && algorithmList[i].algo->isChecked())
            {
                algoMap.insert(algorithmList[i].name, "Fail");
            }
        }
        else
        {
            if(algorithmList[i].algo->isChecked())
            {
                algoMap.insert(algorithmList[i].name, "");
            }
        }
    }
    return algoMap;
}

//get users chosen subsystems with results and return map to regression test editor
QMap<QString, QString> RegTestAlgorithmEditor::getSubsysMap()
{
    QMap<QString, QString> subsysMap;

    for(int i=0; i<subsystemAlgoList.size(); ++i)
    {
        if(subsys[i].tBtn->isChecked() && subsys[i].algo->isChecked())
        {
            subsysMap.insert(QString::fromStdWString(subsystemAlgoList[i]), "Pass");
        }
        else if(subsys[i].fBtn->isChecked() && subsys[i].algo->isChecked())
        {
            subsysMap.insert(QString::fromStdWString(subsystemAlgoList[i]), "Fail");
        }
    }
    free(subsys);
    return subsysMap;
}

void RegTestAlgorithmEditor::on_m_algorithmBtnBox_accepted()
{
    this->accept();
    this->close();
}


void RegTestAlgorithmEditor::on_m_algorithmBtnBox_rejected()
{
    this->reject();
    this->close();
}
