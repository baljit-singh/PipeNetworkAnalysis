/*
  FlowNode.h
  Hold the grahical location pressure at a point.

  Inherits from QGraphicsEllipseItem. Qt does not have a point/cicle class.
  An ellipse with small major and minor axes is used as a "point."
*/

#ifndef FLOWNODE_H
#define FLOWNODE_H

#include "Constants.h"
#include <QGraphicsEllipseItem>


class FlowNode : public QGraphicsEllipseItem
{

public:
    FlowNode();
    FlowNode(const QPointF&);
    FlowNode(const QPointF&, qreal);

    void setPressure(Constants::KnownType, qreal = 2*Constants::EPSILON);
    void setLocation(const QPointF&);
    void setLocation(qreal x, qreal y);

    qreal getPressure() const;
    qreal getPressureCoefficient() const;       // returns BIG_NUMBER if pressure is known. 1 otherwise.
    QPointF getLocation() const;
    bool isPressureKnown() const;
    bool isPressureSolved() const;
    bool isPressureValid() const;

    qreal operator-(const FlowNode&) const;         // returns pressure difference between the two nodes
    bool operator==(const FlowNode&) const;         // tests whether the two nodes coincide


private:    
    qreal nodePressure;
    Constants::KnownType pressureKnownType;
};

#endif // FLOWNODE_H
