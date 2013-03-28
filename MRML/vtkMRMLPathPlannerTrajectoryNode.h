#ifndef __vtkMRMLPathPlannerTrajectoryNode_h
#define __vtkMRMLPathPlannerTrajectoryNode_h

#include "vtkSlicerPathPlannerModuleMRMLExport.h"
#include "vtkMRMLAnnotationHierarchyNode.h" 

class vtkMRMLNode;
class vtkMRMLScene;
class vtkMRMLAnnotationFiducialNode;
class vtkMRMLAnnotationRulerNode;

/// \ingroup Slicer_QtModules_Annotation
class  VTK_SLICER_ANNOTATIONS_MODULE_MRML_EXPORT vtkMRMLPathPlannerTrajectoryNode : public vtkMRMLAnnotationHierarchyNode
{
public:
  static vtkMRMLPathPlannerTrajectoryNode *New();
  vtkTypeMacro(vtkMRMLPathPlannerTrajectoryNode, vtkMRMLAnnotationHierarchyNode);

  //--------------------------------------------------------------------------
  // MRMLNode methods
  //--------------------------------------------------------------------------

  virtual vtkMRMLNode* CreateNodeInstance();
  // Description:
  // Get node XML tag name (like Volume, Model)
  virtual const char* GetNodeTagName() {return "PathPlannerTrajectory";};

  virtual const char* GetIcon() {return ":/Icons/PathPlannerTrajectory.png";};

  // Description:
  // Read node attributes from XML file
  virtual void ReadXMLAttributes( const char** atts);
  
  // Description:
  // Write this node's information to a MRML file in XML format.
  virtual void WriteXML(ostream& of, int indent);

  virtual void UpdateScene(vtkMRMLScene *scene);

  // Description:
  // Copy the node's attributes to this object
  virtual void Copy(vtkMRMLNode *node);

  // Description:
  // alternative method to propagate events generated in Display nodes
  virtual void ProcessMRMLEvents ( vtkObject * /*caller*/, 
                                   unsigned long /*event*/, 
                                   void * /*callData*/ );

protected:
  vtkMRMLPathPlannerTrajectoryNode();
  ~vtkMRMLPathPlannerTrajectoryNode();
  void operator=(const vtkMRMLPathPlannerTrajectoryNode&); 

  typedef std::pair<vtkMRMLAnnotationFiducialNode*, vtkMRMLAnnotationFiducialNode*> FiducialPair;
  typedef std::map<FiducialPair, vtkMRMLAnnotationRulerNode*> FiducialRuler;

  FiducialRuler RulerList;
};

#endif
