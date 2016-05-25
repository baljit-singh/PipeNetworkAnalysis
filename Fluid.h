/*
  Fluid.h
  Stores fluid properties: density and dynamic viscosity
*/

#ifndef FLUID_H
#define FLUID_H

#include <QtGlobal>

class Fluid
{

public:
    Fluid(qreal rho, qreal mu);

    void setDensity(qreal);
    void setDynamicViscosity(qreal);

    qreal getDensity(void) const;
    qreal getDynamicViscosity(void) const;
    qreal getKinematicViscosity(void) const;


private:
    qreal fluidDensity, fluidDynamicViscosity;

};

#endif // FLUID_H
