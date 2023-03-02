/*=========================================================================

  Name:        vtkImageTestDataSource.h

  Author:      David Borland

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: Generates volumes for testing exploratory sliders.

=========================================================================*/

#include "vtkImageTestDataSource.h"

#include <vtkImageData.h>
#include <vtkImageGaussianSmooth.h>
#include <vtkImageProgressIterator.h>
#include <vtkImageShiftScale.h>
#include <vtkMath.h>
#include <vtkImageNoiseSource.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkSmartPointer.h>

vtkStandardNewMacro(vtkImageTestDataSource);

//----------------------------------------------------------------------------
vtkImageTestDataSource::vtkImageTestDataSource()
{
  this->WholeExtent[0] = 0;  this->WholeExtent[1] = 127;
  this->WholeExtent[2] = 0;  this->WholeExtent[3] = 127;
  this->WholeExtent[4] = 0;  this->WholeExtent[5] = 31;

  this->Images[0] = vtkSmartPointer<vtkImageData>::New();
  this->Images[1] = vtkSmartPointer<vtkImageData>::New();

  this->SetNumberOfInputPorts(0);
}


//----------------------------------------------------------------------------
void vtkImageTestDataSource::SetWholeExtent(int xMin, int xMax, 
                                            int yMin, int yMax,
                                            int zMin, int zMax)
{
  int modified = 0;
  
  if (this->WholeExtent[0] != xMin)
    {
    modified = 1;
    this->WholeExtent[0] = xMin ;
    }
  if (this->WholeExtent[1] != xMax)
    {
    modified = 1;
    this->WholeExtent[1] = xMax ;
    }
  if (this->WholeExtent[2] != yMin)
    {
    modified = 1;
    this->WholeExtent[2] = yMin ;
    }
  if (this->WholeExtent[3] != yMax)
    {
    modified = 1;
    this->WholeExtent[3] = yMax ;
    }
  if (this->WholeExtent[4] != zMin)
    {
    modified = 1;
    this->WholeExtent[4] = zMin ;
    }
  if (this->WholeExtent[5] != zMax)
    {
    modified = 1;
    this->WholeExtent[5] = zMax ;
    }
  if (modified)
    {
    this->Modified();
    }
}

//----------------------------------------------------------------------------
void vtkImageTestDataSource::SetImages(vtkImageData* triangleImage, vtkImageData* squareImage)
{
  // XXX: Need to do some size checking and setting of the filter output size to the image size...

  this->Images[0] = triangleImage;
  this->Images[1] = squareImage;
}

//----------------------------------------------------------------------------
int vtkImageTestDataSource::RequestInformation (
  vtkInformation * vtkNotUsed(request),
  vtkInformationVector** vtkNotUsed( inputVector ),
  vtkInformationVector *outputVector)
{
  // get the info objects
  vtkInformation* outInfo = outputVector->GetInformationObject(0);

  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),
               this->WholeExtent,6);

  double x = this->WholeExtent[1] - this->WholeExtent[0] + 1;
  double y = this->WholeExtent[3] - this->WholeExtent[2] + 1;
  double z = this->WholeExtent[5] - this->WholeExtent[4] + 1;

  double spacing[3];
  spacing[0] = 1.0;
  spacing[1] = x / y;
  spacing[2] = x / z;

  outInfo->Set(vtkDataObject::SPACING(),spacing,3);
//  outInfo->Set(vtkDataObject::ORIGIN(),origin,3);

  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_DOUBLE, 1);

  return 1;
}

void vtkImageTestDataSource::ExecuteData(vtkDataObject *output)
{
  vtkImageData *data = this->AllocateOutputData(output);
  double *outPtr;
  int idxX, idxY, idxZ;
  int maxX, maxY, maxZ;
  vtkIdType outIncX, outIncY, outIncZ;
  int *outExt;
  unsigned long count = 0;
  unsigned long target;
  
  if (data->GetScalarType() != VTK_DOUBLE)
    {
    vtkErrorMacro("Execute: This source only outputs doubles");
    }
  
  outExt = data->GetExtent();
  
  // find the region to loop over
  maxX = outExt[1] - outExt[0];
  maxY = outExt[3] - outExt[2]; 
  maxZ = outExt[5] - outExt[4];
  
  // Get increments to march through data 
  data->GetContinuousIncrements(outExt, outIncX, outIncY, outIncZ);
  outPtr = static_cast<double*>(data->GetScalarPointer(outExt[0],outExt[2],outExt[4]));

  target = static_cast<unsigned long>((maxZ+1)*(maxY+1)/50.0);
  target++;

  // Make a noise image
  vtkSmartPointer<vtkImageNoiseSource> noise = vtkSmartPointer<vtkImageNoiseSource>::New();
  noise->SetWholeExtent(this->WholeExtent);
  noise->SetMinimum(0.0);
  noise->SetMaximum(1.0);

  // Set up randomized order for standard deviations of Gaussian kernels
  double sigmas[] = { 1.0, 5.0 };
  if (rand() > RAND_MAX / 2) 
    {
    double temp = sigmas[0];
    sigmas[0] = sigmas[1];
    sigmas[1] = temp;
    }

  // Blur the noise image
  // XXX: Can probably improve performance with seperable filter or FFT
  vtkSmartPointer<vtkImageGaussianSmooth> smooth = vtkSmartPointer<vtkImageGaussianSmooth>::New();
  smooth->SetInputConnection(noise->GetOutputPort());
  smooth->SetRadiusFactor((this->WholeExtent[1] - this->WholeExtent[0]) / 8.0);
  smooth->SetStandardDeviation(sigmas[0]);

  // Normalize the blurred image
  vtkSmartPointer<vtkImageShiftScale> shiftScale = vtkSmartPointer<vtkImageShiftScale>::New();
  shiftScale->SetInputConnection(smooth->GetOutputPort());

  double dataScale = 100.0;

  double max = 0.0;

  int imageIndex = rand() % (this->WholeExtent[5] - this->WholeExtent[4]);

  printf("imageIndex: %d\n\n", imageIndex);


  // Loop through ouput pixels
  for (idxZ = 0; idxZ <= maxZ; idxZ++)
    {
    // Pointer to image data
    double* p;

    // Scale factor for this layer
    double layerScale = 1.0;

    // Max value for this layer
    double layerMax = 0.0;

    if (idxZ == imageIndex)
      {
      // Get the image data
      p = static_cast<double*>(this->Images[rand() % 2]->GetScalarPointer());

      // Switch sigmas for subsequent layers
      smooth->SetStandardDeviation(sigmas[1]);
      }
    else 
      {
      // Create new noisy image
      noise->Modified();

      // Smooth the noise
      smooth->Update();

      // Normalize to [0, 1]
      double* range = smooth->GetOutput()->GetScalarRange();
      shiftScale->SetShift(-range[0]);
      shiftScale->SetScale(1.0 / (range[1] - range[0]));
      shiftScale->Update();

      range = shiftScale->GetOutput()->GetScalarRange();
      printf("%f %f\n", range[0], range[1]);

      // Get the normalized data
      p = static_cast<double*>(shiftScale->GetOutput()->GetScalarPointer());
    
      // Compute the scale for this layer
      if (abs(idxZ - imageIndex) != 1)
        {
        layerScale = (double)rand() / (double)RAND_MAX * dataScale;
        }
      }

    for (idxY = 0; !this->AbortExecute && idxY <= maxY; idxY++)
      {
      if (!(count%target))
        {
        this->UpdateProgress(count/(50.0*target));
        }
      count++;
      for (idxX = 0; idxX <= maxX; idxX++)
        {
        double value = *p * layerScale + max;
        *outPtr = value;
        outPtr++;
        p++;
        if (value > layerMax) 
          {
          layerMax = value;
          }
        }
      outPtr += outIncY;
      p += outIncY;
      }
    outPtr += outIncZ;
    max = layerMax;
    }


  // Normalize to [0, 100]
  int* dims = data->GetDimensions();
  int* ext = data->GetWholeExtent();

  outPtr = static_cast<double*>(data->GetScalarPointer(outExt[0],outExt[2],outExt[4]));
  for (idxZ = 0; idxZ <= maxZ; idxZ++)
    {
    for (idxY = 0; !this->AbortExecute && idxY <= maxY; idxY++)
      {
      for (idxX = 0; idxX <= maxX; idxX++)
        {
        *outPtr = (*outPtr - 0) / (max - 0) * 100.0;
        outPtr++;
        }
      outPtr += outIncY;
      }
    outPtr += outIncZ;
    }
}

void vtkImageTestDataSource::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

