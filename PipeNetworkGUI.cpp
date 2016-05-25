/*
  PipeNetworkGUI.cpp
  Functionality of the main graphical interface
*/

#include "PipeNetworkGUI.h"
#include "ui_pipenetworkgui.h"

#include <QMessageBox>
#include <QFileDialog>                              // known qt 4.8.1 bug: QFileDialog leads to memory leak
#include <QFile>


PipeNetworkGUI::PipeNetworkGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PipeNetworkGUI)
{
    ui->setupUi(this);

    myGraphicsScene = new QGraphicsScene(this);
    ui->graphicsView->setScene(myGraphicsScene);

    ui->fluidSelection->addItems(PipeNetworkSystem::getFluidNamesList());
    updateCustomFluid();

    currentPipeSystem.setGUIProperties(myGraphicsScene, ui->resultsList->topLevelItem(0));
}

PipeNetworkGUI::~PipeNetworkGUI()
{
    delete ui;
}


void PipeNetworkGUI::closeEvent(QCloseEvent *event)
{
    switch (confirmSaveDialog("Quit Program Dialog"))
    {
    case QMessageBox::Save:
        if (!saveAction())      // saves file before exiting.
            event->ignore();
        break;

    case QMessageBox::Cancel:
        event->ignore();
        break;
    }
}


void PipeNetworkGUI::newAction(void)
{
    switch (confirmSaveDialog("New File Dialog"))
    {
    case QMessageBox::Save:
        if(saveAction())
            currentPipeSystem.clearItems();
        break;

    case QMessageBox::Discard:
        currentPipeSystem.clearItems();
        break;
    }
}

void PipeNetworkGUI::openAction(void)
{
    switch (confirmSaveDialog("Open File Dialog"))
    {
    case QMessageBox::Save:
        if(saveAction())
            loadFile();
        break;

    case QMessageBox::Discard:
        loadFile();
        break;
    }
}

bool PipeNetworkGUI::saveAction(void)
{   //returns whether save was successful
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", ".pns", "PipeNetworkSystem File (*.pns)");

    if (!fileName.isEmpty())
    {
        if ((fileName.section("/",-1) != ".pns"))
        {
            QFile saveToFile(fileName);
            if (saveToFile.open(QIODevice::WriteOnly))
            {
                if (saveToFile.write(this->saveState()) != -1)
                {
                    saveToFile.close();
                    return true;
                }
                else
                    QMessageBox::critical(this, "Failed to Save", "Write to file failed.");
            }
            else
                QMessageBox::critical(this, "Failed to Save", "Write access not allowed.");
        }
        else
            QMessageBox::critical(this, "Failed to Save", "Invalid file name.");
    }

    return false;       // if any error occurs or dialog is closed.
}


void PipeNetworkGUI::showAbout(void)
{
    QMessageBox::about(this, "About", Constants::SOFTWARE_NAME + "\n" + Constants::AUTHORS + "\n" + Constants::PROJECT_VERSION);
}


void PipeNetworkGUI::updateFluidSelection()
{
    if (ui->fluidSelection->currentIndex() == 0)
    {
        ui->customDensityBox->setEnabled(true);
        ui->customViscosityBox->setEnabled(true);
    }
    else
    {
        ui->customDensityBox->setEnabled(false);
        ui->customViscosityBox->setEnabled(false);

        const Fluid* tempPtr = &(PipeNetworkSystem::getFluidList().at(ui->fluidSelection->currentIndex() - 1));
        ui->customDensityBox->setValue(tempPtr->getDensity());
        ui->customViscosityBox->setValue(tempPtr->getDynamicViscosity()*1e6);
    }
}


void PipeNetworkGUI::updateCustomFluid()
{
    currentPipeSystem.editSystemFluid(ui->customDensityBox->value(), ui->customViscosityBox->value()* 1e-6);
}

void PipeNetworkGUI::clearAll()
{
    if (QMessageBox::warning(this,"Clear All Confirmation", "Are sure you want to clear all data? This cannot be undone.", QMessageBox::Yes, QMessageBox::Cancel) == QMessageBox::Yes)
        currentPipeSystem.clearItems();
}

void PipeNetworkGUI::runSimulation()
{
    currentPipeSystem.systemSolver();
}

void PipeNetworkGUI::addItem()
{
    if (ui->addItemIndex->currentIndex() == 0)
        currentPipeSystem.addPipe();
    else if (ui->addItemIndex->currentIndex() == 1)
        currentPipeSystem.addValve();
    else
        QMessageBox::about(this, "Missing Functionality", "This functionality will be added in the later versions.");
}

void PipeNetworkGUI::loadFile(void)
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", ".pns", "PipeNetworkSystem File (*.pns)");

    if (!fileName.isEmpty())
    {
        QFile openThisFile(fileName);
        (openThisFile.open(QIODevice::ReadOnly)) ? this->restoreState(openThisFile.readAll())   :   QMessageBox::warning(this,"Open File", "Read access not allowed.");
    }
}

int PipeNetworkGUI::confirmSaveDialog(QString A)    // shows a confirmation dialog. "A" = title of the dialog window
{
    QMessageBox confirmSave;

    confirmSave.setWindowTitle(A);
    confirmSave.setText("Any unsaved progress will be lost.");
    confirmSave.setInformativeText("Do you want to save your progress?");
    confirmSave.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    confirmSave.setDefaultButton(QMessageBox::Cancel);

    return confirmSave.exec();
}
