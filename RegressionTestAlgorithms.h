/*	Author:		Jacob Howard
    Supervisor: Dr. Ryan Leduc

    Project created in conformity with the requirements for the Degree of Master of Engineering in Software Engineering,
    Computing and Software Department,
    McMaster University
    2017
*/

#ifndef REGRESSIONTESTALGORITHMS_H
#define REGRESSIONTESTALGORITHMS_H

#include <QMap>
#include <QStringList>

namespace DESpot
{

class MainForm;
class EventPoolForm;
class ProjectWorkspace;
class DesProject;
class Des;
class CtrlStdAlgo;
class MultiCtrlAlgo;
class OutputUiPart;
class ProjIntegrityAlgo;
class UpdateProgressInterface;
class Instantiation;
class DesFlatProject;
class DesHierProject;
class DesSubsystem;
class DesInterface;

class RegressionTestAlgorithms
{
public:
    RegressionTestAlgorithms(DESpot::DesProject* project);
    int runFlatAlgo(const QString& algorithm);
    int runHierAlgo(const QString& algorithm, const QString& subsys);
    virtual ~RegressionTestAlgorithms(void);

protected:
    int onCheckProjectIntegrityRT(ProjIntegrityAlgo& integAlgo);
    int onRunNonBlockingToolRT();
    int onRunMultiCtrlToolRT();
    int onRunBddNonblockingToolRT();
    int onRunBddControllabilityToolRT();
    int onRunBddSDControllabilityToolRT();
    int onRunBddProperTimebehaviorToolRT();
    int onRunBddSSingularProhibitableBehaviorToolRT();
    int onRunBddPlantCompletenessToolRT();
    int onRunBddALFToolRT();
    int onRunSubsysIConsistToolRT(const QString& subsys);
    int onRunSubsysNonblockingToolRT(const QString& subsys);
    int onRunSubsysCtrlToolRT(const QString& subsys);
    int onRunProjIConsistToolRT();
    int onRunLevelWiseNonblockingToolRT();
    int onRunLevelWiseCtrlToolRT();
    int onRunBddSubsysIConsistToolRT(const QString& subsys);
    int onRunBddSubsysNonblockingToolRT(const QString& subsys);
    int onRunBddSubsysCtrlToolRT(const QString& subsys);
    int onRunBddProjIConsistToolRT();
    int onRunBddLevelWiseNonblockingToolRT();
    int onRunBddLevelWiseCtrlToolRT();

protected:
    DesProject* m_pProject;
    const DesSubsystem* m_pCrtSubsys;
    const DesInterface* m_pCrtInterf;

private:
    DesFlatProject* flatProject();
    DesHierProject* project();
    QMap<QString, RegressionTestAlgorithms> flatAlgoCallMap;
    QMap<QString, RegressionTestAlgorithms> hierAlgoCallMap;
};
} //end of namespace DESpot

#endif // REGRESSIONTESTALGORITHMS_H

