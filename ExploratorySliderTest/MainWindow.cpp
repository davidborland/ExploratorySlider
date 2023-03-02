/*=========================================================================

  Name:        MainWindow.cpp

  Author:      David Borland

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: The main window for the Voluminous visualization program

=========================================================================*/


#include "MainWindow.h"

#include <QFileDialog>
#include <QResource>

#include <vtkQImageToImageSource.h>

#include "VTKPipeline.h"


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // Create the GUI from the Qt Designer file
    setupUi(this);


    // The linear spin box is not visible.  It is just used to control
    // the QDoubleSlider attached to the linear slider.
    linearSpinBox->setVisible(false);

    // Set up the double slider
    doubleSlider = new QDoubleSlider(linearSlider, linearSpinBox, this);


    // Disable the widgets until we have data
    linearSlider->setEnabled(false);
    minSpinBox->setEnabled(false);
    maxSpinBox->setEnabled(false);

    exploratorySlider->setEnabled(false);


    // Connect sliders
    connect(doubleSlider, SIGNAL(valueChanged(double)), this, SLOT(doubleSlider_valueChanged(double)));
    connect(doubleSlider, SIGNAL(valueChanged(double)), valueLabel, SLOT(setNum(double)));
    connect(exploratorySlider, SIGNAL(valueChanged(double)), valueLabel, SLOT(setNum(double)));
    
    valueLabel->setNum(exploratorySlider->getValue());

    // Create the visualization pipeline
    pipeline = NULL;
    CreatePipeline();
}

MainWindow::~MainWindow() {
    delete pipeline;
}


void MainWindow::on_actionOpenVolume_triggered() {
    // Open a file dialog to read the VTK XML file
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Open Volume",
                                                    "",
                                                    "All Files (*);;Legacy VTK Files (*.vtk);;VTK XML ImageData Files (.vti)");

    // Check for file name
    if (fileName == "") {
        return;
    }


    // Disable the GUI until we have data
    exploratorySlider->setEnabled(false);


    // Create a new pipeline
    CreatePipeline();


    // Clear the screen
    pipeline->Render();

    
    // Load the volume
    pipeline->OpenVolume(fileName.toStdString());


    // Refresh the GUI
    RefreshGUI();
}

void MainWindow::on_actionExit_triggered() {
    qApp->exit();
}


///////////////////////////////////////////////////////////////////////////
// Respond to widget events

void MainWindow::doubleSlider_valueChanged(double value) {
    pipeline->SetIsovalue(value);
    pipeline->Render();
}

void MainWindow::on_minSpinBox_editingFinished() {
    maxSpinBox->setMinimum(minSpinBox->value());
    linearSpinBox->setMinimum(minSpinBox->value());

    // Force recomputation of slider position
    doubleSlider->setValue(doubleSlider->value());
}

void MainWindow::on_maxSpinBox_editingFinished() {
    minSpinBox->setMaximum(maxSpinBox->value());
    linearSpinBox->setMaximum(maxSpinBox->value());

    // Force recomputation of slider position
    doubleSlider->setValue(doubleSlider->value());
}

void MainWindow::on_exploratorySlider_valueChanged(double value) {
    pipeline->SetIsovalue(value);
    pipeline->Render();
}

void MainWindow::on_generateVolumeButton_clicked() {
    // Disable the slider until we have data
    exploratorySlider->setEnabled(false);


    // Create a new pipeline
    CreatePipeline();


    // Clear the screen
    pipeline->Render();


    // Generate the volume
    pipeline->GenerateVolume();


    // Refresh the GUI
    RefreshGUI();
}


void MainWindow::CreatePipeline() {
    // Load the images
    QImage triangleImage(":/triangle");
    vtkSmartPointer<vtkQImageToImageSource> triangle = vtkSmartPointer<vtkQImageToImageSource>::New();
    triangle->SetQImage(&triangleImage);
    triangle->Update();

    QImage squareImage(":/square");
    vtkSmartPointer<vtkQImageToImageSource> square = vtkSmartPointer<vtkQImageToImageSource>::New();
    square->SetQImage(&squareImage);
    square->Update();


    // Clean up the old visualization pipeline and create a new one
    if (pipeline) {
        delete pipeline;
    }

    pipeline = new VTKPipeline(qvtkWidget->GetInteractor(),
                               triangle->GetOutput(), square->GetOutput(),
                               reinterpret_cast<const char*>(QResource(":/shader").data()));
}

void MainWindow::RefreshGUI() {
    // Set the information from the data
    double range[2];
    pipeline->GetDataRange(range);

    double value = pipeline->GetIsovalue();

    // Set up linear slider via spin boxes
    minSpinBox->setRange(range[0], range[1]);
    minSpinBox->setValue(range[0]);

    maxSpinBox->setRange(range[0], range[1]);
    maxSpinBox->setValue(range[1]);

    linearSpinBox->setRange(range[0], range[1]);
    linearSpinBox->setValue(value);

    // Set up exploratory slider
    exploratorySlider->setRange(range[0], range[1]);
    exploratorySlider->setValue(value);
    exploratorySlider->setPivotValue(0.5);
    exploratorySlider->setExponent(1.0);

    // Enable the widgets
    minSpinBox->setEnabled(true);
    maxSpinBox->setEnabled(true);
    linearSlider->setEnabled(true);
    exploratorySlider->setEnabled(true);
}