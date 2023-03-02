/*=========================================================================

  Name:        VTKPipeline.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: Container class for all VTK code.

=========================================================================*/


#ifndef VTKPIPELINE_H
#define VTKPIPELINE_H

#include <string>
#include <vector>

#include <vtkSmartPointer.h>


class vtkAlgorithm;
class vtkContourFilter;
class vtkCubeAxesActor;
class vtkImageActor;
class vtkImageData;
class vtkImageTestDataSource;
class vtkRenderWindowInteractor;
class vtkRenderer;
class vtkXMLMaterial;


class VTKPipeline {
public:	
    VTKPipeline(vtkRenderWindowInteractor* rwi, 
                vtkImageData* triangle, vtkImageData* square,
                const std::string& shader);
    ~VTKPipeline();

    // Load data
    bool OpenVolume(const std::string& fileName);

    // Generate data
    void GenerateVolume();
    
    // Get/set isosurface value
    double GetIsovalue();
    void SetIsovalue(double value);

    // Get data range
    void GetDataRange(double range[2]);

    // Force a render
    void Render();

protected:
    // Create the visualization
    void CreateVisualization(vtkAlgorithm* input);

    // Process input images
    vtkSmartPointer<vtkImageData> ProcessImage(vtkImageData* image);

    // Rendering
    vtkSmartPointer<vtkRenderWindowInteractor> interactor;
    vtkSmartPointer<vtkRenderer> renderer;

    // The volume
    vtkSmartPointer<vtkImageData> volume;

    // Test data generator
    vtkSmartPointer<vtkImageTestDataSource> testData;

    // The isosurface
    vtkSmartPointer<vtkContourFilter> isosurface;

    // Shader string
    std::string shader;

    // The range of data values in the volume
    double dataRange[2];
};


#endif