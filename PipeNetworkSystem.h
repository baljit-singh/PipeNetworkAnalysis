/*
  PipeNetworkSystem.h
  Manages multiple instances of Pipe, Valve, Fittings classes, and their interconnections
  Also solves for unknown parameters if given properties
*/

#ifndef PIPENETWORKSYSTEM_H
#define PIPENETWORKSYSTEM_H


#include "Fluid.h"
#include "Pipe.h"
#include "Valve.h"
#include "QGraphicsScene"

class PipeNetworkSystem
{
public:
    PipeNetworkSystem();
    ~PipeNetworkSystem();

    void setGUIProperties(QGraphicsScene*, QTreeWidgetItem*);
    void addPipe();
    void addValve();

    void deletePipe(Pipe*);

    void setFluidByIndex(unsigned);
    void editSystemFluid(qreal rho, qreal mu);

    void clearItems(void);
    void systemSolver(void);

    static const QStringList& getFluidNamesList();
    static const QList<Fluid>& getFluidList();

private:
    Fluid networkFluid;
    QList<FlowBaseClass*> itemsList;
    QGraphicsScene* graphicScenePtr;
    QTreeWidgetItem* resultViewerPtr;

    static QStringList listOfFluidNames;
    static QList<Fluid> listOfFluids;
    static void fluidListLoader(void);

    static bool isNonzero(qreal);


};

#endif // PIPENETWORKSYSTEM_H
