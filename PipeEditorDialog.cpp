/*
  PipeEditorDialog.cpp
  Functionality of the graphical pipe editor
*/

#include "PipeEditorDialog.h"
#include "ui_pipeeditordialog.h"

PipeEditorDialog::PipeEditorDialog(QWidget *parent, Pipe *A) :
    QDialog(parent),
    pipePtr(A),
    ui(new Ui::PipeEditorDialog)
{
    ui->setupUi(this);
    setWindowTitle("Pipe Property Editor");

    toggleFlowRate();
    toggleVelocity();
    togglePInlet();
    togglePOutlet();

    ui->pipeDimsSelection->addItems(FlowBaseClass::getNominalNamesList());
    ui->materialSelection->addItems(Pipe::getMaterialList());

    if (pipePtr != 0)       // prevents error in case of null pointer
    {
        ui->pipeDimsSelection->setCurrentIndex(pipePtr->getDiameterIndex());
        ui->materialSelection->setCurrentIndex(pipePtr->getMaterialIndex());
        ui->lengthBox->setValue(pipePtr->getLength());


        if (pipePtr->isFlowRateKnown())
        {
            ui->flowrateCheckbox->setChecked(true);
            toggleFlowRate();
            ui->flowrateBox->setValue(1000*pipePtr->getFlowRate());
            ui->velocityBox->setValue(pipePtr->getVelocity());
        }

        if (pipePtr->isNodePressureKnown(Constants::RED))
        {
            ui->pressureInletCheckbox->setChecked(true);
            togglePInlet();
            ui->pressureInletBox->setValue(pipePtr->getNodePressure(Constants::RED)/1000.0);
        }


        if (pipePtr->isNodePressureKnown(Constants::GREEN))
        {
            ui->pressureOutletCheckbox->setChecked(true);
            togglePOutlet();
            ui->pressureOutletBox->setValue(pipePtr->getNodePressure(Constants::GREEN)/1000.0);
        }
    }
}


PipeEditorDialog::~PipeEditorDialog()
{
    delete ui;
}

void PipeEditorDialog::toggleFlowRate()
{
    ui->flowrateBox->setEnabled(ui->flowrateCheckbox->isChecked());

    if (ui->flowrateCheckbox->isChecked())
    {
        ui->velocityCheckbox->setChecked(false);
        toggleVelocity();
    }
}

void PipeEditorDialog::toggleVelocity()
{
    ui->velocityBox->setEnabled(ui->velocityCheckbox->isChecked());

    if(ui->velocityCheckbox->isChecked())
    {
        ui->flowrateCheckbox->setChecked(false);
        toggleFlowRate();
    }
}

void PipeEditorDialog::togglePInlet()
{
    ui->pressureInletBox->setEnabled(ui->pressureInletCheckbox->isChecked());
}

void PipeEditorDialog::togglePOutlet()
{
    ui->pressureOutletBox->setEnabled(ui->pressureOutletCheckbox->isChecked());
}


void PipeEditorDialog::accept(void)
{
    if (pipePtr != 0)       // prevents error in case of null pointer
    {
        pipePtr->setDiameterIndex(ui->pipeDimsSelection->currentIndex());
        pipePtr->setMaterialIndex(ui->materialSelection->currentIndex());
        pipePtr->setLength(ui->lengthBox->value());


        if (ui->flowrateCheckbox->isChecked())
            pipePtr->setFlowRate(Constants::KNOWN, ui->flowrateBox->value()/1000.0);
        else if (!ui->velocityCheckbox->isChecked())
            pipePtr->setFlowRate(Constants::UNKNOWN);


        if (ui->velocityCheckbox->isChecked())
            pipePtr->setVelocity(Constants::KNOWN, ui->velocityBox->value());


        if (ui->pressureInletCheckbox->isChecked())
            pipePtr->setNodePressure(Constants::RED, Constants::KNOWN, ui->pressureInletBox->value()*1000.0);
        else
            pipePtr->setNodePressure(Constants::RED, Constants::UNKNOWN);


        if(ui->pressureOutletCheckbox->isChecked())
            pipePtr->setNodePressure(Constants::GREEN, Constants::KNOWN, ui->pressureOutletBox->value()*1000.0);
        else
            pipePtr->setNodePressure(Constants::GREEN, Constants::UNKNOWN);
    }

    QDialog::accept();
}
