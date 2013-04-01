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

==============================================================================*/

// Qt includes
#include <QDebug>

// SlicerQt includes
#include "qSlicerPathPlannerModuleWidget.h"
#include "ui_qSlicerPathPlannerModuleWidget.h"

#include "qSlicerPathPlannerFiducialItem.h"

#include "vtkSlicerAnnotationModuleLogic.h"
#include "qSlicerAbstractCoreModule.h"
#include "qSlicerCoreApplication.h"
#include "qSlicerModuleManager.h"

#include "vtkMRMLAnnotationFiducialNode.h"
#include "vtkMRMLAnnotationRulerNode.h"
#include "vtkMRMLAnnotationHierarchyNode.h"
#include "vtkMRMLPathPlannerTrajectoryNode.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerPathPlannerModuleWidgetPrivate: public Ui_qSlicerPathPlannerModuleWidget
{
public:
  qSlicerPathPlannerModuleWidgetPrivate();

  vtkMRMLPathPlannerTrajectoryNode *selectedTrajectoryNode;
};

//-----------------------------------------------------------------------------
// qSlicerPathPlannerModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerPathPlannerModuleWidgetPrivate::
qSlicerPathPlannerModuleWidgetPrivate()
{
  this->selectedTrajectoryNode = NULL;
}

//-----------------------------------------------------------------------------
// qSlicerPathPlannerModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerPathPlannerModuleWidget::
qSlicerPathPlannerModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerPathPlannerModuleWidgetPrivate )
{
}

//-----------------------------------------------------------------------------
qSlicerPathPlannerModuleWidget::
~qSlicerPathPlannerModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerPathPlannerModuleWidget::
setup()
{
  Q_D(qSlicerPathPlannerModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();

  // Entry table widget
  connect(d->EntryPointListNodeSelector, SIGNAL(nodeActivated(vtkMRMLNode*)),
	  this, SLOT(onEntryListNodeChanged(vtkMRMLNode*)));

  // Target table widget
  connect(d->TargetPointListNodeSelector, SIGNAL(nodeActivated(vtkMRMLNode*)),
	  this, SLOT(onTargetListNodeChanged(vtkMRMLNode*)));
  
  // Trajectory table widget
  connect(d->TrajectoryListNodeSelector, SIGNAL(nodeActivated(vtkMRMLNode*)),
	  this, SLOT(onTrajectoryListNodeChanged(vtkMRMLNode*)));

  connect(d->AddButton, SIGNAL(clicked()),
	  this, SLOT(onAddButtonClicked()));
	  
  connect(d->DeleteButton, SIGNAL(clicked()),
	  this, SLOT(onDeleteButtonClicked()));

  connect(d->UpdateButton, SIGNAL(clicked()),
	  this, SLOT(onUpdateButtonClicked()));

  connect(d->ClearButton, SIGNAL(clicked()),
	  this, SLOT(onClearButtonClicked()));

}

//-----------------------------------------------------------------------------
void qSlicerPathPlannerModuleWidget::
onEntryListNodeChanged(vtkMRMLNode* newList)
{
  Q_D(qSlicerPathPlannerModuleWidget);

  if (!newList || !d->EntryPointWidget)
    {
    return;
    }

  vtkMRMLAnnotationHierarchyNode* entryList = 
    vtkMRMLAnnotationHierarchyNode::SafeDownCast(newList);

  if (!entryList)
    {
    return;
    }

  // Observe new hierarchy node
  qvtkConnect(entryList, vtkMRMLAnnotationHierarchyNode::ChildNodeAddedEvent,
	      this, SLOT(refreshEntryView()));
  qvtkConnect(entryList, vtkMRMLAnnotationHierarchyNode::ChildNodeRemovedEvent,
	      this, SLOT(refreshEntryView()));

  // Update groupbox name
  std::stringstream groupBoxName;
  groupBoxName << "Entry Point : " << entryList->GetName();
  d->EntryGroupBox->setTitle(groupBoxName.str().c_str());

  // Update widget
  d->EntryPointWidget->setSelectedHierarchyNode(entryList);

  // Refresh view
  this->refreshEntryView();
}

//-----------------------------------------------------------------------------
void qSlicerPathPlannerModuleWidget::
onTargetListNodeChanged(vtkMRMLNode* newList)
{
  Q_D(qSlicerPathPlannerModuleWidget);

  if (!newList || !d->TargetPointWidget)
    {
    return;
    }

  vtkMRMLAnnotationHierarchyNode* targetList = 
    vtkMRMLAnnotationHierarchyNode::SafeDownCast(newList);

  if (!targetList)
    {
    return;
    }

  // Observe new hierarchy node
  qvtkConnect(targetList, vtkMRMLAnnotationHierarchyNode::ChildNodeAddedEvent,
	      this, SLOT(refreshTargetView()));
  qvtkConnect(targetList, vtkMRMLAnnotationHierarchyNode::ChildNodeRemovedEvent,
	      this, SLOT(refreshTargetView()));

  // Update groupbox name
  std::stringstream groupBoxName;
  groupBoxName << "Target Point : " << targetList->GetName();
  d->TargetGroupBox->setTitle(groupBoxName.str().c_str());

  // Update widget
  d->TargetPointWidget->setSelectedHierarchyNode(targetList);

  // Refresh view
  this->refreshTargetView();
}

//-----------------------------------------------------------------------------
void qSlicerPathPlannerModuleWidget::
addNewFiducialItem(QTableWidget* tableWidget, vtkMRMLAnnotationFiducialNode* fiducialNode)
{
  if (!tableWidget || !fiducialNode)
    {
    return;
    }
  
  int numberOfItems = tableWidget->rowCount(); 
  tableWidget->insertRow(numberOfItems);

  // Add row
  qSlicerPathPlannerFiducialItem* newItem =
    new qSlicerPathPlannerFiducialItem();
  tableWidget->setItem(numberOfItems, 0, newItem);
  tableWidget->setItem(numberOfItems, 1, new QTableWidgetItem());
  tableWidget->setItem(numberOfItems, 2, new QTableWidgetItem());
  tableWidget->setItem(numberOfItems, 3, new QTableWidgetItem());
  tableWidget->setItem(numberOfItems, 4, new QTableWidgetItem());
  newItem->setFiducialNode(fiducialNode);

  // Set only name editable
  tableWidget->item(numberOfItems,1)->setFlags(tableWidget->item(numberOfItems,1)->flags() & ~Qt::ItemIsEditable);
  tableWidget->item(numberOfItems,2)->setFlags(tableWidget->item(numberOfItems,2)->flags() & ~Qt::ItemIsEditable);
  tableWidget->item(numberOfItems,3)->setFlags(tableWidget->item(numberOfItems,3)->flags() & ~Qt::ItemIsEditable);
  tableWidget->item(numberOfItems,4)->setFlags(tableWidget->item(numberOfItems,4)->flags() & ~Qt::ItemIsEditable);

  // Automatic scroll to last item added
  tableWidget->scrollToItem(tableWidget->item(numberOfItems,1));

  // Update item if changed  
  connect(tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)),
          this, SLOT(onItemChanged(QTableWidgetItem*)));
}

//-----------------------------------------------------------------------------
void qSlicerPathPlannerModuleWidget::
onItemChanged(QTableWidgetItem *item)
{
  if (!item)
    {
    return;
    }

  qSlicerPathPlannerFiducialItem *itemChanged
    = dynamic_cast<qSlicerPathPlannerFiducialItem*>(item);
  
  if (!itemChanged)
    {
    return;
    }

  itemChanged->updateItem();
}

//-----------------------------------------------------------------------------
void qSlicerPathPlannerModuleWidget::
refreshEntryView()
{
  Q_D(qSlicerPathPlannerModuleWidget);

  vtkMRMLAnnotationHierarchyNode* entryList =
    d->EntryPointWidget->selectedHierarchyNode();
  
  if (!entryList)
    {
    return;
    }

  // Clear entry table widget
  d->EntryPointWidget->getTableWidget()->clearContents();
  d->EntryPointWidget->getTableWidget()->setRowCount(0);
  
  // Populate table with new items in the new list
  for(int i = 0; i < entryList->GetNumberOfChildrenNodes(); i++)
    {
    vtkMRMLAnnotationFiducialNode* fiducialPoint =
      vtkMRMLAnnotationFiducialNode::SafeDownCast(entryList->GetNthChildNode(i)->GetAssociatedNode());
    if (fiducialPoint)
      {
      this->addNewFiducialItem(d->EntryPointWidget->getTableWidget(), fiducialPoint);
      }
    }
}

//-----------------------------------------------------------------------------
void qSlicerPathPlannerModuleWidget::
refreshTargetView()
{
  Q_D(qSlicerPathPlannerModuleWidget);

  vtkMRMLAnnotationHierarchyNode* targetList =
    d->TargetPointWidget->selectedHierarchyNode();
  
  if (!targetList)
    {
    return;
    }

  // Clear target table widget
  d->TargetPointWidget->getTableWidget()->clearContents(); 
  d->TargetPointWidget->getTableWidget()->setRowCount(0);
 
  // Populate table with new items in the new list
  for(int i = 0; i < targetList->GetNumberOfChildrenNodes(); i++)
    {
    vtkMRMLAnnotationFiducialNode* fiducialPoint =
      vtkMRMLAnnotationFiducialNode::SafeDownCast(targetList->GetNthChildNode(i)->GetAssociatedNode());
    if (fiducialPoint)
      {
      this->addNewFiducialItem(d->TargetPointWidget->getTableWidget(), fiducialPoint);
      }
    }
}

//-----------------------------------------------------------------------------
void qSlicerPathPlannerModuleWidget::
onTrajectoryListNodeChanged(vtkMRMLNode* newList)
{
  Q_D(qSlicerPathPlannerModuleWidget);

  if (!newList)
    {
    return;
    }

  vtkMRMLPathPlannerTrajectoryNode* trajectoryList = 
    vtkMRMLPathPlannerTrajectoryNode::SafeDownCast(newList);

  if (!trajectoryList)
    {
    return;
    }

  // Update groupbox name
  std::stringstream groupBoxName;
  groupBoxName << "Trajectory : " << trajectoryList->GetName();
  d->TrajectoryGroupBox->setTitle(groupBoxName.str().c_str());

  // Update selected node
  d->selectedTrajectoryNode = trajectoryList;
}

//-----------------------------------------------------------------------------
void qSlicerPathPlannerModuleWidget::
onAddButtonClicked()
{
  Q_D(qSlicerPathPlannerModuleWidget);

  if (!d->EntryPointWidget | !d->TargetPointWidget)
    {
    return;
    }

  if (!d->EntryPointWidget->getTableWidget() |
      !d->TargetPointWidget->getTableWidget())
    {
    return;
    }

  // Get target position
  double targetRow = d->TargetPointWidget->getTableWidget()->currentRow();
  if (targetRow < 0)
    {
    return;
    }
   
  qSlicerPathPlannerFiducialItem* targetItem
    = dynamic_cast<qSlicerPathPlannerFiducialItem*>(d->TargetPointWidget->getTableWidget()->item(targetRow, 0));
  if (!targetItem)
      {
      return;
      }
      
  vtkMRMLAnnotationFiducialNode* targetFiducial
    = targetItem->getFiducialNode();
  if (!targetFiducial)
    {
    return;
    }

  // Get entry position
  double entryRow = d->EntryPointWidget->getTableWidget()->currentRow();
  if (entryRow < 0)
    {
    return;
    }

  qSlicerPathPlannerFiducialItem* entryItem
    = dynamic_cast<qSlicerPathPlannerFiducialItem*>(d->EntryPointWidget->getTableWidget()->item(entryRow, 0));
  if (!entryItem)
    {
    return;
    }

  vtkMRMLAnnotationFiducialNode* entryFiducial
    = entryItem->getFiducialNode();
  if (!entryFiducial)
    {
    return;
    }
  
  // Add new ruler
  this->addNewRulerItem(entryFiducial, targetFiducial);

  
}

//-----------------------------------------------------------------------------
void qSlicerPathPlannerModuleWidget::
onDeleteButtonClicked()
{
  Q_D(qSlicerPathPlannerModuleWidget);
}

//-----------------------------------------------------------------------------
void qSlicerPathPlannerModuleWidget::
onUpdateButtonClicked()
{
  Q_D(qSlicerPathPlannerModuleWidget);
}

//-----------------------------------------------------------------------------
void qSlicerPathPlannerModuleWidget::
onClearButtonClicked()
{
  Q_D(qSlicerPathPlannerModuleWidget);
}

//-----------------------------------------------------------------------------
void qSlicerPathPlannerModuleWidget::
addNewRulerItem(vtkMRMLAnnotationFiducialNode* entryPoint, vtkMRMLAnnotationFiducialNode* targetPoint)
{
  Q_D(qSlicerPathPlannerModuleWidget);

  if (!entryPoint | !targetPoint)
    {
    return;
    }

  if (!d->selectedTrajectoryNode)
    {
    return;
    }

  double rowCount = d->TrajectoryTableWidget->rowCount();

  // Check ruler not already existing
  for (int i = 0; i < rowCount; i++)
    {
    QString targetName
      = QString(d->TrajectoryTableWidget->item(i,1)->text());
    QString newTargetName(targetPoint->GetName());
    if (targetName.compare(newTargetName) == 0)
      {
      // Target found. Check if same entry point
      QString entryName
        = QString(d->TrajectoryTableWidget->item(i,2)->text());
      QString newEntryName(entryPoint->GetName());
      if (entryName.compare(newEntryName) == 0)
        {
        // Same entry point
        return;
        }
      }
    }


  // Set active hierachy node
  qSlicerAbstractCoreModule* annotationModule =
    qSlicerCoreApplication::application()->moduleManager()->module("Annotations");
  vtkSlicerAnnotationModuleLogic* annotationLogic;
  if (annotationModule)
    {
    annotationLogic = 
      vtkSlicerAnnotationModuleLogic::SafeDownCast(annotationModule->logic());
    }
  
  if (annotationLogic->GetActiveHierarchyNode() != d->selectedTrajectoryNode)
    {
    annotationLogic->SetActiveHierarchyNodeID(d->selectedTrajectoryNode->GetID());
    }

  // Get target point position
  double targetPosition[3];
  targetPoint->GetFiducialCoordinates(targetPosition);

  // Get entry point position
  double entryPosition[3];
  entryPoint->GetFiducialCoordinates(entryPosition);

  // Create new ruler
  vtkSmartPointer<vtkMRMLAnnotationRulerNode> rulerNode
    = vtkSmartPointer<vtkMRMLAnnotationRulerNode>::New();
  rulerNode->Initialize(this->mrmlScene());
  rulerNode->SetPosition1(entryPosition);
  rulerNode->SetPosition2(targetPosition);

  // Insert new row
  d->TrajectoryTableWidget->insertRow(rowCount);

  // Populate row
  d->TrajectoryTableWidget->setItem(rowCount, 0, new QTableWidgetItem(rulerNode->GetName()));
  d->TrajectoryTableWidget->setItem(rowCount, 1, new QTableWidgetItem(targetPoint->GetName()));
  d->TrajectoryTableWidget->setItem(rowCount, 2, new QTableWidgetItem(entryPoint->GetName()));

  // Set only name editable
  d->TrajectoryTableWidget->item(rowCount,1)->setFlags(d->TrajectoryTableWidget->item(rowCount,1)->flags() & ~Qt::ItemIsEditable);
  d->TrajectoryTableWidget->item(rowCount,2)->setFlags(d->TrajectoryTableWidget->item(rowCount,2)->flags() & ~Qt::ItemIsEditable);

  // Automatic scroll to last item added
  d->TrajectoryTableWidget->scrollToItem(d->TrajectoryTableWidget->item(rowCount,0));  
}
