/*
  PipeNetworkGUI
  Provides the graphical interface to input and view results
*/

#ifndef PIPENETWORKGUI_H
#define PIPENETWORKGUI_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QCloseEvent>
#include "PipeNetworkSystem.h"

namespace Ui {
class PipeNetworkGUI;
}

class PipeNetworkGUI : public QMainWindow
{
    Q_OBJECT
//    Q_CLASSINFO("Name", Constants::SOFTWARE_NAME)
//    Q_CLASSINFO("Authors", Constants::AUTHORS)
//    Q_CLASSINFO("Version", Constants::PROJECT_VERSION)
    
public:
    explicit PipeNetworkGUI(QWidget *parent = 0);
    ~PipeNetworkGUI();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void newAction(void);
    void openAction(void);
    bool saveAction(void);
    void showAbout(void);

    void updateFluidSelection();
    void updateCustomFluid();

    void addItem(void);
    void clearAll(void);

    void runSimulation();


private:
    Ui::PipeNetworkGUI *ui;
    QGraphicsScene *myGraphicsScene;
    PipeNetworkSystem currentPipeSystem;

    void loadFile(void);
    static int confirmSaveDialog(QString);
};

#endif  // PIPENETWORKGUI_H
