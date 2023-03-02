/*=========================================================================

  Name:        VTKPipeline.cpp

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: Container class for all Voluminous VTK code.

=========================================================================*/


#include "VTKPipeline.h"

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkContourFilter.h>
#include <vtkCubeAxesActor.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkImageExtractComponents.h>
#include <vtkImageMapper.h>
#include <vtkImageResize.h>
#include <vtkImageShiftScale.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkOutlineFilter.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkStructuredPointsReader.h>
#include <vtkTubeFilter.h>
#include <vtkXMLImageDataReader.h>

#include <algorithm>

#include "vtkImageTestDataSource.h"
#include "vtkMyInteractorStyleTrackballCamera.h"


#include <vtkImageSinusoidSource.h>
#include <vtkImageNoiseSource.h>


VTKPipeline::VTKPipeline(vtkRenderWindowInteractor* rwi, 
                         vtkImageData* triangle, vtkImageData* square,
                         const std::string& shader)
: interactor(rwi), shader(shader) {
    // Renderer
    renderer = vtkSmartPointer<vtkRenderer>::New();

//    renderer->SetBackground(0.95, 0.95, 0.9);
//    renderer->SetBackground(1.0, 1.0, 1.0);
//    renderer->SetBackground(0.0, 0.0, 0.0);
    renderer->SetBackground(0.15, 0.15, 0.2);
    renderer->LightFollowCameraOff();

    interactor->GetRenderWindow()->AddRenderer(renderer);


    vtkSmartPointer<vtkMyInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkMyInteractorStyleTrackballCamera>::New();
    interactor->SetInteractorStyle(style);

    
    // Create the test data source
    testData = vtkSmartPointer<vtkImageTestDataSource>::New();
    testData->SetImages(ProcessImage(triangle), ProcessImage(square));


    // No volume yet
    volume = NULL;


    // No data range yet
    dataRange[0] = dataRange[1] = 0.0;
}

VTKPipeline::~VTKPipeline() {
    interactor->GetRenderWindow()->RemoveRenderer(renderer);
}


bool VTKPipeline::OpenVolume(const std::string& fileName) {   
    vtkSmartPointer<vtkAlgorithm> reader;

    if (fileName.rfind(".vtk") == fileName.length() - 4) {
        // Load legacy VTK structured point data
        vtkSmartPointer<vtkStructuredPointsReader> spReader = vtkSmartPointer<vtkStructuredPointsReader>::New();
        spReader->SetFileName(fileName.c_str());
        
        if (spReader->IsFileStructuredPoints()) {
            reader = spReader;

            reader->Update();
        }
    }
    else if (fileName.rfind(".vti") == fileName.length() - 4) {
        // Load VTK XML image data file
        vtkSmartPointer<vtkXMLImageDataReader> iReader = vtkSmartPointer<vtkXMLImageDataReader>::New();
        iReader->SetFileName(fileName.c_str());
        
        reader = iReader;

        reader->Update();
    }  

    
    if (reader == NULL) {
        std::cout << "VTKPipeline::OpenVolume() : Volume must be in .vtk structured points or .vti format." << std::endl;

        return false;
    }
            

    if (reader->GetOutputDataObject(0) == NULL) {
       std::cout << "VTKPipeline::OpenVolume() : Could not open " << fileName << std::endl;

       return false;
    }


    // Create the visualization
    CreateVisualization(reader);


    return true;
}


void VTKPipeline::GenerateVolume() {   
    // Update the test data source
    testData->Update();

    // Create the visualization
    CreateVisualization(testData);
}


double VTKPipeline::GetIsovalue() {
    return isosurface->GetValue(0);
}

void VTKPipeline::SetIsovalue(double value) {
    isosurface->SetValue(0, value);
}


void VTKPipeline::GetDataRange(double range[2]) {
    range[0] = dataRange[0];
    range[1] = dataRange[1];
}


void VTKPipeline::Render() {
    interactor->Render();
}


void VTKPipeline::CreateVisualization(vtkAlgorithm* input) {
    // Get the volume information
    volume = (vtkImageData*) input->GetOutputDataObject(0);

    volume->GetScalarRange(dataRange);

    double bounds[6];
    volume->GetBounds(bounds);

    double origin[3];
    volume->GetOrigin(origin);

    double size[3];
    for (int i = 0; i < 3; i++) {
        size[i] = (bounds[2 * i + 1] - bounds[2 * i]);
    }

    double center[3];
    for (int i = 0; i < 3; i++) {
        center[i] = origin[i] + size[i] * 0.5;
    }


    // Create the outline    
    vtkSmartPointer<vtkOutlineFilter> outline = vtkSmartPointer<vtkOutlineFilter>::New();
    outline->SetInputConnection(input->GetOutputPort());
    
    // Remove one edge
    outline->Update();      
    vtkPolyData* pd = outline->GetOutput();
    pd->BuildLinks();
    pd->DeleteCell(2);
    pd->RemoveDeletedCells();


    vtkSmartPointer<vtkTubeFilter> tubeOutline = vtkSmartPointer<vtkTubeFilter>::New();
//    tubeOutline->SetInputConnection(outline->GetOutputPort());
tubeOutline->SetInput(pd);
    tubeOutline->SetRadius(size[0] * 0.002);
    
    vtkSmartPointer<vtkPolyDataMapper> outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    outlineMapper->SetInputConnection(tubeOutline->GetOutputPort());

    vtkSmartPointer<vtkActor> outlineActor = vtkSmartPointer<vtkActor>::New();
    outlineActor->SetMapper(outlineMapper);
//    outlineActor->GetProperty()->SetColor(0.75, 0.75, 0.75);
    double colorScale = 1.2;
    outlineActor->GetProperty()->SetColor(renderer->GetBackground()[0] * colorScale,
                                          renderer->GetBackground()[1] * colorScale,
                                          renderer->GetBackground()[2] * colorScale);
    outlineActor->GetProperty()->SetAmbient(1.0);
    outlineActor->GetProperty()->SetDiffuse(0.25);
    outlineActor->GetProperty()->SetSpecular(0.0);

    renderer->AddViewProp(outlineActor);


    // Create the isosurface
    isosurface = vtkSmartPointer<vtkContourFilter>::New();
    isosurface->SetInputConnection(input->GetOutputPort());
    isosurface->SetNumberOfContours(1);
    isosurface->SetValue(0, (dataRange[1] - dataRange[0]) / 2.0);
    isosurface->ComputeScalarsOff();
    isosurface->ComputeNormalsOn();
    isosurface->ComputeGradientsOff();

    vtkSmartPointer<vtkPolyDataMapper> isosurfaceMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
    isosurfaceMapper->SetInputConnection(isosurface->GetOutputPort());

    vtkSmartPointer<vtkActor> isosurfaceActor = vtkSmartPointer<vtkActor>::New();
    isosurfaceActor->SetMapper(isosurfaceMapper);

    vtkProperty* p = isosurfaceActor->GetProperty();
    p->SetAmbient(0.0);
    p->SetDiffuse(1.0);
    p->SetSpecular(0.0);
    p->SetSpecularColor(1.0, 1.0, 1.0);
    p->SetSpecularPower(50.0);
    p->LoadMaterialFromString(shader.c_str());  
    p->ShadingOn();

    p->SetColor(0.95, 0.95, 0.9);

    renderer->AddViewProp(isosurfaceActor);


    // Show the data
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Elevation(-60.0);

    Render();
}


vtkSmartPointer<vtkImageData> VTKPipeline::ProcessImage(vtkImageData* image) {    
    // Make sure the input image only have one component
    vtkSmartPointer<vtkImageExtractComponents> extract = vtkSmartPointer<vtkImageExtractComponents>::New();
    extract->SetInput(image);
    extract->SetComponents(0);
    extract->Update();

    double* range = extract->GetOutput()->GetScalarRange();

    // Make sure the input images are in the correct format
    vtkSmartPointer<vtkImageShiftScale> shiftScale = vtkSmartPointer<vtkImageShiftScale>::New();
    shiftScale->SetInputConnection(extract->GetOutputPort());
    shiftScale->SetShift(-range[0]);
    shiftScale->SetScale(1.0 / (range[1] - range[0]));
    shiftScale->SetOutputScalarTypeToDouble();
    shiftScale->Update();

    return shiftScale->GetOutput();
}