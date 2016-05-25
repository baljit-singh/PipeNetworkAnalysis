#include "Valve.h"
/*
  Valve.cpp
  Defines methods for Valve class
*/

#include "QPen"


QStringList Valve::valveTypeList;

const QStringList &Valve::getValveTypeList(void)
{
    valveTypeListLoader();
    return valveTypeList;
}

void Valve::valveTypeListLoader()
{
    if (valveTypeList.isEmpty())
    {
        valveTypeList.push_back("Globe Valve, Fully Open");

        valveTypeList.push_back("Steel: Galvanized");

        valveTypeList.push_back("PVC");

        valveTypeList.push_back("Cast Iron: Asphalted");

        valveTypeList.push_back("Wrought Iron");
    }
}



Valve::Valve(QTreeWidgetItem *resultPtr, Fluid *fPtr)   :
    FlowBaseClass(resultPtr, fPtr),
    valveTypeIndex(0)
{
    addToGroup(&rectGraphic);
    nodeGreen.setLocation(5.0*Constants::PIXEL_SIZE,0.0);
    rectGraphic.setRect(nodeRed.getLocation().x(), nodeRed.getLocation().y() - 3.0*Constants::PIXEL_SIZE, nodeGreen.getLocation().x() - Constants::PIXEL_SIZE, nodeGreen.getLocation().y() + 5.0*Constants::PIXEL_SIZE);
    rectGraphic.setPen(QPen(QBrush(Qt::black), 0.5*Constants::PIXEL_SIZE, Qt::SolidLine, Qt::FlatCap));
    rectGraphic.setBrush(Qt::Dense7Pattern);

    valveTypeListLoader();
}


void Valve::setValveTypeIndex(unsigned index)         {valveTypeIndex = (index % valveTypeList.size());}

qreal Valve::getMinorLoss() const
{
    return 0;

}

unsigned Valve::getValveTypeIndex() const            {return valveTypeIndex;}
qreal Valve::getSystemEquation(void) const           {return (getNodePressureDifference() - getMinorLoss()*getDynamicPressure());}

qreal Valve::getDiffSysEqWithFlowRate()
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


void Valve::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    //PipeEditorDialog(0, this).exec();
    FlowBaseClass::mouseDoubleClickEvent(event);
}

void Valve::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // {D[cm], L[m], Q[L/s], ?P[kPa], k, f, Re, m[kg/s], V[m/s], Pin[kPa], Pout[kPa]
    // {0,      1,     2,       3,    4, 5,  6,   7,       8,      9,        10}

    for (int i = 1; i < resultViewerPtr->columnCount(); i++)
        resultViewerPtr->setText(i, "???");

    resultViewerPtr->setText(1, "N/A");
    resultViewerPtr->setText(5, "N/A");
    resultViewerPtr->setText(4, QString::number(getMinorLoss()));

    FlowBaseClass::mousePressEvent(event);
}
