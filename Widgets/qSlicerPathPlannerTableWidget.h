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

#ifndef __qSlicerPathPlannerTableWidget_h
#define __qSlicerPathPlannerTableWidget_h

// VTK includes
#include <ctkVTKObject.h>

// SlicerQt includes
#include "qSlicerPathPlannerModuleWidgetsExport.h"
#include "qSlicerWidget.h"

// Qt includes
#include <QTableWidget>

class qSlicerPathPlannerTableWidgetPrivate;
class vtkMRMLNode;
class vtkMRMLScene;
class vtkMRMLAnnotationHierarchyNode;

class Q_SLICER_MODULE_PATHPLANNER_WIDGETS_EXPORT qSlicerPathPlannerTableWidget
//  : public QAbstractTableModel
  : public qSlicerWidget
{
  Q_OBJECT
  QVTK_OBJECT
    //Q_PROPERTY(bool noneEnabled READ noneEnabled WRITE setNoneEnabled)

public:
  typedef qSlicerWidget Superclass;
  qSlicerPathPlannerTableWidget(QWidget *parent=0);
  virtual ~qSlicerPathPlannerTableWidget();

  QTableWidget* getTableWidget();
  void setSelectedHierarchyNode(vtkMRMLAnnotationHierarchyNode* selectedNode);
  vtkMRMLAnnotationHierarchyNode* selectedHierarchyNode();

public slots:
  void onAddButtonClicked();
  void onDeleteButtonClicked();
  void onClearButtonClicked();
  void onSelectionChanged();
  void onCellChanged(int row, int column);

protected:
  QScopedPointer<qSlicerPathPlannerTableWidgetPrivate> d_ptr;
  
private:
  Q_DECLARE_PRIVATE(qSlicerPathPlannerTableWidget);
  Q_DISABLE_COPY(qSlicerPathPlannerTableWidget);

};

#endif // __qSlicerPathPlannerTableWidget_h
