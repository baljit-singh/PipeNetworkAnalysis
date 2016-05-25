/*
  Valve.h
  Declares functionality specific for valves

  Inherits from FlowBaseClass
*/

#ifndef VALVE_H
#define VALVE_H

#include "FlowBaseClass.h"

class Valve : public FlowBaseClass
{
public:
    static const QStringList& getValveTypeList(void);


    Valve(QTreeWidgetItem*, Fluid*);

    void setValveTypeIndex(unsigned);

    unsigned getValveTypeIndex(void) const;
    qreal getMinorLoss(void) const;
    qreal getSystemEquation(void) const;
    qreal getDiffSysEqWithFlowRate();       // returns the derivative of system equation with respect to flow rate. if flow rate is knows, returns -BIG_NUMBER

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);


private:
    static QStringList valveTypeList;
    static void valveTypeListLoader(void);


    QGraphicsRectItem rectGraphic;
    unsigned valveTypeIndex;
};

#endif // VALVE_H
