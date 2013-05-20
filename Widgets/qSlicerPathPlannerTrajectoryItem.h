/*==============================================================================

  Program: 3D Slicer
 
  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.
 
  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 
  This file was originally developed by Laurent Chauvin, Brigham and Women's
  Hospital. The project was supported by grants 5P01CA067165,
  5R01CA124377, 5R01CA138586, 2R44DE019322, 7R01CA124377,
  5R42CA137886, 8P41EB015898
 
==============================================================================*/

#ifndef __qSlicerPathPlannerTrajectoryItem_h
#define __qSlicerPathPlannerTrajectoryItem_h

#include "qSlicerPathPlannerModuleWidgetsExport.h"

// Standards
#include <sstream>

// Qt includes
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTime>

// VTK includes
#include <ctkVTKObject.h>

class vtkMRMLAnnotationFiducialNode;
class vtkMRMLAnnotationRulerNode;

class Q_SLICER_MODULE_PATHPLANNER_WIDGETS_EXPORT qSlicerPathPlannerTrajectoryItem
  : public QObject, public QTableWidgetItem
{
  Q_OBJECT
  QVTK_OBJECT
 public:
    qSlicerPathPlannerTrajectoryItem();
  ~qSlicerPathPlannerTrajectoryItem();
  
  // MRML Nodes
  void setEntryPoint(vtkMRMLAnnotationFiducialNode* entryPoint);
  vtkMRMLAnnotationFiducialNode* entryPoint();
  void setTargetPoint(vtkMRMLAnnotationFiducialNode* targetPoint);
  vtkMRMLAnnotationFiducialNode* targetPoint();
  void setTrajectory(vtkMRMLAnnotationRulerNode* trajectory);
  vtkMRMLAnnotationRulerNode* trajectoryNode();

 public slots:
   void updateItem();

 private:
  vtkMRMLAnnotationFiducialNode* EntryPoint;
  vtkMRMLAnnotationFiducialNode* TargetPoint;
  vtkMRMLAnnotationRulerNode* Trajectory;
};

#endif
