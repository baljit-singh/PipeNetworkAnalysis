/*
  FlowBaseClass.cpp
  Class methods definitions
*/


#include "FlowBaseClass.h"

// static fields & methods
QStringList FlowBaseClass::nominalNamesList;
QList<qreal> FlowBaseClass::nominalDiameterList;

const QStringList &FlowBaseClass::getNominalNamesList()
{
    nominalSizesLoader();
    return nominalNamesList;
}

void FlowBaseClass::nominalSizesLoader()
{
    if (nominalNamesList.isEmpty())
    {
        nominalNamesList.push_back("4 Nominal Schedule 40");
        nominalDiameterList.push_back(10.23e-2);

        nominalNamesList.push_back("4 Nominal Schedule 80");
        nominalDiameterList.push_back(9.73e-2);

        nominalNamesList.push_back("6 Nominal Schedule 40");
        nominalDiameterList.push_back(15.42e-2);
    }
}



FlowBaseClass::FlowBaseClass(QTreeWidgetItem *resultPtr, const Fluid *fPtr)  :
    resultViewerPtr(resultPtr),
    fluidPtr(fPtr),
    diameterIndex(0),
    flowRateKnownType(Constants::UNKNOWN),
    flowRate(2*Constants::EPSILON)
{
    nodeRed.setBrush(Qt::red);
    nodeGreen.setBrush(Qt::green);
    addToGroup(&nodeRed);
    addToGroup(&nodeGreen);
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);

    nominalSizesLoader();
}

FlowBaseClass::~FlowBaseClass()
{
    removeFromGroup(&nodeRed);
    removeFromGroup(&nodeGreen);
}



void FlowBaseClass::setFlowRate(Constants::KnownType type, qreal Q)
{
    flowRate = Q;
    flowRateKnownType = type;
}

void FlowBaseClass::setNodePressure(Constants::NodeType node, Constants::KnownType type, qreal P)
{
    if (node == Constants::RED)
        nodeRed.setPressure(type,P);
    else if (node == Constants::GREEN)
        nodeGreen.setPressure(type,P);
}

void FlowBaseClass::setVelocity(Constants::KnownType type, qreal V)     {setFlowRate(type, V*getArea());}
void FlowBaseClass::setDiameterIndex(unsigned index)                    {diameterIndex = (index % nominalDiameterList.size());}


qreal FlowBaseClass::getFlowRate() const                                {return flowRate;}
qreal FlowBaseClass::getFlowRateCoefficient() const                     {return (isFlowRateKnown() ? Constants::BIG_NUMBER : 1.0);}
qreal FlowBaseClass::getVelocity() const                                {return flowRate/getArea();}
qreal FlowBaseClass::getFlowSpeed() const                               {return (getVelocity() > 0.0) ? getVelocity() : -getVelocity();}
qreal FlowBaseClass::getNodePressureDifference() const                  {return nodeRed - nodeGreen;}
qreal FlowBaseClass::getDynamicPressure() const                         {return 0.5*fluidPtr->getDensity()*getVelocity()*getFlowSpeed();}
qreal FlowBaseClass::getDiameter() const                                {return nominalDiameterList.at(diameterIndex);}
qreal FlowBaseClass::getArea() const                                    {return 0.25*Constants::PI*getDiameter()*getDiameter();}
qreal FlowBaseClass::getReynoldsNumber() const                          {return (getFlowSpeed()*getDiameter()/fluidPtr->getKinematicViscosity());}
qreal FlowBaseClass::getSystemEquation() const                          {return (getNodePressureDifference() - getDynamicPressure());}
qreal FlowBaseClass::getDiffSysEqWithFlowRate()                         {return (isFlowRateKnown() ? -Constants::BIG_NUMBER : -fluidPtr->getDensity()*getFlowSpeed()/getArea());}
unsigned FlowBaseClass::getDiameterIndex() const                        {return diameterIndex;}
bool FlowBaseClass::isLaminar() const                                   {return (getReynoldsNumber() < Constants::LAMINAR_LIMIT);}
bool FlowBaseClass::isFlowRateKnown() const                             {return (flowRateKnownType == Constants::KNOWN);}
bool FlowBaseClass::isFlowRateSolved() const                            {return (flowRateKnownType == Constants::SOLVED);}
bool FlowBaseClass::isFlowRateValid() const                             {return (isFlowRateKnown() || isFlowRateSolved());}

bool FlowBaseClass::isNodePressureKnown(Constants::NodeType type) const
{
    if (type == Constants::RED)
        return nodeRed.isPressureKnown();
    else if (type == Constants::GREEN)
        return nodeGreen.isPressureKnown();
}

qreal FlowBaseClass::getNodePressure(Constants::NodeType type) const
{
    if (type==Constants::RED)
        return nodeRed.getPressure();
    else if (type==Constants::GREEN)
        return nodeGreen.getPressure();
}

const FlowNode &FlowBaseClass::getNode(Constants::NodeType type) const
{
    if (type == Constants::RED)
        return nodeRed;
    else if (type == Constants::GREEN)
        return nodeGreen;
}

void FlowBaseClass::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Delete:
        break;          // delete item feature to be added in the future
    case Qt::Key_Space:
        setRotation(rotation()-90.0);
        break;
    }

    QGraphicsItemGroup::keyPressEvent(event);
}

void FlowBaseClass::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // {D[cm], L[m], Q[L/s], ?P[kPa], k, f, Re, m[kg/s], V[m/s], Pin[kPa], Pout[kPa]
    // {0,      1,     2,       3,    4, 5,  6,   7,       8,      9,        10}

    resultViewerPtr->setText(0, QString::number(100.0*getDiameter()));

    if (isFlowRateValid())
    {
        resultViewerPtr->setText(2, QString::number(1000.0*getFlowRate()));
        resultViewerPtr->setText(6, QString::number(getReynoldsNumber()));
        resultViewerPtr->setText(7, QString::number(getFlowRate() * fluidPtr->getDensity()));
        resultViewerPtr->setText(8, QString::number(getVelocity()));
    }

    if (nodeRed.isPressureValid() && nodeGreen.isPressureValid())
        resultViewerPtr->setText(3, QString::number(getNodePressureDifference()/1000.0));

    if (nodeRed.isPressureValid())
        resultViewerPtr->setText(9, QString::number(nodeRed.getPressure()/1000.0));

    if(nodeGreen.isPressureValid())
        resultViewerPtr->setText(10, QString::number(nodeGreen.getPressure()/1000.0));


    QGraphicsItemGroup::mousePressEvent(event);
}
