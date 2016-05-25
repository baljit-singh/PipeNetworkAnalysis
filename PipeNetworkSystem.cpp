/*
  PipeNetworkSystem.cpp
  Funcationality of the pipe system manager class
*/

#include "PipeNetworkSystem.h"
#include <QMessageBox>
#include "armadillo/include/armadillo"

QStringList PipeNetworkSystem::listOfFluidNames;
QList<Fluid> PipeNetworkSystem::listOfFluids;

const QStringList &PipeNetworkSystem::getFluidNamesList()
{
    fluidListLoader();
    return listOfFluidNames;
}

const QList<Fluid> &PipeNetworkSystem::getFluidList()
{
    fluidListLoader();
    return listOfFluids;
}

void PipeNetworkSystem::fluidListLoader()
{
    if (listOfFluids.isEmpty())
    {
        listOfFluidNames.push_back("Air (STP)");
        listOfFluids.push_back(Fluid(1.293,1.8e-5));

        listOfFluidNames.push_back("Air (Room)");
        listOfFluids.push_back(Fluid(1.205,1.983e-5));

        listOfFluidNames.push_back("Acetone");
        listOfFluids.push_back(Fluid(1049.0,0.000316));

        listOfFluidNames.push_back("Castor Oil");
        listOfFluids.push_back(Fluid(960.0,650e-3));

        listOfFluidNames.push_back("Chloroform");
        listOfFluids.push_back(Fluid(1470.0,0.53e-3));

        listOfFluidNames.push_back("Ethyl Alcohol (Ethanol)");
        listOfFluids.push_back(Fluid(789.0,0.001095));

        listOfFluidNames.push_back("Water");
        listOfFluids.push_back(Fluid(998.0,1.787e-3));
    }
}

bool PipeNetworkSystem::isNonzero(qreal A)
{
    return ((A <= -Constants::EPSILON) || (A >= Constants::EPSILON));
}



PipeNetworkSystem::PipeNetworkSystem()  :
    networkFluid(Fluid(998,1.787e-3))
{
    fluidListLoader();
}

PipeNetworkSystem::~PipeNetworkSystem()
{
    clearItems();
}

void PipeNetworkSystem::setGUIProperties(QGraphicsScene* gPtr, QTreeWidgetItem* rPtr)
{
    graphicScenePtr = gPtr;
    resultViewerPtr = rPtr;
}

void PipeNetworkSystem::addPipe()
{
    itemsList.push_back(new Pipe(resultViewerPtr, &networkFluid));
    graphicScenePtr->addItem(itemsList.last());
}

void PipeNetworkSystem::addValve()
{
    itemsList.push_back(new Valve(resultViewerPtr, &networkFluid));
    graphicScenePtr->addItem(itemsList.last());
}

void PipeNetworkSystem::deletePipe(Pipe* A)
{
    if (itemsList.removeOne(A))
        delete A;
}

void PipeNetworkSystem::setFluidByIndex(unsigned i)
{
    networkFluid = listOfFluids.at(i);
}

void PipeNetworkSystem::editSystemFluid(qreal rho, qreal mu)
{
    networkFluid.setDensity(rho);
    networkFluid.setDynamicViscosity(mu);
}

void PipeNetworkSystem::clearItems()
{
    for (int i = 0; i < itemsList.size(); i++)
        delete itemsList.at(i);

    itemsList.clear();
}

void PipeNetworkSystem::systemSolver()
{
    // using Newtown-Rhapson method. x_nextIteration = x_current - f(x_current)/f'(x_current)

    using namespace arma;

    const int numOfItems = itemsList.size();

    int mainIndex, minorIndex, miscIndex, nextIndex, numOfKnowns, numOfPressureEqs, unknownsSolved= 0, loopCounter =0;

    Mat<qreal> jacobian(numOfItems*Constants::N_UNKNOWNS, numOfItems*Constants::N_UNKNOWNS);
    Col<qreal> sysOfEqs(numOfItems*Constants::N_UNKNOWNS);
    Col<qreal> delX(numOfItems*Constants::N_UNKNOWNS);

    QVector<bool> nodeUsed;
    QList<const FlowNode*> UsedNode;
    bool WasUsedBefore;

    jacobian.zeros();
    sysOfEqs.zeros();

    while ((unknownsSolved != sysOfEqs.n_rows) && (loopCounter < Constants::MAX_NUM_OF_LOOPS))
    {
        // reset variables
        loopCounter++;
        unknownsSolved = 0;
        nextIndex = numOfItems;
        numOfKnowns = 0;
        numOfPressureEqs = 0;
        UsedNode.clear();
        nodeUsed = QVector<bool>(numOfItems*Constants::N_NODES, false);


        for (mainIndex = 0; mainIndex < numOfItems; mainIndex++)
        {
            // system eqs
            sysOfEqs(mainIndex) = itemsList.at(mainIndex)->getSystemEquation();
            jacobian(mainIndex, mainIndex*Constants::N_UNKNOWNS)   =  itemsList.at(mainIndex)->getNode(Constants::RED).getPressureCoefficient();
            jacobian(mainIndex, mainIndex*Constants::N_UNKNOWNS+1) = -itemsList.at(mainIndex)->getNode(Constants::GREEN).getPressureCoefficient();
            jacobian(mainIndex, mainIndex*Constants::N_UNKNOWNS+2) = itemsList.at(mainIndex)->getDiffSysEqWithFlowRate();

            // Knowns Eqs
            if (itemsList.at(mainIndex)->isNodePressureKnown(Constants::RED))
            {
                jacobian(nextIndex + numOfKnowns, mainIndex*Constants::N_UNKNOWNS) = Constants::BIG_NUMBER;      // known
                numOfKnowns++;
            }

            if (itemsList.at(mainIndex)->isNodePressureKnown(Constants::GREEN))
            {
                jacobian(nextIndex + numOfKnowns, mainIndex*Constants::N_UNKNOWNS+1) = Constants::BIG_NUMBER;
                numOfKnowns++;
            }

            if (itemsList.at(mainIndex)->isFlowRateKnown())
            {
                jacobian(nextIndex + numOfKnowns, mainIndex*Constants::N_UNKNOWNS+2) = Constants::BIG_NUMBER;
                numOfKnowns++;
            }
        }

        nextIndex += numOfKnowns;


        // Writes equations for pressures for nodes that coincide
        for (mainIndex = 0; mainIndex < numOfItems; mainIndex++)
        {
            for (minorIndex = mainIndex + 1; minorIndex < numOfItems; minorIndex++)
            {
                if (!nodeUsed.at(minorIndex*Constants::N_NODES))        // prevents redudant eqs
                {
                    if (itemsList.at(mainIndex)->getNode(Constants::RED) == itemsList.at(minorIndex)->getNode(Constants::RED))
                    {
                        sysOfEqs(nextIndex + numOfPressureEqs) = itemsList.at(minorIndex)->getNode(Constants::RED) - itemsList.at(mainIndex)->getNode(Constants::RED);

                        jacobian(nextIndex + numOfPressureEqs, mainIndex*Constants::N_UNKNOWNS)  = -itemsList.at(mainIndex)->getNode(Constants::RED).getPressureCoefficient();
                        jacobian(nextIndex + numOfPressureEqs, minorIndex*Constants::N_UNKNOWNS) =  itemsList.at(minorIndex)->getNode(Constants::RED).getPressureCoefficient();
                        numOfPressureEqs++;

                        nodeUsed.replace(mainIndex*Constants::N_NODES, true);
                        nodeUsed.replace(minorIndex*Constants::N_NODES, true);
                    }
                    else if (itemsList.at(mainIndex)->getNode(Constants::GREEN) == itemsList.at(minorIndex)->getNode(Constants::RED))
                    {
                        sysOfEqs(nextIndex + numOfPressureEqs) = itemsList.at(minorIndex)->getNode(Constants::RED) - itemsList.at(mainIndex)->getNode(Constants::GREEN);


                        jacobian(nextIndex + numOfPressureEqs, mainIndex*Constants::N_UNKNOWNS+1)  = -itemsList.at(mainIndex)->getNode(Constants::GREEN).getPressureCoefficient();
                        jacobian(nextIndex + numOfPressureEqs, minorIndex*Constants::N_UNKNOWNS)   =  itemsList.at(minorIndex)->getNode(Constants::RED).getPressureCoefficient();
                        numOfPressureEqs++;

                        nodeUsed.replace(mainIndex*Constants::N_NODES + 1, true);
                        nodeUsed.replace(minorIndex*Constants::N_NODES, true);
                    }
                }


                if (!nodeUsed.at(minorIndex*Constants::N_NODES+1))
                {
                    if (itemsList.at(mainIndex)->getNode(Constants::RED) == itemsList.at(minorIndex)->getNode(Constants::GREEN))
                    {
                        sysOfEqs(nextIndex + numOfPressureEqs) = itemsList.at(minorIndex)->getNode(Constants::GREEN) - itemsList.at(mainIndex)->getNode(Constants::RED);

                        jacobian(nextIndex + numOfPressureEqs, mainIndex*Constants::N_UNKNOWNS)    = -itemsList.at(mainIndex)->getNode(Constants::RED).getPressureCoefficient();
                        jacobian(nextIndex + numOfPressureEqs, minorIndex*Constants::N_UNKNOWNS+1) =  itemsList.at(minorIndex)->getNode(Constants::GREEN).getPressureCoefficient();
                        numOfPressureEqs++;

                        nodeUsed.replace(mainIndex*Constants::N_NODES, true);
                        nodeUsed.replace(minorIndex*Constants::N_NODES + 1, true);
                    }

                    else if (itemsList.at(mainIndex)->getNode(Constants::GREEN) == itemsList.at(minorIndex)->getNode(Constants::GREEN))
                    {
                        sysOfEqs(nextIndex + numOfPressureEqs) = itemsList.at(minorIndex)->getNode(Constants::GREEN) - itemsList.at(mainIndex)->getNode(Constants::GREEN);

                        jacobian(nextIndex + numOfPressureEqs, mainIndex*Constants::N_UNKNOWNS+1)  = -itemsList.at(mainIndex)->getNode(Constants::GREEN).getPressureCoefficient();
                        jacobian(nextIndex + numOfPressureEqs, minorIndex*Constants::N_UNKNOWNS+1) =  itemsList.at(minorIndex)->getNode(Constants::GREEN).getPressureCoefficient();
                        numOfPressureEqs++;

                        nodeUsed.replace(mainIndex*Constants::N_NODES +1, true);
                        nodeUsed.replace(minorIndex*Constants::N_NODES +1, true);
                    }
                }
            }
        }

        nextIndex += numOfPressureEqs;


        // write convervation of mass equations
        UsedNode.clear();
        for (mainIndex = 0; mainIndex < numOfItems; mainIndex++)
        {
            WasUsedBefore = false;
            for (miscIndex = 0; miscIndex < UsedNode.size(); miscIndex++)
            {
                if (*UsedNode.at(miscIndex) == itemsList.at(mainIndex)->getNode(Constants::RED))
                {
                    WasUsedBefore = true;
                    break;
                }
            }

            if (!WasUsedBefore && nodeUsed.at(mainIndex*Constants::N_NODES))     // prevents redudanct equations for mass conservation
            {
                sysOfEqs(nextIndex + UsedNode.size()) = -itemsList.at(mainIndex)->getFlowRate();
                jacobian(nextIndex + UsedNode.size(), mainIndex*Constants::N_UNKNOWNS+2) =  -itemsList.at(mainIndex)->getFlowRateCoefficient();

                for (minorIndex = mainIndex + 1; minorIndex < numOfItems; minorIndex++)
                {
                    if ((itemsList.at(mainIndex)->getNode(Constants::RED) == itemsList.at(minorIndex)->getNode(Constants::RED)) || (itemsList.at(mainIndex)->getNode(Constants::RED) == itemsList.at(minorIndex)->getNode(Constants::GREEN)))
                    {
                        sysOfEqs(nextIndex + UsedNode.size()) += itemsList.at(minorIndex)->getFlowRate();
                        jacobian(nextIndex + UsedNode.size(), minorIndex*Constants::N_UNKNOWNS+2) = itemsList.at(minorIndex)->getFlowRateCoefficient();
                    }
                }

                UsedNode.push_back(&itemsList.at(mainIndex)->getNode(Constants::RED));
            }


            WasUsedBefore = false;
            for (miscIndex = 0; miscIndex < UsedNode.size(); miscIndex++)
            {
                if (*UsedNode.at(miscIndex) == itemsList.at(mainIndex)->getNode(Constants::GREEN))
                {
                    WasUsedBefore = true;
                    break;
                }
            }

            if (!WasUsedBefore && nodeUsed.at(mainIndex*Constants::N_NODES+1))
            {
                sysOfEqs(nextIndex + UsedNode.size()) = -itemsList.at(mainIndex)->getFlowRate();
                jacobian(nextIndex + UsedNode.size(), mainIndex*Constants::N_UNKNOWNS+2) = -itemsList.at(mainIndex)->getFlowRateCoefficient();

                for (minorIndex = mainIndex + 1; minorIndex < numOfItems; minorIndex++)
                {
                    if ((itemsList.at(mainIndex)->getNode(Constants::GREEN) == itemsList.at(minorIndex)->getNode(Constants::RED)) || (itemsList.at(mainIndex)->getNode(Constants::GREEN) == itemsList.at(minorIndex)->getNode(Constants::GREEN)))
                    {
                        sysOfEqs(nextIndex + UsedNode.size()) += itemsList.at(minorIndex)->getFlowRate();
                        jacobian(nextIndex + UsedNode.size(), minorIndex*Constants::N_UNKNOWNS+2) = itemsList.at(minorIndex)->getFlowRateCoefficient();
                    }
                }

                UsedNode.push_back(&itemsList.at(mainIndex)->getNode(Constants::GREEN));
            }
        }

        nextIndex += UsedNode.size();


        if (nextIndex != sysOfEqs.n_rows)
        {
            QMessageBox::warning(0,"Error", "Too Many Unknowns");
            break;  // while loop
        }
        else
        {
            delX = solve(jacobian, sysOfEqs, true);

            // get answers
            for (mainIndex = 0; mainIndex < numOfItems; mainIndex++)
            {
                if(!itemsList.at(mainIndex)->isNodePressureKnown(Constants::RED))
                    itemsList.at(mainIndex)->setNodePressure(Constants::RED, Constants::SOLVED, itemsList.at(mainIndex)->getNodePressure(Constants::RED) - delX(mainIndex*Constants::N_UNKNOWNS));

                if(!itemsList.at(mainIndex)->isNodePressureKnown(Constants::GREEN))
                    itemsList.at(mainIndex)->setNodePressure(Constants::GREEN, Constants::SOLVED, itemsList.at(mainIndex)->getNodePressure(Constants::GREEN) - delX(mainIndex*Constants::N_UNKNOWNS+1));

                if(!itemsList.at(mainIndex)->isFlowRateKnown())
                    itemsList.at(mainIndex)->setFlowRate(Constants::SOLVED, itemsList.at(mainIndex)->getFlowRate() - delX(mainIndex*Constants::N_UNKNOWNS+2));


                if (!isNonzero(delX(mainIndex*Constants::N_UNKNOWNS)))
                    unknownsSolved++;

                if (!isNonzero(delX(mainIndex*Constants::N_UNKNOWNS+1)))
                    unknownsSolved++;
                if (!isNonzero(delX(mainIndex*Constants::N_UNKNOWNS+2)))
                    unknownsSolved++;
            }
        }
    }

    if (loopCounter == Constants::MAX_NUM_OF_LOOPS)
        QMessageBox::critical(0,"Too many iterations", "Solution not converging. Check inputs.");

}
