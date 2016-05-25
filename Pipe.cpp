/*
  Pipe.cpp
  Defines methods for Pipe class
*/

#include "Pipe.h"
#include "PipeEditorDialog.h"
#include <QtCore/qmath.h>
#include "QPen"


QStringList Pipe::materialNamesList;
QList<qreal> Pipe::materialRoughnessList;

const QStringList &Pipe::getMaterialList(void)
{
    materialListLoader();
    return materialNamesList;
}

void Pipe::materialListLoader()
{
    if (materialNamesList.isEmpty())
    {
        materialNamesList.push_back("Steel: Commercial");
        materialRoughnessList.push_back(0.0046e-2);

        materialNamesList.push_back("Steel: Galvanized");
        materialRoughnessList.push_back((0.006+0.025)*0.5e-2);

        materialNamesList.push_back("PVC");
        materialRoughnessList.push_back(0.0);

        materialNamesList.push_back("Cast Iron: Asphalted");
        materialRoughnessList.push_back(0.012e-2);

        materialNamesList.push_back("Wrought Iron");
        materialRoughnessList.push_back(0.0046e-2);
    }
}



Pipe::Pipe(QTreeWidgetItem *resultPtr, Fluid *fPtr)   :
    FlowBaseClass(resultPtr, fPtr),
    materialIndex(0),
    pipeLength(0.0)
{
    addToGroup(&lineGraphic);
    nodeGreen.setLocation(20.0*Constants::PIXEL_SIZE,0.0);
    lineGraphic.setLine(QLineF(nodeRed.getLocation(), nodeGreen.getLocation()));
    lineGraphic.setPen(QPen(QBrush(Qt::black), Constants::PIXEL_SIZE, Qt::SolidLine, Qt::FlatCap));

    materialListLoader();
}


void Pipe::setLength(qreal L)                       {pipeLength = L;}
void Pipe::setMaterialIndex(unsigned index)         {materialIndex = (index % materialRoughnessList.size());}

unsigned Pipe::getMaterialIndex() const             {return materialIndex;}
qreal Pipe::getLength() const                       {return pipeLength;}
qreal Pipe::getRoughness() const                    {return materialRoughnessList.at(materialIndex);}
qreal Pipe::getRelativeRoughness() const            {return getRoughness()/getDiameter();}
qreal Pipe::getSystemEquation(void) const           {return (getNodePressureDifference() - getFrictionFactor()*pipeLength*getDynamicPressure()/getDiameter());}

qreal Pipe::getFrictionFactor() const
{
    if (isLaminar())
        return 64.0/getReynoldsNumber();
    else
    {
        qreal tempRe = getReynoldsNumber();
        qreal tempRough = getRelativeRoughness();
        return qPow(-2.0*qLn(tempRough/3.7065 - 5.0452*qLn(qPow(tempRough, 1.1098)/2.8257 + 5.8506/qPow(tempRe, 0.8981))/(qLn(10)*tempRe))/qLn(10),-2.0);
    }
}

qreal Pipe::getDiffSysEqWithFlowRate()
{
    if (isFlowRateKnown())
        return -Constants::BIG_NUMBER;              // ensure negative
    else
    {
    flowRate += Constants::EPSILON;
    qreal upperValue = getSystemEquation();
    flowRate -= Constants::EPSILON;

    return (upperValue - getSystemEquation())/Constants::EPSILON;
    }
}


void Pipe::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    PipeEditorDialog(0, this).exec();
    FlowBaseClass::mouseDoubleClickEvent(event);
}

void Pipe::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // {D[cm], L[m], Q[L/s], ?P[kPa], k, f, Re, m[kg/s], V[m/s], Pin[kPa], Pout[kPa]
    // {0,      1,     2,       3,    4, 5,  6,   7,       8,      9,        10}

    for (int i = 1; i < resultViewerPtr->columnCount(); i++)
        resultViewerPtr->setText(i, "???");

    resultViewerPtr->setText(1, QString::number(pipeLength));

    if (isFlowRateValid())
    {
        resultViewerPtr->setText(5, QString::number(getFrictionFactor()));
        resultViewerPtr->setText(4, QString::number(getFrictionFactor()*pipeLength/getDiameter()));
    }

    FlowBaseClass::mousePressEvent(event);
}



