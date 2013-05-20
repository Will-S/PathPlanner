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

// SlicerQt includes
#include "qSlicerCoreApplication.h"
#include "qSlicerPathPlannerTrajectoryItem.h"

// MRML includes
#include "vtkMRMLAnnotationFiducialNode.h"
#include "vtkMRMLAnnotationRulerNode.h"
#include "vtkMRMLScene.h"

// --------------------------------------------------------------------------
qSlicerPathPlannerTrajectoryItem
::qSlicerPathPlannerTrajectoryItem() : QTableWidgetItem()
{
  this->EntryPoint  = NULL;
  this->TargetPoint = NULL;
  this->Trajectory  = NULL;
}

// --------------------------------------------------------------------------
qSlicerPathPlannerTrajectoryItem::
~qSlicerPathPlannerTrajectoryItem()
{
  if (this->Trajectory)
    {
    this->Trajectory->Delete();
    }
}

// --------------------------------------------------------------------------
void qSlicerPathPlannerTrajectoryItem::
setEntryPoint(vtkMRMLAnnotationFiducialNode* entryPoint)
{
  if (entryPoint)
    {
    qvtkReconnect(this->EntryPoint, entryPoint, vtkCommand::ModifiedEvent,
                  this, SLOT(updateItem()));
    this->EntryPoint = entryPoint;
    this->updateItem();
    }
}

// --------------------------------------------------------------------------
vtkMRMLAnnotationFiducialNode* qSlicerPathPlannerTrajectoryItem::
entryPoint()
{
  return this->EntryPoint;
}

// --------------------------------------------------------------------------
void qSlicerPathPlannerTrajectoryItem::
setTargetPoint(vtkMRMLAnnotationFiducialNode* targetPoint)
{
  if (targetPoint)
    {
    qvtkReconnect(this->TargetPoint, targetPoint, vtkCommand::ModifiedEvent,
                  this, SLOT(updateItem()));
    this->TargetPoint = targetPoint;
    this->updateItem();
    }
}

// --------------------------------------------------------------------------
vtkMRMLAnnotationFiducialNode* qSlicerPathPlannerTrajectoryItem::
targetPoint()
{
  return this->TargetPoint;
}

// --------------------------------------------------------------------------
void qSlicerPathPlannerTrajectoryItem::
setTrajectory(vtkMRMLAnnotationRulerNode* trajectory)
{
  if (trajectory)
    {
    qvtkReconnect(this->Trajectory, trajectory, vtkCommand::ModifiedEvent,
                  this, SLOT(updateItem()));
    this->Trajectory = trajectory;
    this->updateItem();
    }
}

// --------------------------------------------------------------------------
vtkMRMLAnnotationRulerNode* qSlicerPathPlannerTrajectoryItem::
trajectoryNode()
{
  return this->Trajectory;
}


// --------------------------------------------------------------------------
void qSlicerPathPlannerTrajectoryItem::
updateItem()
{
  // TODO: Remove ruler if one of the point removed
  if (!this->EntryPoint || !this->TargetPoint)
    {
    return;
    }
  
  if (!this->Trajectory)
    {
    // Points are set but trajectory doesn't exist yet. Create a new one.
    this->Trajectory = vtkMRMLAnnotationRulerNode::New();
    }

  // Set ruler points
  // Convention: Point1 -> Entry Point
  //             Point2 -> Target Point
  double targetPosition[3];
  double entryPosition[3];
  this->TargetPoint->GetFiducialCoordinates(targetPosition);
  this->EntryPoint->GetFiducialCoordinates(entryPosition);

  this->Trajectory->SetPosition1(entryPosition);
  this->Trajectory->SetPosition2(targetPosition);

  // Update table widget
  double itemRow = this->row();
  QTableWidget* tableWidget = this->tableWidget();
  if (!tableWidget)
    {
    return;
    }

  // -- Update ruler name
  QString itemName = tableWidget->item(itemRow,0)->text();
  QString rulerName(this->Trajectory->GetName());
  if (itemName.isEmpty())
    {
    // No name set. Use ruler name.
    tableWidget->item(itemRow,0)->setText(rulerName.toStdString().c_str());
    }
  else
    {
    if (itemName.compare(rulerName) != 0)
      {
      // Item name is different from ruler name
      // Update ruler name
      this->Trajectory->SetName(itemName.toStdString().c_str());
      }
    }

  // -- Update entry cell
  tableWidget->item(itemRow,1)->setText(this->TargetPoint->GetName()); 
  
  // -- Update target cell
  tableWidget->item(itemRow,2)->setText(this->EntryPoint->GetName()); 
}
