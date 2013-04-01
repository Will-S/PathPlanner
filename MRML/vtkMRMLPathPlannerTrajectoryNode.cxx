#include "vtkMRMLPathPlannerTrajectoryNode.h"

#include "vtkMRMLAnnotationFiducialNode.h"
#include "vtkMRMLAnnotationRulerNode.h"

#include <vtkObjectFactory.h>

//----------------------------------------------------------------------------
vtkMRMLNodeNewMacro(vtkMRMLPathPlannerTrajectoryNode);


//----------------------------------------------------------------------------
vtkMRMLPathPlannerTrajectoryNode::vtkMRMLPathPlannerTrajectoryNode()
{
  this->HideFromEditors = false;
}

//----------------------------------------------------------------------------
vtkMRMLPathPlannerTrajectoryNode::~vtkMRMLPathPlannerTrajectoryNode()
{
}

//----------------------------------------------------------------------------
void vtkMRMLPathPlannerTrajectoryNode::WriteXML(ostream& of, int nIndent)
{
  Superclass::WriteXML(of, nIndent);
}


//----------------------------------------------------------------------------
void vtkMRMLPathPlannerTrajectoryNode::ReadXMLAttributes(const char** atts)
{
  Superclass::ReadXMLAttributes(atts);
}

//----------------------------------------------------------------------------
void vtkMRMLPathPlannerTrajectoryNode::Copy(vtkMRMLNode *anode)
{
  Superclass::Copy(anode);
}

//-----------------------------------------------------------
void vtkMRMLPathPlannerTrajectoryNode::UpdateScene(vtkMRMLScene *scene)
{
  Superclass::UpdateScene(scene);
}

//---------------------------------------------------------------------------
void vtkMRMLPathPlannerTrajectoryNode::ProcessMRMLEvents ( vtkObject *caller,
                                           unsigned long event, 
                                           void *callData )
{
  Superclass::ProcessMRMLEvents(caller, event, callData);
}
