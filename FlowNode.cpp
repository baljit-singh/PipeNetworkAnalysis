/*
  FlowNode.cpp
  Class method definitions
*/

#include "FlowNode.h"
#include "QBrush"


FlowNode::FlowNode()    :
    pressureKnownType(Constants::UNKNOWN),
    nodePressure(2*Constants::EPSILON)
{
    setRect(0.0,0.0,2*Constants::PIXEL_SIZE,2*Constants::PIXEL_SIZE);
    setBrush(Qt::SolidPattern);
}

FlowNode::FlowNode(const QPointF &location)
{
    setRect(location.x(),location.y(),2*Constants::PIXEL_SIZE,2*Constants::PIXEL_SIZE);
    setBrush(Qt::SolidPattern);
}

FlowNode::FlowNode(const QPointF &location, qreal pressure)
{
    setRect(location.x(),location.y(),2*Constants::PIXEL_SIZE,2*Constants::PIXEL_SIZE);
    setBrush(Qt::SolidPattern);
    setPressure(Constants::KNOWN, pressure);
}



void FlowNode::setPressure(Constants::KnownType type, qreal P)
{
    nodePressure = P;
    pressureKnownType = type;
}

void FlowNode::setLocation(const QPointF &A)
{
    setX(A.x());
    setY(A.y());
}
void FlowNode::setLocation(qreal x, qreal y)
{
    setX(x);
    setY(y);
}


qreal FlowNode::getPressure() const                         {return nodePressure;}
qreal FlowNode::getPressureCoefficient() const              {return (isPressureKnown() ? Constants::BIG_NUMBER : 1.0);}
QPointF FlowNode::getLocation() const                       {return (scenePos() + QPointF(Constants::PIXEL_SIZE,Constants::PIXEL_SIZE));}  // returns center instead of top left edge
bool FlowNode::isPressureKnown() const                      {return (pressureKnownType==Constants::KNOWN);}
bool FlowNode::isPressureSolved() const                     {return (pressureKnownType==Constants::SOLVED);}
bool FlowNode::isPressureValid() const                      {return (isPressureKnown() || isPressureSolved());}
qreal FlowNode::operator-(const FlowNode &otherNode) const  {return (nodePressure - otherNode.nodePressure);}

bool FlowNode::operator==(const FlowNode &otherNode) const
{
    QPointF A(getLocation() - otherNode.getLocation());
    return (A.x()*A.x() + A.y()*A.y()) < 5.0*(Constants::PIXEL_SIZE*Constants::PIXEL_SIZE);     // 5.0 is allowed tolerance
}
