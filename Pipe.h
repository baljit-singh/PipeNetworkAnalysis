/*
  Pipe.h
  Declares functionality specific for pipes

  Inherits from FlowBaseClass
*/

#ifndef PIPE_H
#define PIPE_H

#include "FlowBaseClass.h"

class Pipe : public FlowBaseClass
{
public:
    static const QStringList& getMaterialList(void);


    Pipe(QTreeWidgetItem*, Fluid*);

    void setLength(qreal);
    void setMaterialIndex(unsigned);

    unsigned getMaterialIndex(void) const;
    qreal getLength(void) const;
    qreal getRoughness(void) const;
    qreal getRelativeRoughness(void) const;
    qreal getSystemEquation(void) const;
    qreal getFrictionFactor(void) const;
    qreal getDiffSysEqWithFlowRate();       // returns the derivative of system equation with respect to flow rate. if flow rate is knows, returns -BIG_NUMBER

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    static QStringList materialNamesList;
    static QList<qreal> materialRoughnessList;
    static void materialListLoader(void);


    QGraphicsLineItem lineGraphic;
    qreal pipeLength;
    unsigned materialIndex;
};

#endif // PIPE_H
