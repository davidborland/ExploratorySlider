/*=========================================================================

  Name:        vtkMyInteractorStyleTrackballCamera.h

  Author:      David Borland

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: Overrides keypreses and only allows rotation around a 
               single axis.

=========================================================================*/

#ifndef __vtkMyInteractorStyleTrackballCamera_h
#define __vtkMyInteractorStyleTrackballCamera_h

#include <vtkInteractorStyleTrackballCamera.h>

class vtkMyInteractorStyleTrackballCamera : public vtkInteractorStyleTrackballCamera 
{
public:
  static vtkMyInteractorStyleTrackballCamera *New();
  vtkTypeRevisionMacro(vtkMyInteractorStyleTrackballCamera,vtkInteractorStyleTrackballCamera);

  virtual void OnChar();

  virtual void Rotate();
  virtual void Spin();
  virtual void Pan();

protected:
  vtkMyInteractorStyleTrackballCamera(){}
  ~vtkMyInteractorStyleTrackballCamera(){}

private:
  vtkMyInteractorStyleTrackballCamera(const vtkMyInteractorStyleTrackballCamera&);  // Not implemented.
  void operator=(const vtkMyInteractorStyleTrackballCamera&);  // Not implemented.
};

#endif
