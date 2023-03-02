/*=========================================================================

  Name:        MainWindow.cpp

  Author:      David Borland

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: The main window for the Voluminous visualization program

=========================================================================*/


#include "MainWindow.h"

#include <QColor>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QResource>

#include <QDoubleSlider.h>


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // Create the GUI from the Qt Designer file
    setupUi(this);


    // The linear spin box is not visible.  It is just used to control
    // the QDoubleSlider attached to the linear slider.
    linearSpinBox->setVisible(false);

    // Set up the double slider
    doubleSlider = new QDoubleSlider(linearSlider, linearSpinBox, this);


    // Connect slider
    connect(doubleSlider, SIGNAL(valueChanged(double)), this, SLOT(doubleSlider_valueChanged(double)));
//    connect(doubleSlider, SIGNAL(valueChanged(double)), valueLabel, SLOT(setNum(double)));
//    connect(exploratorySlider, SIGNAL(valueChanged(double)), valueLabel, SLOT(setNum(double)));


    // Value attributes
    min = 0.0;
    max = 1000.0;
    decimals = 4;

    double mid = min + (max - min) * 0.5;


    // Set up linear slider via spin boxes
    minSpinBox->setRange(min, max);
    minSpinBox->setValue(min);

    maxSpinBox->setRange(min, max);
    maxSpinBox->setValue(max);

    linearSpinBox->setRange(min, max);
    linearSpinBox->setValue(mid);

    // Set up exploratory slider
    exploratorySlider->setRange(min, max);
    exploratorySlider->setValue(mid);
    
    // Create the timer
    timer = new QElapsedTimer();

    // Generate a new value
    GenerateValue();
}

MainWindow::~MainWindow() {
    delete timer;
}


void MainWindow::on_actionExit_triggered() {
    qApp->exit();
}


///////////////////////////////////////////////////////////////////////////
// Respond to widget events

void MainWindow::doubleSlider_valueChanged(double value) {
    SetValueLabel(value);
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
    SetValueLabel(value);
}

void MainWindow::on_generateValueButton_clicked() {
    GenerateValue();
}


void MainWindow::GenerateValue() {
    printf("%d milliseconds elapsed\n", timer->elapsed());
    timer->start();


    double v = (double)qrand() / (double)RAND_MAX;
    targetValue = min + v * (max - min);

    // XXX: Need to truncate to decimals
}

void MainWindow::SetValueLabel(double value) {
    QString s;
    s.setNum(value, 'f', decimals);
    valueLabel->setText(s);

    if (value < targetValue) {
        valueLabel->setPaletteForegroundColor(QColor("blue"));
    }
    else {
        valueLabel->setPaletteForegroundColor(QColor("red"));
    }
}