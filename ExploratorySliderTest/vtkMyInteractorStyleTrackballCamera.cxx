/*=========================================================================

  Name:        vtkMyInteractorStyleTrackballCamera.cxx

  Author:      David Borland

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: Overrides keypreses and only allows rotation around a 
               single axis.

=========================================================================*/

#include "vtkMyInteractorStyleTrackballCamera.h"

#include <vtkCamera.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

vtkCxxRevisionMacro(vtkMyInteractorStyleTrackballCamera, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkMyInteractorStyleTrackballCamera);

//----------------------------------------------------------------------------
void vtkMyInteractorStyleTrackballCamera::OnChar()
{
}

//----------------------------------------------------------------------------
void vtkMyInteractorStyleTrackballCamera::Rotate()
{
  if (this->CurrentRenderer == NULL)
    {
    return;
    }
  
  vtkRenderWindowInteractor *rwi = this->Interactor;

  int dy = rwi->GetEventPosition()[1] - rwi->GetLastEventPosition()[1];
  
  int *size = this->CurrentRenderer->GetRenderWindow()->GetSize();

  double delta_elevation = -20.0 / size[1];
  
  double ryf = dy * delta_elevation * this->MotionFactor;
  
  vtkCamera *camera = this->CurrentRenderer->GetActiveCamera();
  camera->Elevation(ryf);
  camera->OrthogonalizeViewUp();

  if (this->AutoAdjustCameraClippingRange)
    {
    this->CurrentRenderer->ResetCameraClippingRange();
    }

  if (rwi->GetLightFollowCamera()) 
    {
    this->CurrentRenderer->UpdateLightsGeometryToFollowCamera();
    }

  rwi->Render();
}

//----------------------------------------------------------------------------
void vtkMyInteractorStyleTrackballCamera::Spin()
{
}

//----------------------------------------------------------------------------
void vtkMyInteractorStyleTrackballCamera::Pan()
{
}