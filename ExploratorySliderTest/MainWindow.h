/*=========================================================================

  Name:        MainWindow.h

  Author:      David Borland

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: The main window for the ExploratorySliderTest visualization program

=========================================================================*/


#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>

#include <QDoubleSlider.h>

#include "ui_MainWindow.h"

class VTKPipeline;


class MainWindow : public QMainWindow, private Ui_MainWindow {
    Q_OBJECT

public:
    // Constructor/destructor
    MainWindow(QWidget* parent = NULL);
    virtual ~MainWindow();

public slots:
    // For double slider
    virtual void doubleSlider_valueChanged(double value);

    // Use Qt's auto-connect magic to tie GUI widgets to slots,
    // removing the need to call connect() explicitly.
    // Names of the methods must follow the naming convention
    // on_<widget name>_<signal name>(<signal parameters>).

    // Menu events
    virtual void on_actionOpenVolume_triggered();
    virtual void on_actionExit_triggered();

    // Widget events
    virtual void on_minSpinBox_editingFinished();
    virtual void on_maxSpinBox_editingFinished();
    virtual void on_exploratorySlider_valueChanged(double value);
    virtual void on_generateVolumeButton_clicked();

protected:
    // The linear double slider
    QDoubleSlider* doubleSlider;

    // The visualization pipeline object
	VTKPipeline* pipeline;

    // Create the VTK pipeline object
    void CreatePipeline();

    // Set GUI widget values from the VTK pipeline
    void RefreshGUI();
};


#endif