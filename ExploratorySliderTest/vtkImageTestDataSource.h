/*=========================================================================

  Name:        vtkImageTestDataSource.h

  Author:      David Borland

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: Generates volumes for testing exploratory sliders.

=========================================================================*/

#ifndef __vtkImageTestDataSource_h
#define __vtkImageTestDataSource_h


#include <vtkImageAlgorithm.h>

#include <vtkSmartPointer.h>


class vtkImageTestDataSource : public vtkImageAlgorithm 
{
public:
  static vtkImageTestDataSource *New();
  vtkTypeMacro(vtkImageTestDataSource,vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set how large of an image to generate.
  void SetWholeExtent(int xMinx, int xMax, int yMin, int yMax,
                      int zMin, int zMax);
  void SetWholeExtent(const int ext[6])
    {
    this->SetWholeExtent(ext[0], ext[1], ext[2], ext[3], ext[4], ext[5]);
    }

  // Description:
  // Set the images to be used
  void SetImages(vtkImageData* triangleImage, vtkImageData* squareImage);

protected:
  vtkImageTestDataSource();
  ~vtkImageTestDataSource() {};

  int WholeExtent[6];

  vtkSmartPointer<vtkImageData> Images[2];

  virtual int RequestInformation (vtkInformation *, vtkInformationVector**, vtkInformationVector *);
  virtual void ExecuteData(vtkDataObject *data);
private:
  vtkImageTestDataSource(const vtkImageTestDataSource&);  // Not implemented.
  void operator=(const vtkImageTestDataSource&);  // Not implemented.
};


#endif

  
