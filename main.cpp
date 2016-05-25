/* main.cpp
  Initiates program
*/

#include <QtGui/QApplication>
#include "PipeNetworkGUI.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PipeNetworkGUI w;
    w.show();
    
    return a.exec();
}
