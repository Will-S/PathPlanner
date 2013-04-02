/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qSlicerPathPlannerTrajectoryItem_h
#define __qSlicerPathPlannerTrajectoryItem_h

// Standards
#include <sstream>

// Qt includes
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QTime>

// VTK includes
#include <ctkVTKObject.h>

class vtkMRMLAnnotationFiducialNode;
class vtkMRMLAnnotationRulerNode;

class qSlicerPathPlannerTrajectoryItem : public QObject, public QTableWidgetItem
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
