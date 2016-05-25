/*
  PipeEditorDialog.h
  Graohics interface to edit pipe properties
*/

#ifndef PIPEEDITORDIALOG_H
#define PIPEEDITORDIALOG_H

#include <QDialog>
#include "Pipe.h"

namespace Ui {
class PipeEditorDialog;
}

class PipeEditorDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PipeEditorDialog(QWidget *parent = 0, Pipe *A = 0);
    ~PipeEditorDialog();
    
private slots:
    void toggleFlowRate(void);
    void toggleVelocity(void);
    void togglePInlet(void);
    void togglePOutlet(void);

    void accept(void);                  // save and close dialog. virtual function

private:
    Ui::PipeEditorDialog *ui;
    Pipe *pipePtr;
};

#endif // PIPEEDITORDIALOG_H
