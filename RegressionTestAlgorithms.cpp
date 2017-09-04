/*	Author:		Jacob Howard
    Supervisor: Dr. Ryan Leduc

    Project created in conformity with the requirements for the Degree of Master of Engineering in Software Engineering,
    Computing and Software Department,
    McMaster University
    2017
*/

#include "RegressionTestAlgorithms.h"
#include "HierProjectEditor.h"
#include "CommonDefinitions.h"
#include "EventPoolForm.h"
#include "HierProjectWorkspace.h"
#include "DesHierProject.h"
#include "DesSubsystem.h"
#include "DesInterface.h"
#include "AddLowLevelSubsysDlg.h"
#include "AddInterfaceDlg.h"
#include "Des.h"
#include "HierProjStructureUiPart.h"
#include "AddDesToProjectDlg.h"
#include "HierProjIntegrityAlgo.h"
#include "WaitCursor.h"
#include "LdInterfaceCheckAlgo.h"
#include "OutputUiPart.h"
#include "IConsistAlgo.h"
#include "LwNonBlockingAlgo.h"
#include "LwCtrlAlgo.h"
#include "IConsistAnsAcceptAlgo.h"
#include "LowSubsysIConsistAlgo.h"
#include "HighSubsysNonBlockAlgo.h"
#include "LowSubsysNonBlockAlgo.h"
#include "HighSubsysCtrlAlgo.h"
#include "LowSubsysCtrlAlgo.h"
#include "CheckProjectDlg.h"
#include "HierProjectPropCheck.h"
#include "DesEditor.h"
#include "BddHiscMain.h"
#include "SubsystemLWSynDialog.h"
#include "FlatProjectEditor.h"
#include "ProjectEditor.h"
#include "BddSdMain.h"

namespace DESpot
{

RegressionTestAlgorithms::RegressionTestAlgorithms(DESpot::DesProject* project) :
    m_pProject(project)
{
    if(m_pProject->getType() == eHierProject)
    {
        m_pCrtSubsys = null;
        m_pCrtInterf = null;
    }
}

RegressionTestAlgorithms::~RegressionTestAlgorithms(void)
{
}

DesFlatProject* RegressionTestAlgorithms::flatProject()
{
    return dynamic_cast<DesFlatProject*>(m_pProject);
}

DesHierProject* RegressionTestAlgorithms::project()
{
    return dynamic_cast<DesHierProject*>(m_pProject);
}

//if adding algorithm, add here or under runHierAlgo()
//also add algorithm code as other are below
//only need to return 0 if pass or 1 if fail
int RegressionTestAlgorithms::runFlatAlgo(const QString& algorithm)
{
    FlatProjIntegrityAlgo integAlgo;
    if(onCheckProjectIntegrityRT(integAlgo) == 3) return 3;

    if(algorithm == "Controllability") return onRunMultiCtrlToolRT();
    else if(algorithm == "Nonblocking") return onRunNonBlockingToolRT();
    else if(algorithm == "BDD Nonblocking") return onRunBddNonblockingToolRT();
    else if(algorithm == "Untimed Controllability") return onRunBddControllabilityToolRT();
    else if(algorithm == "SD Controllability") return onRunBddSDControllabilityToolRT();
    else if(algorithm == "Proper Time Behavior") return onRunBddProperTimebehaviorToolRT();
    else if(algorithm == "S-Singular Prohibitable Behavior") return onRunBddSSingularProhibitableBehaviorToolRT();
    else if(algorithm == "Plant Completeness") return onRunBddPlantCompletenessToolRT();
    else if(algorithm == "Activity Loop Free") return onRunBddALFToolRT();
    else return 2;
}

int RegressionTestAlgorithms::runHierAlgo(const QString& algorithm, const QString& subsys)
{
    HierProjIntegrityAlgo integAlgo;
    if(onCheckProjectIntegrityRT(integAlgo) == 3) return 3;

    if(algorithm == "Controllability") return onRunMultiCtrlToolRT();
    else if(algorithm == "Nonblocking") return onRunNonBlockingToolRT();
    else if(algorithm == "Subsystem LD I-Consistent") return onRunSubsysIConsistToolRT(subsys);
    else if(algorithm == "Subsystem LD Controllable") return onRunSubsysCtrlToolRT(subsys);
    else if(algorithm == "Subsystem LD Nonblocking") return onRunSubsysNonblockingToolRT(subsys);
    else if(algorithm == "LD Interface Consistent") return onRunProjIConsistToolRT();
    else if(algorithm == "LD Level-Wise Controllability") return onRunLevelWiseCtrlToolRT();
    else if(algorithm == "LD Level-Wise Nonblocking") return onRunLevelWiseNonblockingToolRT();

    else if(algorithm == "BDD Controllability") return onRunBddControllabilityToolRT();
    else if(algorithm == "BDD Nonblocking") return onRunBddNonblockingToolRT();
    else if(algorithm == "BDD Subsystem LD I-Consistent") return onRunBddSubsysIConsistToolRT(subsys);
    else if(algorithm == "BDD Subsystem LD Controllable") return onRunBddSubsysCtrlToolRT(subsys);
    else if(algorithm == "BDD Subsystem LD Nonblocking") return onRunBddSubsysNonblockingToolRT(subsys);
    else if(algorithm == "BDD LD Interface Consistent") return onRunBddProjIConsistToolRT();
    else if(algorithm == "BDD LD Level-Wise Controllability") return onRunBddLevelWiseCtrlToolRT();
    else if(algorithm == "BDD LD Level-Wise Nonblocking") return onRunBddLevelWiseNonblockingToolRT();

    else return 2;
}

int RegressionTestAlgorithms::onCheckProjectIntegrityRT(ProjIntegrityAlgo& integAlgo)
{
    bool integ = m_pProject->checkIntegrity(integAlgo);
    if(!integ) return 3;
    else return 0;
}

int RegressionTestAlgorithms::onRunNonBlockingToolRT()
{
    try
    {
        MultiNonBlockingAlgo flatNonBlock;
        DesSubsystem::DesIteratorPtr hDesIt = m_pProject->createDesIterator();
        for(hDesIt->first(); hDesIt->notDone(); hDesIt->next())
        {
            const Des& hDes = hDesIt->currentItem();
            flatNonBlock.addInputDes(&hDes);
        }
        if(m_pProject->checkNonBlocking(flatNonBlock)) return 0;
        else return 1;
    }
    catch(const std::wstring e)
    {
        return 2;
    }
}

int RegressionTestAlgorithms::onRunMultiCtrlToolRT()
{
    try
    {
        MultiCtrlAlgo ctrlAlgo(m_pProject->createDesIterator(ePlantDes), m_pProject->createDesIterator(eSupervisorDes));
        if(m_pProject->checkControllable(ctrlAlgo)) return 0;
        else return 1;
    }
    catch(const std::wstring e)
    {
        return 2;
    }
}

int RegressionTestAlgorithms::onRunBddNonblockingToolRT()
{
    try
    {
        BDDSD::SD_NBChkInfo  checkInfo("Project passes Nonblocking Check");
        BddSdCheckNB(*m_pProject, checkInfo);
        if(checkInfo.m_chkPassed) return 0;
        else return 1;
    }
    catch(const std::wstring e)
    {
        return 2;
    }
}

int RegressionTestAlgorithms::onRunBddControllabilityToolRT()
{
    try
    {
        BDDSD::SD_CtrlChkInfo  checkInfo("Project passes Untimed Controllability Check");
        BddSdCheckCtrl(*m_pProject, checkInfo);
        if(checkInfo.m_chkPassed) return 0;
        else return 1;
    }
    catch(const std::wstring e)
    {
        return 2;
    }
}

int RegressionTestAlgorithms::onRunBddSDControllabilityToolRT()
{
    try
    {
        BDDSD::SD_SDCtrlChkInfo  checkInfo("Project passes SD Controllability Check");
        BddSdCheckSDCtrl(*m_pProject, checkInfo);
        if(checkInfo.m_chkPassed) return 0;
        else return 1;
    }
    catch(const std::wstring e)
    {
        return 2;
    }
}

int RegressionTestAlgorithms::onRunBddProperTimebehaviorToolRT()
{
    try
    {
        BDDSD::SD_PTBChkInfo  checkInfo("Project passes Proper Time Behavior Check");
        BddSdCheckPTB(*m_pProject, checkInfo);
        if(checkInfo.m_chkPassed) return 0;
        else return 1;
    }
    catch(const std::wstring e)
    {
        return 2;
    }
}

int RegressionTestAlgorithms::onRunBddSSingularProhibitableBehaviorToolRT()
{
    try
    {
        BDDSD::SD_SSingPChkInfo  checkInfo("Project passes S-singular Prohibitable Behavior Check");
        BddSdCheckSSingP(*m_pProject, checkInfo);
        if(checkInfo.m_chkPassed) return 0;
        else return 1;
    }
    catch(const std::wstring e)
    {
        return 2;
    }
}

int RegressionTestAlgorithms::onRunBddPlantCompletenessToolRT()
{
    try
    {
        BDDSD::SD_PCmpltChkInfo  checkInfo("Project passes Plant Completeness Check");
        BddSdCheckPCmplt(*m_pProject, checkInfo);
        if(checkInfo.m_chkPassed) return 0;
        else return 1;
    }
    catch(const std::wstring e)
    {
        return 2;
    }
}

int RegressionTestAlgorithms::onRunBddALFToolRT()
{
    try
    {
        //int tmp;
        BDDSD::SD_ALFChkInfo  checkInfo("Project passes Activity Loop Free Check");
        BddSdCheckALF(*m_pProject, checkInfo);
        if(checkInfo.m_chkPassed) return 0;
        else return 1;
    }
    catch(const std::wstring e)
    {
        return 2;
    }
}

int RegressionTestAlgorithms::onRunSubsysIConsistToolRT(const QString& subsys)
{
    try
    {
        DESpot::DesHierProject::SubsysIteratorPtr subsysIt = project()->createSubsysIterator();
        for(subsysIt->first(); subsysIt->notDone(); subsysIt->next())
        {
            if(subsysIt->currentItem().getName() == subsys.toStdWString())
            {
                m_pCrtSubsys = &subsysIt->currentItem();

                if (m_pCrtSubsys == null)
                    return 2;

                if(project()->isBilevel())
                {
                    if (m_pCrtSubsys->isRoot())
                    {
                        IConsistAnsAcceptAlgo ansAcceptAlgo(*project());
                        if(project()->checkSubsysIConsistProp(ansAcceptAlgo)) return 0;
                        else return 1;
                    }
                    else
                    {
                        LowSubsysIConsistAlgo lowSubsysIConsAlgo(*m_pCrtSubsys);
                        if(project()->checkSubsysIConsistProp(lowSubsysIConsAlgo)) return 0;
                        else return 1;
                    }
                }
                else
                {
                    if (project()->isValid() == false)
                    {
                        throw EX("Either project is not valid, or integrity has not yet been checked.");
                        return 2;
                    }
                    bool nonLeaf = false;
                    DesSubsystem::DependIteratorPtr depIt = m_pCrtSubsys->createDependsIterator();
                    for(depIt->first(); depIt->notDone(); depIt->next())
                    {
                        nonLeaf = true;
                    }
                    if(m_pCrtSubsys->isRoot())
                    {
                        DesProject* desProj = m_pProject->extractionSystem(m_pCrtSubsys);
                        DesHierProject* extractedProj = dynamic_cast<DesHierProject*>(desProj);
                        IConsistAnsAcceptAlgo ansAcceptAlgo(*extractedProj);

                        bool subIConst = extractedProj->checkSubsysIConsistProp(ansAcceptAlgo);
                        if(subIConst) return 0;
                        else return 1;
                    }
                    else if(m_pCrtSubsys->isRoot() == false && nonLeaf)
                    {
                        DesProject* desProjAsRoot = m_pProject->extractionSystem(m_pCrtSubsys);
                        DesHierProject* extractedProjAsRoot = dynamic_cast<DesHierProject*>(desProjAsRoot);
                        IConsistAnsAcceptAlgo ansAcceptAlgo(*extractedProjAsRoot);

                        if(extractedProjAsRoot->checkSubsysIConsistProp(ansAcceptAlgo)==false)
                        {
                            return 1;
                        }

                        const DesSubsystem& crtSubsysParent = m_pCrtSubsys->getInterface().getSubsystem();
                        DesProject* desProj = m_pProject->extractionSystem(&crtSubsysParent);
                        DesHierProject* extractedProj = dynamic_cast<DesHierProject*>(desProj);
                        DesSubsystem::DependIteratorPtr lowSubsysIt = extractedProj->getRootSubsys().createDependsIterator();
                        for(lowSubsysIt->first(); lowSubsysIt->isDone() == false; lowSubsysIt->next())
                        {
                            const DesSubsystem*  lowSubsys = lowSubsysIt->currentItem().subsystem;
                            if(lowSubsys->getName() == m_pCrtSubsys->getName())
                            {
                                LowSubsysIConsistAlgo lowSubsysIConsAlgo(*lowSubsys);

                                if(extractedProj->checkSubsysIConsistProp(lowSubsysIConsAlgo)==false)
                                {
                                    return 1;
                                }
                            }
                        }
                        return 0;
                    }
                    else if(nonLeaf == false)
                    {
                        const DesSubsystem& crtSubsysParent = m_pCrtSubsys->getInterface().getSubsystem();
                        DesProject* desProj = m_pProject->extractionSystem(&crtSubsysParent);
                        DesHierProject* extractedProj = dynamic_cast<DesHierProject*>(desProj);
                        DesSubsystem::DependIteratorPtr lowSubsysIt = extractedProj->getRootSubsys().createDependsIterator();
                        for(lowSubsysIt->first(); lowSubsysIt->isDone() == false; lowSubsysIt->next())
                        {
                            const DesSubsystem*  lowSubsys = lowSubsysIt->currentItem().subsystem;
                            if(lowSubsys->getName() == m_pCrtSubsys->getName())
                            {
                                LowSubsysIConsistAlgo lowSubsysIConsAlgo(*lowSubsys);

                                bool subIConst = extractedProj->checkSubsysIConsistProp(lowSubsysIConsAlgo);
                                if(subIConst) return 0;
                                else return 1;
                            }
                        }
                    }
                }
            }
        }
        return 2;
    }

    catch(const std::wstring e)
    {
        return 2;
    }
}

int RegressionTestAlgorithms::onRunSubsysNonblockingToolRT(const QString& subsys)
{
    try
    {
        DESpot::DesHierProject::SubsysIteratorPtr subsysIt = project()->createSubsysIterator();
        for(subsysIt->first(); subsysIt->notDone(); subsysIt->next())
        {
            if(subsysIt->currentItem().getName() == subsys.toStdWString())
            {
                m_pCrtSubsys = &subsysIt->currentItem();

                if (m_pCrtSubsys == null)
                    return 2;
                if(project()->isBilevel())
                {
                    if (m_pCrtSubsys->isRoot())
                    {
                        HighSubsysNonBlockAlgo hSubsysNonBlockAlgo(*project());
                        if(project()->checkSubsysNonBlockProp(hSubsysNonBlockAlgo)) return 0;
                        else return 1;
                    }
                    else
                    {
                        LowSubsysNonBlockAlgo lowSubsysNonBlockAlgo(*m_pCrtSubsys);
                        if(project()->checkSubsysNonBlockProp(lowSubsysNonBlockAlgo)) return 0;
                        else return 1;
                    }
                }
                else
                {
                    if (project()->isValid() == false)
                    {
                        throw EX("Either project is not valid, or integrity has not yet been checked.");
                        return 2;
                    }
                    bool nonLeaf = false;
                    DesSubsystem::DependIteratorPtr depIt = m_pCrtSubsys->createDependsIterator();
                    for(depIt->first(); depIt->notDone(); depIt->next())
                    {
                        nonLeaf = true;
                    }
                    if(nonLeaf)
                    {
                        DesProject* desProj = m_pProject->extractionSystem(m_pCrtSubsys);
                        DesHierProject* extractedProj = dynamic_cast<DesHierProject*>(desProj);
                        HighSubsysNonBlockAlgo hSubsysNonBlockAlgo(*extractedProj);

                        bool subNonBlock = extractedProj->checkSubsysNonBlockProp(hSubsysNonBlockAlgo);
                        if(subNonBlock) return 0;
                        else return 1;
                    }
                    else
                    {
                        const DesSubsystem& crtSubsysParent = m_pCrtSubsys->getInterface().getSubsystem();
                        DesProject* desProj = m_pProject->extractionSystem(&crtSubsysParent);
                        DesHierProject* extractedProj = dynamic_cast<DesHierProject*>(desProj);
                        DesSubsystem::DependIteratorPtr lowSubsysIt = extractedProj->getRootSubsys().createDependsIterator();
                        for(lowSubsysIt->first(); lowSubsysIt->isDone() == false; lowSubsysIt->next())
                        {
                            const DesSubsystem*  lowSubsys = lowSubsysIt->currentItem().subsystem;
                            if(lowSubsys->getName() == m_pCrtSubsys->getName())
                            {
                                LowSubsysNonBlockAlgo lowSubsysNonBlockAlgo(*lowSubsys);
                                bool subNonBlock = extractedProj->checkSubsysNonBlockProp(lowSubsysNonBlockAlgo);
                                if(subNonBlock) return 0;
                                else return 1;
                            }
                        }
                    }
                }
            }
        }
        return 2;
    }
    catch(const std::wstring e)
    {
        return 2;
    }
}

int RegressionTestAlgorithms::onRunSubsysCtrlToolRT(const QString& subsys)
{
    try
    {
        DESpot::DesHierProject::SubsysIteratorPtr subsysIt = project()->createSubsysIterator();
        for(subsysIt->first(); subsysIt->notDone(); subsysIt->next())
        {
            if(subsysIt->currentItem().getName() == subsys.toStdWString())
            {
                m_pCrtSubsys = &subsysIt->currentItem();

                if (m_pCrtSubsys == null)
                    return 2;
                if(project()->isBilevel())
                {
                    if (m_pCrtSubsys->isRoot())
                    {
                        HighSubsysCtrlAlgo hSubsysCtrlAlgo(*project());
                        if(project()->checkSubsysLwCtrlProp(hSubsysCtrlAlgo)) return 0;
                        else return 1;
                    }
                    else
                    {
                        LowSubsysCtrlAlgo lowSubsysCtrlAlgo(*m_pCrtSubsys);
                        if(project()->checkSubsysLwCtrlProp(lowSubsysCtrlAlgo)) return 0;
                        else return 1;
                    }
                }
                else
                {
                    if (project()->isValid() == false)
                    {
                        throw EX("Either project is not valid, or integrity has not yet been checked.");
                        return 2;
                    }
                    if(m_pCrtSubsys->isRoot())
                    {
                        DesProject* desProj = m_pProject->extractionSystem(m_pCrtSubsys);
                        DesHierProject* extractedProj = dynamic_cast<DesHierProject*>(desProj);
                        HighSubsysCtrlAlgo hSubsysCtrlAlgo(*extractedProj);

                        bool subContrl = extractedProj->checkSubsysLwCtrlProp(hSubsysCtrlAlgo);
                        if(subContrl) return 0;
                        else return 1;
                    }
                    else
                    {
                        const DesSubsystem& crtSubsysParent = m_pCrtSubsys->getInterface().getSubsystem();
                        DesProject* desProj = m_pProject->extractionSystem(&crtSubsysParent);
                        DesHierProject* extractedProj = dynamic_cast<DesHierProject*>(desProj);
                        DesSubsystem::DependIteratorPtr lowSubsysIt = extractedProj->getRootSubsys().createDependsIterator();
                        for(lowSubsysIt->first(); lowSubsysIt->isDone() == false; lowSubsysIt->next())
                        {
                            const DesSubsystem*  lowSubsys = lowSubsysIt->currentItem().subsystem;
                            if(lowSubsys->getName() == m_pCrtSubsys->getName())
                            {
                                LowSubsysCtrlAlgo lowSubsysCtrlAlgo(*lowSubsys);

                                bool subContrl=extractedProj->checkSubsysLwCtrlProp(lowSubsysCtrlAlgo);
                                if(subContrl) return 0;
                                else return 1;
                            }
                        }
                    }
                }
            }
        }
        return 2;
    }
    catch(const std::wstring e)
    {
        return 2;
    }
}

int RegressionTestAlgorithms::onRunProjIConsistToolRT()
{
    try
    {
        if(project()->isBilevel())
        {
            IConsistAlgo iConsAlgo(*project());
            if(project()->checkIConsistProp(iConsAlgo)) return 0;
            else return 1;
        }
        //this conditions is just to verify that the multi-level project passes integrity test before creating extraction systems on it.
        //It is called againg the iConsAlgo so that it sets the appropriate flags and give appropriate output.
        else if(project()->isBilevel()==false && project()->isValid()==false)
        {
            IConsistAlgo iConsAlgo(*project());
            if(project()->checkIConsistProp(iConsAlgo)) return 0;
            else return 1;
        }
        else
        {
            std::map<std::wstring, bool> subsysIConstMap;
            std::map<std::wstring, bool>::iterator subsysIConstMapIt;
            DesHierProject::SubsysIteratorPtr subsysIt = project()->createSubsysIterator();
            for(subsysIt->first(); subsysIt->notDone(); subsysIt->next())
            {
                bool nonLeaf = false;
                DesSubsystem::DependIteratorPtr depIt = subsysIt->currentItem().createDependsIterator();
                for(depIt->first(); depIt->notDone(); depIt->next())
                {
                    nonLeaf = true;
                }
                //call extraction against non-leaf node only
                if(nonLeaf)
                {
                    const DesSubsystem& subsysExtraction = subsysIt->currentItem();
                    DesProject* desProj = m_pProject->extractionSystem(&subsysExtraction);
                    DesHierProject* extractedProj = dynamic_cast<DesHierProject*>(desProj);
                    IConsistAlgo iConsAlgo(*extractedProj);

                    bool isIcons = extractedProj->checkIConsistProp(iConsAlgo,subsysIConstMap,project());
                    if(isIcons == false)
                    {
                        DesHierProject::SubsysIteratorPtr subsysMainProjIt = project()->createSubsysIterator();
                        for(subsysMainProjIt->first(); subsysMainProjIt->notDone(); subsysMainProjIt->next())
                        {
                            const DesSubsystem& subMainProj = subsysMainProjIt->currentItem();
                            subsysIConstMapIt = subsysIConstMap.find(subMainProj.getName());
                            if (subsysIConstMapIt != subsysIConstMap.end())
                            {
                                bool isNonLeaf = false;
                                DesSubsystem::DependIteratorPtr depSubIt = subMainProj.createDependsIterator();
                                for(depSubIt->first(); depSubIt->notDone(); depSubIt->next())
                                {
                                    isNonLeaf = true;
                                }
                                if(subMainProj.isRoot() || isNonLeaf==false)
                                    subMainProj.setInterfConsist(subsysIConstMapIt->second);
                            }
                        }
                        DesHierProject::InterfIteratorPtr intrefAllIt = project()->createInterfIterator();
                        for(intrefAllIt->first(); intrefAllIt->notDone(); intrefAllIt->next())
                        {
                            const DesInterface& interfMainProj = intrefAllIt->currentItem();
                            DesInterface& interfNc= const_cast<DesInterface &>(interfMainProj);

                            subsysIConstMapIt = subsysIConstMap.find(interfMainProj.getName());
                            if (subsysIConstMapIt != subsysIConstMap.end())
                                interfNc.setInterfConsist(subsysIConstMapIt->second);
                        }
                        return 1;
                    }
                }
            }
            DesHierProject::SubsysIteratorPtr subsysAllIt = project()->createSubsysIterator();
            for(subsysAllIt->first(); subsysAllIt->notDone(); subsysAllIt->next())
            {
                const DesSubsystem& subsysMainProj = subsysAllIt->currentItem();
                DesSubsystem& subsysNc= const_cast<DesSubsystem &>(subsysMainProj);
                subsysNc.setInterfConsist(true);
            }
            DesHierProject::InterfIteratorPtr intrefAllIt = project()->createInterfIterator();
            for(intrefAllIt->first(); intrefAllIt->notDone(); intrefAllIt->next())
            {
                const DesInterface& interfMainProj = intrefAllIt->currentItem();
                DesInterface& interfNc= const_cast<DesInterface &>(interfMainProj);
                interfNc.setInterfConsist(true);
            }
            return 0;
        }
    }
    catch(const std::wstring e)
    {
        return 2;
    }
}

int RegressionTestAlgorithms::onRunLevelWiseNonblockingToolRT()
{
    try
    {
        if(project()->isBilevel())
        {
            LwNonBlockingAlgo lwNonBlockAlgo(*project());
            if(project()->checkLwNonBlockProp(lwNonBlockAlgo)) return 0;
            else return 1;
        }
        //this conditions is just to verify that the multi-level project passes integrity test before creating extraction systems on it.
        //It is called against the lwNonBlocAlgo so that it sets the appropriate flags and give appropriate output.
        else if(project()->isBilevel()==false && project()->isValid()==false)
        {
            LwNonBlockingAlgo lwNonBlockAlgo(*project());
            if(project()->checkLwNonBlockProp(lwNonBlockAlgo)) return 0;
            else return 1;
        }
        else
        {
            std::map<std::wstring, bool> subsysNonBlockMap;
            std::map<std::wstring, bool>::iterator subsysNonBlockMapIt;
            DesHierProject::SubsysIteratorPtr subsysIt = project()->createSubsysIterator();
            for(subsysIt->first(); subsysIt->notDone(); subsysIt->next())
            {
                bool nonLeaf = false;
                DesSubsystem::DependIteratorPtr depIt = subsysIt->currentItem().createDependsIterator();
                for(depIt->first(); depIt->notDone(); depIt->next())
                {
                    nonLeaf = true;
                }
                //call extraction against non-leaf node only
                if(nonLeaf)
                {
                    const DesSubsystem& subsysExtraction = subsysIt->currentItem();
                    DesProject* desProj = project()->extractionSystem(&subsysExtraction);
                    DesHierProject* extractedProj = dynamic_cast<DesHierProject*>(desProj);
                    LwNonBlockingAlgo lwNonBlockAlgo(*extractedProj);

                    bool isNonBloc = extractedProj->checkLwNonBlockProp(lwNonBlockAlgo,subsysNonBlockMap);

                    if(isNonBloc==false)
                    {
                        DesHierProject::SubsysIteratorPtr subsysNonBlockIt = project()->createSubsysIterator();
                        for(subsysNonBlockIt->first(); subsysNonBlockIt->notDone(); subsysNonBlockIt->next())
                        {
                            subsysNonBlockMapIt = subsysNonBlockMap.find(subsysNonBlockIt->currentItem().getName());
                            if (subsysNonBlockMapIt != subsysNonBlockMap.end())
                            {
                                subsysNonBlockIt->currentItem().setNonBlocking(subsysNonBlockMapIt->second);
                            }
                        }
                        return 1;
                    }
                }
            }
            DesHierProject::SubsysIteratorPtr subsysIt1 = project()->createSubsysIterator();
            for(subsysIt1->first(); subsysIt1->notDone(); subsysIt1->next())
            {
                const DesSubsystem& subsysMainProj = subsysIt1->currentItem();
                DesSubsystem& subsysNc= const_cast<DesSubsystem &>(subsysMainProj);
                subsysNc.setNonBlocking(true);
            }
            return 0;
        }
    }
    catch(const std::wstring e)
    {
        return 2;
    }
}

int RegressionTestAlgorithms::onRunLevelWiseCtrlToolRT()
{
    try
    {
        if(project()->isBilevel())
        {
            LwCtrlAlgo lwCtrlAlgo(*project());
            if(project()->checkLwCtrlProp(lwCtrlAlgo)) return 0;
            else return 1;
        }
        //this conditions is just to verify that the multi-level project passes integrity test before creating extraction systems on it.
        //It is called againg the loCtrlAlgo so that it sets the appropriate flags and give appropriate output.
        else if(project()->isBilevel()==false && project()->isValid()==false)
        {
            LwCtrlAlgo lwCtrlAlgo(*project());
            if(project()->checkLwCtrlProp(lwCtrlAlgo)) return 0;
            else return 1;
        }
        else
        {
            std::map<std::wstring, bool> subsysCtrlMap;
            std::map<std::wstring, bool>::iterator subsysCtrlMapIt;
            DesHierProject::SubsysIteratorPtr subsysIt = project()->createSubsysIterator();
            for(subsysIt->first(); subsysIt->notDone(); subsysIt->next())
            {
                bool nonLeaf = false;
                DesSubsystem::DependIteratorPtr depIt = subsysIt->currentItem().createDependsIterator();
                for(depIt->first(); depIt->notDone(); depIt->next())
                {
                    nonLeaf = true;
                }
                //call extraction against non-leaf node only
                if(nonLeaf)
                {
                    const DesSubsystem& subsysExtraction = subsysIt->currentItem();
                    DesProject* desProj = project()->extractionSystem(&subsysExtraction);
                    DesHierProject* extractedProj = dynamic_cast<DesHierProject*>(desProj);
                    LwCtrlAlgo lwCtrlAlgo(*extractedProj);

                    extractedProj->checkLwCtrlProp(lwCtrlAlgo,subsysCtrlMap);

                    if(extractedProj->isLwControllable()==false)
                    {
                        DesHierProject::SubsysIteratorPtr subsysNonBlockIt = project()->createSubsysIterator();
                        for(subsysNonBlockIt->first(); subsysNonBlockIt->notDone(); subsysNonBlockIt->next())
                        {
                            subsysCtrlMapIt = subsysCtrlMap.find(subsysNonBlockIt->currentItem().getName());
                            if (subsysCtrlMapIt != subsysCtrlMap.end())
                            {
                                subsysNonBlockIt->currentItem().setControllable(subsysCtrlMapIt->second);
                            }

                        }
                        return 1;
                    }
                }
            }
            DesHierProject::SubsysIteratorPtr subsysIt1 = project()->createSubsysIterator();
            for(subsysIt1->first(); subsysIt1->notDone(); subsysIt1->next())
            {
                const DesSubsystem& subsysMainProj = subsysIt1->currentItem();
                DesSubsystem& subsysNc= const_cast<DesSubsystem &>(subsysMainProj);
                subsysNc.setControllable(true);
            }
            return 0;
        }
    }
    catch(const std::wstring e)
    {
        return 2;
    }
}

int RegressionTestAlgorithms::onRunBddSubsysIConsistToolRT(const QString& subsys)
{
    try
    {
        DESpot::DesHierProject::SubsysIteratorPtr subsysIt = project()->createSubsysIterator();
        for(subsysIt->first(); subsysIt->notDone(); subsysIt->next())
        {
            if(subsysIt->currentItem().getName() == subsys.toStdWString())
            {
                m_pCrtSubsys = &subsysIt->currentItem();

                if (m_pCrtSubsys == null)
                {
                    // no subsystem selected
                    return 2;
                }

                int tmp;

                // need to determine which subsystem to verify
                QString DESpotSubName = QString::fromStdWString(m_pCrtSubsys->getName());
                QString subSysName;
                bool isHigh = false;

                if (m_pCrtSubsys->isRoot())
                {
                    isHigh = true;
                }

                subSysName = QString::fromStdWString(m_pCrtSubsys->getName());

                if(project()->isBilevel())
                {
                    //check only returns here if a pass/fail result is available
                    // need to set appropriate flags
                    if (isHigh)
                    {
                        BDDHISC::Hisc_SubIConsisChkInfo checkInfo("High Level subsystem '" + DESpotSubName +
                                                                  "' is LD interface consistent.", DESpotSubName);
                        tmp = BDDHISC::BddHiscCheckIConsist(*m_pProject, subSysName,
                                                            isHigh, checkInfo);
                        if(checkInfo.m_chkPassed) return 0;
                        else return 1;
                    }
                    else
                    {
                        BDDHISC::Hisc_SubIConsisChkInfo checkInfo("Low Level subsystem '" + DESpotSubName +
                                                                  "' is LD interface consistent.", DESpotSubName);
                        tmp = BDDHISC::BddHiscCheckIConsist(*m_pProject, subSysName,
                                                            isHigh, checkInfo);
                        if(checkInfo.m_chkPassed) return 0;
                        else return 1;
                    }
                }
                else
                {
                    if (project()->isValid() == false)
                    {
                        throw EX("Project integrity test must pass before running BDD algorithms.");
                        return 2;
                    }

                    bool nonLeaf= false;
                    DesSubsystem::DependIteratorPtr depIt = m_pCrtSubsys->createDependsIterator();
                    for(depIt->first(); depIt->notDone(); depIt->next())
                    {
                        nonLeaf = true;
                    }

                    if (isHigh)
                    {
                        DesProject* desProj = m_pProject->extractionSystem(m_pCrtSubsys);
                        //DesHierProject* extractedProj = dynamic_cast<DesHierProject*>(desProj);

                        BDDHISC::Hisc_SubIConsisChkInfo checkInfo("High Level subsystem '" + DESpotSubName +
                                                                  "' is LD interface consistent.", DESpotSubName);
                        tmp = BDDHISC::BddHiscCheckIConsist(*desProj, subSysName,
                                                            isHigh, checkInfo);
                        if(checkInfo.m_chkPassed) return 0;
                        else return 1;
                    }
                    else if(isHigh==false && nonLeaf)
                    {
                        DesProject* desProjForHigh = m_pProject->extractionSystem(m_pCrtSubsys);

                        BDDHISC::Hisc_SubIConsisChkInfo checkInfo("High/Low Level subsystem '" + DESpotSubName +
                                                                  "' is LD interface consistent.", DESpotSubName);
                        tmp = BDDHISC::BddHiscCheckIConsist(*desProjForHigh, subSysName,
                                                            nonLeaf, checkInfo);
                        bool isHighICons=checkInfo.m_chkPassed;
                        if(isHighICons==false)
                        {
                            return 1;
                        }

                        const DesSubsystem& crtSubsysParent = m_pCrtSubsys->getInterface().getSubsystem();
                        DesProject* desProjForLow = m_pProject->extractionSystem(&crtSubsysParent);
                        DesHierProject* extractedProj = dynamic_cast<DesHierProject*>(desProjForLow);
                        DesSubsystem::DependIteratorPtr lowSubsysIt = extractedProj->getRootSubsys().createDependsIterator();
                        for(lowSubsysIt->first(); lowSubsysIt->isDone() == false; lowSubsysIt->next())
                        {
                            const DesSubsystem*  lowSubsys = lowSubsysIt->currentItem().subsystem;
                            if(lowSubsys->getName() == m_pCrtSubsys->getName())
                            {
                                BDDHISC::Hisc_SubIConsisChkInfo checkInfo("High/Low Level subsystem '" + DESpotSubName +
                                                                          "' is LD interface consistent.", DESpotSubName);
                                tmp = BDDHISC::BddHiscCheckIConsist(*desProjForLow, subSysName,
                                                                    isHigh, checkInfo);
                                bool isLowICons=checkInfo.m_chkPassed;
                                if(isLowICons==false)
                                {
                                    return 1;
                                }
                            }
                        }
                        return 0;
                    }
                    else if(nonLeaf==false)
                    {
                        const DesSubsystem& crtSubsysParent = m_pCrtSubsys->getInterface().getSubsystem();
                        DesProject* desProj = m_pProject->extractionSystem(&crtSubsysParent);
                        DesHierProject* extractedProj = dynamic_cast<DesHierProject*>(desProj);
                        DesSubsystem::DependIteratorPtr lowSubsysIt = extractedProj->getRootSubsys().createDependsIterator();
                        for(lowSubsysIt->first(); lowSubsysIt->isDone() == false; lowSubsysIt->next())
                        {
                            const DesSubsystem*  lowSubsys = lowSubsysIt->currentItem().subsystem;
                            if(lowSubsys->getName() == m_pCrtSubsys->getName())
                            {
                                BDDHISC::Hisc_SubIConsisChkInfo checkInfo("Low Level subsystem '" + DESpotSubName +
                                                                          "' is LD interface consistent.", DESpotSubName);
                                tmp = BDDHISC::BddHiscCheckIConsist(*desProj, subSysName,
                                                                    isHigh, checkInfo);
                                if(desProj->getRootSubsys().isInterfConsist()) return 0;
                                else return 1;
                            }
                        }
                    }
                }
            }
        }
        return 2;
    }

    catch(const std::wstring e)
    {
        return 2;
    }
}

int RegressionTestAlgorithms::onRunBddSubsysNonblockingToolRT(const QString& subsys)
{
    try
    {
        DESpot::DesHierProject::SubsysIteratorPtr subsysIt = project()->createSubsysIterator();
        for(subsysIt->first(); subsysIt->notDone(); subsysIt->next())
        {
            if(subsysIt->currentItem().getName() == subsys.toStdWString())
            {
                m_pCrtSubsys = &subsysIt->currentItem();

                if (m_pCrtSubsys == null)
                {
                    // no subsystem selected
                    return 2;
                }

                int tmp;

                // need to determine which subsystem to verify
                QString DESpotSubName = QString::fromStdWString(m_pCrtSubsys->getName());
                QString subSysName;
                bool isHigh = false;

                DesSubsystem::DependIteratorPtr depIt = m_pCrtSubsys->createDependsIterator();
                for(depIt->first(); depIt->notDone(); depIt->next())
                {
                    isHigh = true;
                }

                subSysName = QString::fromStdWString(m_pCrtSubsys->getName());

                if(project()->isBilevel())
                {
                    //check only returns here if a pass/fail result is available
                    // need to set appropriate flags
                    if (isHigh)
                    {
                        BDDHISC::Hisc_SubLWNBChkInfo checkInfo("High level subsystem '" + DESpotSubName +
                                                               "' is LD nonblocking.", DESpotSubName);
                        tmp = BDDHISC::BddHiscCheckLWNonBlk(*m_pProject, subSysName,
                                                            isHigh, checkInfo);
                        if(checkInfo.m_chkPassed) return 0;
                        else return 1;
                    }
                    else
                    {
                        BDDHISC::Hisc_SubLWNBChkInfo checkInfo("Low level subsystem '" + DESpotSubName +
                                                               "' is LD nonblocking.", DESpotSubName);
                        tmp = BDDHISC::BddHiscCheckLWNonBlk(*m_pProject, subSysName,
                                                            isHigh, checkInfo);
                        if(checkInfo.m_chkPassed) return 0;
                        else return 1;
                    }
                }

                else
                {
                    if (project()->isValid() == false)
                    {
                        throw EX("Project integrity test must pass before running BDD algorithms.");
                        return 2;
                    }

                    if (isHigh)
                    {
                        DesProject* desProj = m_pProject->extractionSystem(m_pCrtSubsys);
                        //DesHierProject* extractedProj = dynamic_cast<DesHierProject*>(desProj);

                        BDDHISC::Hisc_SubLWNBChkInfo checkInfo("High level subsystem '" + DESpotSubName +
                                                               "' is LD nonblocking.", DESpotSubName);
                        tmp = BDDHISC::BddHiscCheckLWNonBlk(*desProj, subSysName,
                                                            isHigh, checkInfo);
                        if(checkInfo.m_chkPassed) return 0;
                        else return 1;
                    }
                    else
                    {
                        const DesSubsystem& crtSubsysParent = m_pCrtSubsys->getInterface().getSubsystem();
                        DesProject* desProj = m_pProject->extractionSystem(&crtSubsysParent);
                        DesHierProject* extractedProj = dynamic_cast<DesHierProject*>(desProj);
                        DesSubsystem::DependIteratorPtr lowSubsysIt = extractedProj->getRootSubsys().createDependsIterator();
                        for(lowSubsysIt->first(); lowSubsysIt->isDone() == false; lowSubsysIt->next())
                        {
                            const DesSubsystem*  lowSubsys = lowSubsysIt->currentItem().subsystem;
                            if(lowSubsys->getName() == m_pCrtSubsys->getName())
                            {
                                BDDHISC::Hisc_SubLWNBChkInfo checkInfo("Low level subsystem '" + DESpotSubName +
                                                                       "' is LD nonblocking.", DESpotSubName);
                                tmp = BDDHISC::BddHiscCheckLWNonBlk(*desProj, subSysName,
                                                                    isHigh, checkInfo);
                                if(checkInfo.m_chkPassed) return 0;
                                else return 1;
                            }
                        }
                    }
                }
            }
        }
        return 2;
    }

    catch(const std::wstring e)
    {
        return 2;
    }
}

int RegressionTestAlgorithms::onRunBddSubsysCtrlToolRT(const QString& subsys)
{
    try
    {
        DESpot::DesHierProject::SubsysIteratorPtr subsysIt = project()->createSubsysIterator();
        for(subsysIt->first(); subsysIt->notDone(); subsysIt->next())
        {
            if(subsysIt->currentItem().getName() == subsys.toStdWString())
            {
                m_pCrtSubsys = &subsysIt->currentItem();

                if (m_pCrtSubsys == null)
                {
                    // no subsystem selected
                    return 2;
                }

                int tmp;

                // need to determine which subsystem to verify
                QString DESpotSubName = QString::fromStdWString(m_pCrtSubsys->getName());

                QString subSysName;
                bool isHigh = false;

                if (m_pCrtSubsys->isRoot())
                {
                    isHigh = true;
                }

                subSysName = QString::fromStdWString(m_pCrtSubsys->getName());

                if(project()->isBilevel())
                {
                    //check only returns here if a pass/fail result is available
                    // need to set appropriate flags
                    if (isHigh)
                    {
                        BDDHISC::Hisc_SubLWContChkInfo checkInfo("High level subsystem '" + DESpotSubName +
                                                                 "' is LD controllable.", DESpotSubName);

                        tmp = BDDHISC::BddHiscCheckLWCtrl(*m_pProject, subSysName,
                                                          isHigh, checkInfo);
                        if(checkInfo.m_chkPassed) return 0;
                        else return 1;
                    }
                    else
                    {
                        BDDHISC::Hisc_SubLWContChkInfo checkInfo("Low level subsystem '" + DESpotSubName +
                                                                 "' is LD controllable.", DESpotSubName);

                        tmp = BDDHISC::BddHiscCheckLWCtrl(*m_pProject, subSysName,
                                                          isHigh, checkInfo);
                        if(checkInfo.m_chkPassed) return 0;
                        else return 1;
                    }
                }
                else
                {
                    if (project()->isValid() == false)
                    {
                        throw EX("Project integrity test must pass before running BDD algorithms.");
                        return 2;
                    }

                    if (isHigh)
                    {
                        DesProject* desProj = m_pProject->extractionSystem(m_pCrtSubsys);
                        //DesHierProject* extractedProj = dynamic_cast<DesHierProject*>(desProj);

                        BDDHISC::Hisc_SubLWContChkInfo checkInfo("High level subsystem '" + DESpotSubName +
                                                                 "' is LD controllable.", DESpotSubName);

                        tmp = BDDHISC::BddHiscCheckLWCtrl(*desProj, subSysName,
                                                          isHigh, checkInfo);
                        if(checkInfo.m_chkPassed) return 0;
                        else return 1;
                    }
                    else
                    {
                        const DesSubsystem& crtSubsysParent = m_pCrtSubsys->getInterface().getSubsystem();
                        DesProject* desProj = m_pProject->extractionSystem(&crtSubsysParent);
                        DesHierProject* extractedProj = dynamic_cast<DesHierProject*>(desProj);
                        DesSubsystem::DependIteratorPtr lowSubsysIt = extractedProj->getRootSubsys().createDependsIterator();
                        for(lowSubsysIt->first(); lowSubsysIt->isDone() == false; lowSubsysIt->next())
                        {
                            const DesSubsystem*  lowSubsys = lowSubsysIt->currentItem().subsystem;
                            if(lowSubsys->getName() == m_pCrtSubsys->getName())
                            {
                                BDDHISC::Hisc_SubLWContChkInfo checkInfo("Low level subsystem '" + DESpotSubName +
                                                                         "' is LD controllable.", DESpotSubName);

                                tmp = BDDHISC::BddHiscCheckLWCtrl(*desProj, subSysName,
                                                                  isHigh, checkInfo);
                                if(checkInfo.m_chkPassed) return 0;
                                else return 1;
                            }
                        }
                    }
                }
            }
        }
        return 2;
    }

    catch(const std::wstring e)
    {
        return 2;
    }
}

int RegressionTestAlgorithms::onRunBddProjIConsistToolRT()
{
    try
    {
        int tmp;

        // set to process all susbsytems
        QString subSysName =  "all!";
        // isHigh will be ignored
        bool isHigh = false;

        BDDHISC::Hisc_IConsisChkInfo checkInfo("Project is LD interface consistent.");
        if(project()->isBilevel())
        {
            tmp = BDDHISC::BddHiscCheckIConsist(*m_pProject, subSysName,
                isHigh, checkInfo);

            //check only returns here if a pass/fail result is available
            // need to set appropriate flags
            DesHierProject::SubsysIteratorPtr subsysAllIt = project()->createSubsysIterator();
            for(subsysAllIt->first(); subsysAllIt->notDone(); subsysAllIt->next())
            {
                const DesSubsystem& subsysMainProj = subsysAllIt->currentItem();
                DesSubsystem& subsysNc= const_cast<DesSubsystem &>(subsysMainProj);
                subsysNc.setInterfConsist(checkInfo.m_chkPassed);
            }
            DesHierProject::InterfIteratorPtr intrefAllIt = project()->createInterfIterator();
            for(intrefAllIt->first(); intrefAllIt->notDone(); intrefAllIt->next())
            {
                const DesInterface& interfMainProj = intrefAllIt->currentItem();
                DesInterface& interfNc= const_cast<DesInterface &>(interfMainProj);
                interfNc.setInterfConsist(checkInfo.m_chkPassed);
            }
            if(checkInfo.m_chkPassed) return 0;
            else return 1;
        }
        else if(project()->isBilevel()==false && project()->isValid()==false)
        {
            tmp = BDDHISC::BddHiscCheckIConsist(*m_pProject, subSysName,
                isHigh, checkInfo);

            //check only returns here if a pass/fail result is available
            // need to set appropriate flags
            if(checkInfo.m_chkPassed) return 0;
            else return 1;
        }
        else
        {
            DesHierProject::SubsysIteratorPtr subsysIt = project()->createSubsysIterator();
            for(subsysIt->first(); subsysIt->notDone(); subsysIt->next())
            {
                bool nonLeaf = false;
                DesSubsystem::DependIteratorPtr depIt = subsysIt->currentItem().createDependsIterator();
                for(depIt->first(); depIt->notDone(); depIt->next())
                {
                    nonLeaf = true;
                }
                //call extraction against non-leaf node only
                if(nonLeaf)
                {
                    const DesSubsystem& subsysExtraction = subsysIt->currentItem();
                    DesProject* desProj = m_pProject->extractionSystem(&subsysExtraction);
                    //DesHierProject* extractedProj = dynamic_cast<DesHierProject*>(desProj);
                    tmp = BDDHISC::BddHiscCheckIConsist(*desProj, subSysName,
                        isHigh, checkInfo);

                    //check only returns here if a pass/fail result is available
                    // need to set appropriate flags
                    if(checkInfo.m_chkPassed==false)
                    {
                        DesHierProject::SubsysIteratorPtr subsysAllIt = project()->createSubsysIterator();
                        for(subsysAllIt->first(); subsysAllIt->notDone(); subsysAllIt->next())
                        {
                            const DesSubsystem& subsysMainProj = subsysAllIt->currentItem();
                            DesSubsystem& subsysNc= const_cast<DesSubsystem &>(subsysMainProj);
                            subsysNc.setInterfConsist(false);
                        }
                        DesHierProject::InterfIteratorPtr intrefAllIt = project()->createInterfIterator();
                        for(intrefAllIt->first(); intrefAllIt->notDone(); intrefAllIt->next())
                        {
                            const DesInterface& interfMainProj = intrefAllIt->currentItem();
                            DesInterface& interfNc= const_cast<DesInterface &>(interfMainProj);
                            interfNc.setInterfConsist(false);
                        }
                        return 1;
                    }
                }
            }
            DesHierProject::SubsysIteratorPtr subsysAllIt = project()->createSubsysIterator();
            for(subsysAllIt->first(); subsysAllIt->notDone(); subsysAllIt->next())
            {
                const DesSubsystem& subsysMainProj = subsysAllIt->currentItem();
                DesSubsystem& subsysNc= const_cast<DesSubsystem &>(subsysMainProj);
                subsysNc.setInterfConsist(true);
            }
            DesHierProject::InterfIteratorPtr intrefAllIt = project()->createInterfIterator();
            for(intrefAllIt->first(); intrefAllIt->notDone(); intrefAllIt->next())
            {
                const DesInterface& interfMainProj = intrefAllIt->currentItem();
                DesInterface& interfNc= const_cast<DesInterface &>(interfMainProj);
                interfNc.setInterfConsist(true);
            }
            return 0;
        }
    }
    catch(const std::wstring e)
    {
        return 2;
    }
}

int RegressionTestAlgorithms::onRunBddLevelWiseNonblockingToolRT()
{
    try
    {
        int tmp;

        // set to process all susbsytems
        QString subSysName =  "all!";
        // isHigh will be ignored
        bool isHigh = false;

        BDDHISC::Hisc_LWNBChkInfo checkInfo("Project is LD level-wise nonblocking.");

        if(project()->isBilevel())
        {
            tmp = BDDHISC::BddHiscCheckLWNonBlk(*m_pProject, subSysName,
                isHigh, checkInfo);

            // set appropriate flags for des of all subsytems
            DesHierProject::SubsysIteratorPtr subsysIt1 = project()->createSubsysIterator();
            for(subsysIt1->first(); subsysIt1->notDone(); subsysIt1->next())
            {
                const DesSubsystem& subsysMainProj = subsysIt1->currentItem();
                DesSubsystem& subsysNc= const_cast<DesSubsystem &>(subsysMainProj);
                subsysNc.setNonBlocking(checkInfo.m_chkPassed);
            }
            //check only returns here if a pass/fail result is available
            // need to set appropriate flags
            if(checkInfo.m_chkPassed) return 0;
            else return 1;
        }
        else if(project()->isBilevel()==false && project()->isValid()==false)
        {
            tmp = BDDHISC::BddHiscCheckLWNonBlk(*m_pProject, subSysName,
                isHigh, checkInfo);

            //check only returns here if a pass/fail result is available
            // need to set appropriate flags
            if(checkInfo.m_chkPassed) return 0;
            else return 1;
        }
        else
        {
            DesHierProject::SubsysIteratorPtr subsysIt = project()->createSubsysIterator();
            for(subsysIt->first(); subsysIt->notDone(); subsysIt->next())
            {
                bool nonLeaf = false;
                DesSubsystem::DependIteratorPtr depIt = subsysIt->currentItem().createDependsIterator();
                for(depIt->first(); depIt->notDone(); depIt->next())
                {
                    nonLeaf = true;
                }
                //call extraction against non-leaf node only
                if(nonLeaf)
                {
                    const DesSubsystem& subsysExtraction = subsysIt->currentItem();
                    DesProject* desProj = project()->extractionSystem(&subsysExtraction);
                    //DesHierProject* extractedProj = dynamic_cast<DesHierProject*>(desProj);

                    tmp = BDDHISC::BddHiscCheckLWNonBlk(*desProj, subSysName,
                        isHigh, checkInfo);

                    if(checkInfo.m_chkPassed==false)
                    {
                        //set non blocking flag for all the des in the main project to false
                        DesHierProject::SubsysIteratorPtr subsysIt1 = project()->createSubsysIterator();
                        for(subsysIt1->first(); subsysIt1->notDone(); subsysIt1->next())
                        {
                            const DesSubsystem& subsysMainProj = subsysIt1->currentItem();
                            DesSubsystem& subsysNc= const_cast<DesSubsystem &>(subsysMainProj);
                            subsysNc.setNonBlocking(false);
                        }
                        return 1;
                    }
                }
            }
            DesHierProject::SubsysIteratorPtr subsysIt1 = project()->createSubsysIterator();
            for(subsysIt1->first(); subsysIt1->notDone(); subsysIt1->next())
            {
                const DesSubsystem& subsysMainProj = subsysIt1->currentItem();
                DesSubsystem& subsysNc= const_cast<DesSubsystem &>(subsysMainProj);
                subsysNc.setNonBlocking(true);
            }
            return 0;
        }
    }
    catch(const std::wstring e)
    {
        return 2;
    }
}

int RegressionTestAlgorithms::onRunBddLevelWiseCtrlToolRT()
{
    try
    {
        int tmp;

        // set to process all susbsytems
        QString subSysName =  "all!";
        // isHigh will be ignored
        bool isHigh = false;

        BDDHISC::Hisc_LWContChkInfo checkInfo("Project is LD level-wise controllable.");
        if(project()->isBilevel())
        {
            tmp = BDDHISC::BddHiscCheckLWCtrl(*m_pProject, subSysName,
                isHigh, checkInfo);

            //check only returns here if a pass/fail result is available
            // need to set appropriate flags
            DesHierProject::SubsysIteratorPtr subsysIt1 = project()->createSubsysIterator();
            for(subsysIt1->first(); subsysIt1->notDone(); subsysIt1->next())
            {
                const DesSubsystem& subsysMainProj = subsysIt1->currentItem();
                DesSubsystem& subsysNc= const_cast<DesSubsystem &>(subsysMainProj);
                subsysNc.setControllable(checkInfo.m_chkPassed);
            }
            if(checkInfo.m_chkPassed) return 0;
            else return 1;
        }
        else if(project()->isBilevel()== false && project()->isValid()==false)
        {
            tmp = BDDHISC::BddHiscCheckLWCtrl(*m_pProject, subSysName,
                isHigh, checkInfo);
            if(checkInfo.m_chkPassed) return 0;
            else return 1;
        }
        else
        {
            DesHierProject::SubsysIteratorPtr subsysIt = project()->createSubsysIterator();
            for(subsysIt->first(); subsysIt->notDone(); subsysIt->next())
            {
                bool nonLeaf = false;
                DesSubsystem::DependIteratorPtr depIt = subsysIt->currentItem().createDependsIterator();
                for(depIt->first(); depIt->notDone(); depIt->next())
                {
                    nonLeaf = true;
                }
                //call extraction against non-leaf node only
                if(nonLeaf)
                {
                    const DesSubsystem& subsysExtraction = subsysIt->currentItem();
                    DesProject* desProj = project()->extractionSystem(&subsysExtraction);
                    //DesHierProject* extractedProj = dynamic_cast<DesHierProject*>(desProj);
                    tmp = BDDHISC::BddHiscCheckLWCtrl(*desProj, subSysName,
                        isHigh, checkInfo);

                    if(checkInfo.m_chkPassed==false)
                    {
                        DesHierProject::SubsysIteratorPtr subsysIt1 = project()->createSubsysIterator();
                        for(subsysIt1->first(); subsysIt1->notDone(); subsysIt1->next())
                        {
                            const DesSubsystem& subsysMainProj = subsysIt1->currentItem();
                            DesSubsystem& subsysNc= const_cast<DesSubsystem &>(subsysMainProj);
                            subsysNc.setControllable(false);
                        }
                        return 1;
                    }
                }
            }
            DesHierProject::SubsysIteratorPtr subsysIt1 = project()->createSubsysIterator();
            for(subsysIt1->first(); subsysIt1->notDone(); subsysIt1->next())
            {
                const DesSubsystem& subsysMainProj = subsysIt1->currentItem();
                DesSubsystem& subsysNc= const_cast<DesSubsystem &>(subsysMainProj);
                subsysNc.setControllable(true);
            }
            return 0;
        }
    }
    catch(const std::wstring e)
    {
        return 2;
    }
}

} //end of namespace DESpot

