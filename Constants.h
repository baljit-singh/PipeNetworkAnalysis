/*
  Constants.h
  This header file defines constants used in various locations of the project.
*/


#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QtGlobal>
#include <QString>

namespace Constants
{

const QString PROJECT_VERSION("1.0/Beta");
const QString AUTHORS("Baljit Singh & Samira Darvishi");
const QString SOFTWARE_NAME("Pipe Network Analysis Software");
const qreal PI = 3.14159265;
const qreal EPSILON = 1e-5;
const qreal BIG_NUMBER = 1e30;
const unsigned N_UNKNOWNS = 3;
const unsigned N_NODES = 2;
const qreal PIXEL_SIZE = 5.0;
const qreal LAMINAR_LIMIT = 2100.0;
const unsigned MAX_NUM_OF_LOOPS = 500;

enum KnownType  {KNOWN, SOLVED, GUESSED, UNKNOWN};
enum NodeType   {RED,GREEN};

}

#endif // CONSTANTS_H
