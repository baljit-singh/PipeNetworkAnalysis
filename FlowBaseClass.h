/*
  FlowBaseClass.h
  Parent class for pipe network items such as pipes, valves, fittings

  Inherits from QGraphicsItemGroup
  The parent groups the graphical items together, such as the points of nodes
*/

#ifndef FLOWBASECLASS_H
#define FLOWBASECLASS_H

#include "FlowNode.h"
#include "Fluid.h"
#include <QTreeWidgetItem>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>


class FlowBaseClass : public QGraphicsItemGroup

{
public:
    static const QStringList& getNominalNamesList();

    FlowBaseClass(QTreeWidgetItem*, const Fluid*);
    ~FlowBaseClass();

    void setFlowRate(Constants::KnownType, qreal = 2*Constants::EPSILON);
    void setNodePressure(Constants::NodeType, Constants::KnownType, qreal = 2*Constants::EPSILON);
    void setVelocity(Constants::KnownType, qreal = 2*Constants::EPSILON);
    void setDiameterIndex(unsigned);

    qreal getFlowRate() const;
    qreal getFlowRateCoefficient() const;           // returns BIG_NUMBER if flowrate is known. otherwise, 1.
    qreal getVelocity() const;
    qreal getFlowSpeed() const;                     // =|getVelocity()|
    qreal getNodePressureDifference() const;
    qreal getDynamicPressure() const;
    qreal getDiameter() const;
    qreal getArea() const;
    qreal getReynoldsNumber(void) const;

    virtual qreal getSystemEquation() const;
    virtual qreal getDiffSysEqWithFlowRate();

    unsigned getDiameterIndex() const;
    bool isLaminar() const;
    bool isFlowRateKnown() const;
    bool isFlowRateSolved() const;
    bool isFlowRateValid() const;                   // known OR valid
    bool isNodePressureKnown(Constants::NodeType) const;
    qreal getNodePressure(Constants::NodeType) const;
    const FlowNode& getNode(Constants::NodeType) const;


protected:
    const Fluid* const fluidPtr;
    QTreeWidgetItem* const resultViewerPtr;
    FlowNode nodeRed, nodeGreen;
    qreal flowRate;

    void keyPressEvent(QKeyEvent*);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    static QStringList nominalNamesList;            // list of standard pipe sizes
    static QList<qreal> nominalDiameterList;
    static void nominalSizesLoader(void);


    unsigned diameterIndex;
    Constants::KnownType flowRateKnownType;
};

#endif // FLOWBASECLASS_H
