/*
  Fluid.cpp
  Functionality of the Fluid Class
*/

#include "Fluid.h"

Fluid::Fluid(qreal rho, qreal mu)
{
    // sets functions check for invalid entries
    setDensity(rho);
    setDynamicViscosity(mu);
}

void Fluid::setDensity(qreal rho)
{
    fluidDensity = rho;
}

void Fluid::setDynamicViscosity(qreal mu)
{
    fluidDynamicViscosity = mu;
}

qreal Fluid::getDensity(void) const
{
    return fluidDensity;
}

qreal Fluid::getDynamicViscosity(void) const
{
    return fluidDynamicViscosity;
}

qreal Fluid::getKinematicViscosity(void) const
{
    return (fluidDynamicViscosity/fluidDensity);
}
