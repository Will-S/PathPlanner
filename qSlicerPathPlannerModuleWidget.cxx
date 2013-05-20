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

// Qt includes
#include <QDebug>

// SlicerQt includes
#include "qSlicerPathPlannerModuleWidget.h"
#include "ui_qSlicerPathPlannerModuleWidget.h"

// Annotation logic
#include "vtkSlicerAnnotationModuleLogic.h"

// Slicer
#include "qSlicerAbstractCoreModule.h"
#include "qSlicerCoreApplication.h"
#include "qSlicerModuleManager.h"
#include "qSlicerPathPlannerFiducialItem.h"
#include "qSlicerPathPlannerTrajectoryItem.h"

// MRML
#include "vtkMRMLAnnotationFiducialNode.h"
#include "vtkMRMLAnnotationHierarchyNode.h"
#include "vtkMRMLAnnotationPointDisplayNode.h"
#include "vtkMRMLAnnotationRulerNode.h"
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

  connect(d->EntryPointWidget->getTableWidget(), SIGNAL(itemSelectionChanged()),
	  this, SLOT(onEntrySelectionChanged()));

  // Target table widget
  connect(d->TargetPointListNodeSelector, SIGNAL(nodeActivated(vtkMRMLNode*)),
	  this, SLOT(onTargetListNodeChanged(vtkMRMLNode*)));
  
  connect(d->TargetPointWidget->getTableWidget(), SIGNAL(itemSelectionChanged()),
	  this, SLOT(onTargetSelectionChanged()));

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

  connect(d->TrajectoryTableWidget, SIGNAL(cellClicked(int,int)),
	  this, SLOT(onTrajectoryCellClicked(int,int)));

  connect(d->TrajectoryTableWidget, SIGNAL(cellChanged(int,int)),
	  this, SLOT(onTrajectoryCellChanged(int,int)));

  // mrmlScene
  connect(this, SIGNAL(mrmlSceneChanged(vtkMRMLScene*)),
	  this, SLOT(onMRMLSceneChanged(vtkMRMLScene*)));


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
  Q_D(qSlicerPathPlannerModuleWidget);

  if (!tableWidget || !fiducialNode)
    {
    return;
    }
  
  // Set fiducial properties
  // Opacity: 0.3 by default
  fiducialNode->GetAnnotationPointDisplayNode()->SetOpacity(0.3);

  // Set fiducial name
  if (tableWidget == d->TargetPointWidget->getTableWidget())
    {
    std::stringstream targetName;
    targetName << "Target " << d->TargetPointWidget->getTableWidget()->rowCount()+1;
    fiducialNode->SetName(targetName.str().c_str());
    }
  else if (tableWidget == d->EntryPointWidget->getTableWidget())
    {
    std::stringstream entryName;
    entryName << "Entry " << d->EntryPointWidget->getTableWidget()->rowCount()+1;
    fiducialNode->SetName(entryName.str().c_str());
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

  // Automatic scroll and select last item added
  tableWidget->scrollToItem(tableWidget->item(numberOfItems,1));
  tableWidget->setCurrentCell(tableWidget->rowCount()-1,0);

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
      // Set color to blue for entry points
      fiducialPoint->GetAnnotationPointDisplayNode()->SetColor(0,0,1);

      // Add fiducial
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
      // Set color to blue for entry points
      fiducialPoint->GetAnnotationPointDisplayNode()->SetColor(0,1,0);

      // Add fiducial
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

  // Clear widget
  d->TrajectoryTableWidget->clearContents();
  d->TrajectoryTableWidget->setRowCount(0);

  // TODO: Populate table with trajectory in new node
  // How to know which fiducials have been used to create ruler ?
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

  int selectedRow = d->TrajectoryTableWidget->currentRow();
  if (selectedRow < 0)
    {
    return;
    }

  // Remove ruler from scene
  qSlicerPathPlannerTrajectoryItem* itemToRemove =
    dynamic_cast<qSlicerPathPlannerTrajectoryItem*>(d->TrajectoryTableWidget->item(selectedRow,0));
  if (itemToRemove)
    {
    this->mrmlScene()->RemoveNode(itemToRemove->trajectoryNode());
    }

  // Remove from widget
  d->TrajectoryTableWidget->removeRow(selectedRow);
}

//-----------------------------------------------------------------------------
void qSlicerPathPlannerModuleWidget::
onUpdateButtonClicked()
{
  Q_D(qSlicerPathPlannerModuleWidget);

  if (!d->TrajectoryTableWidget)
    {
    return;
    }

  if (!d->TargetPointWidget |
      !d->EntryPointWidget)
    {
    return;
    }

  QTableWidget* targetTable = d->TargetPointWidget->getTableWidget();
  QTableWidget* entryTable = d->EntryPointWidget->getTableWidget();

  if (!targetTable |
      !entryTable)
    {
    return;
    }

  // Get selected rows
  int trajectoryRow = d->TrajectoryTableWidget->currentRow();
  int targetRow = targetTable->currentRow();
  int entryRow = entryTable->currentRow();

  if ((trajectoryRow < 0) |
      (targetRow < 0) |
      (entryRow < 0))
    {
    return;
    }

  // Get items
  qSlicerPathPlannerFiducialItem *targetItem =
    dynamic_cast<qSlicerPathPlannerFiducialItem*>(targetTable->item(targetRow,0));
  qSlicerPathPlannerFiducialItem *entryItem =
    dynamic_cast<qSlicerPathPlannerFiducialItem*>(entryTable->item(entryRow,0));
  qSlicerPathPlannerTrajectoryItem *trajectoryItem =
    dynamic_cast<qSlicerPathPlannerTrajectoryItem*>(d->TrajectoryTableWidget->item(trajectoryRow,0));

  if (!targetItem |
      !entryItem |
      !trajectoryItem)
    {
    return;
    }
  
  // Update trajectory
  if (trajectoryItem->entryPoint() != entryItem->getFiducialNode())
    {
    trajectoryItem->setEntryPoint(entryItem->getFiducialNode());
    }

  if (trajectoryItem->targetPoint() != targetItem->getFiducialNode())
    {
    trajectoryItem->setTargetPoint(targetItem->getFiducialNode());
    }
  
  d->UpdateButton->setEnabled(0);
}

//-----------------------------------------------------------------------------
void qSlicerPathPlannerModuleWidget::
onClearButtonClicked()
{
  Q_D(qSlicerPathPlannerModuleWidget);

  if (!d->selectedTrajectoryNode)
    {
    return;
    }

  // Clear hierarchy node and widget
  d->selectedTrajectoryNode->RemoveAllChildrenNodes();
  d->TrajectoryTableWidget->clearContents();
  d->TrajectoryTableWidget->setRowCount(0);
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
    qSlicerPathPlannerTrajectoryItem* currentItem = 
      dynamic_cast<qSlicerPathPlannerTrajectoryItem*>(d->TrajectoryTableWidget->item(i,0));
    if (currentItem)
      {
      if (currentItem->entryPoint() == entryPoint)
	{
	if (currentItem->targetPoint() == targetPoint)
	  {
	  // Trajectory alread exists
	  return;
	  }
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

  // Insert new row
  d->TrajectoryTableWidget->insertRow(rowCount);

  // Populate row
  qSlicerPathPlannerTrajectoryItem* newTrajectory = new qSlicerPathPlannerTrajectoryItem();
  d->TrajectoryTableWidget->setItem(rowCount, 0, newTrajectory);
  d->TrajectoryTableWidget->setItem(rowCount, 1, new QTableWidgetItem());
  d->TrajectoryTableWidget->setItem(rowCount, 2, new QTableWidgetItem());

  newTrajectory->setEntryPoint(entryPoint);
  newTrajectory->setTargetPoint(targetPoint);

  if (newTrajectory->trajectoryNode())
    {
    newTrajectory->trajectoryNode()->Initialize(this->mrmlScene());
    newTrajectory->updateItem();
    }

  // Set only name editable
  d->TrajectoryTableWidget->item(rowCount,1)->setFlags(d->TrajectoryTableWidget->item(rowCount,1)->flags() & ~Qt::ItemIsEditable);
  d->TrajectoryTableWidget->item(rowCount,2)->setFlags(d->TrajectoryTableWidget->item(rowCount,2)->flags() & ~Qt::ItemIsEditable);

  // Automatic scroll to last item added
  d->TrajectoryTableWidget->scrollToItem(d->TrajectoryTableWidget->item(rowCount,0));  
}

//-----------------------------------------------------------------------------
void qSlicerPathPlannerModuleWidget::
onTrajectoryCellClicked(int row, int column)
{
  Q_D(qSlicerPathPlannerModuleWidget);

  if (!d->TrajectoryTableWidget |
      !d->EntryPointWidget |
      !d->TargetPointWidget)
    {
    return;
    }

  // Get item selected
  qSlicerPathPlannerTrajectoryItem* selectedTrajectory
    = dynamic_cast<qSlicerPathPlannerTrajectoryItem*>(d->TrajectoryTableWidget->item(row,0));
  if (!selectedTrajectory)
    {
    return;
    }

  // Find target point
  vtkMRMLAnnotationFiducialNode* targetFiducial =
    selectedTrajectory->targetPoint();
  if (targetFiducial)
    {
    for (int i = 0; i < d->TargetPointWidget->getTableWidget()->rowCount(); i++)
      {
      qSlicerPathPlannerFiducialItem* currentFiducial =
	dynamic_cast<qSlicerPathPlannerFiducialItem*>(d->TargetPointWidget->getTableWidget()->item(i,0));
      if (currentFiducial)
	{
	if (currentFiducial->getFiducialNode() == targetFiducial)
	  {
	  // Target found. Select it.
	  d->TargetPointWidget->getTableWidget()->setCurrentCell(i,0);
	  break;
	  }
	}
      }
    }

  // Find entry point
  vtkMRMLAnnotationFiducialNode* entryFiducial =
    selectedTrajectory->entryPoint();
  if (entryFiducial)
    {
    for (int i = 0; i < d->EntryPointWidget->getTableWidget()->rowCount(); i++)
      {
      qSlicerPathPlannerFiducialItem* currentFiducial =
	dynamic_cast<qSlicerPathPlannerFiducialItem*>(d->EntryPointWidget->getTableWidget()->item(i,0));
      if (currentFiducial)
	{
	if (currentFiducial->getFiducialNode() == entryFiducial)
	  {
	  // Entry found. Select it.
	  d->EntryPointWidget->getTableWidget()->setCurrentCell(i,0);
	  break;
	  }
	}
      }
    }
}


//-----------------------------------------------------------------------------
void qSlicerPathPlannerModuleWidget::
onMRMLSceneChanged(vtkMRMLScene* newScene)
{
  Q_D(qSlicerPathPlannerModuleWidget);
  
  if (!d->TargetPointListNodeSelector |
      !d->EntryPointListNodeSelector |
      !d->TrajectoryListNodeSelector)
    {
    return;
    }

  // Get Annotation Logic
  qSlicerAbstractCoreModule* annotationModule =
    qSlicerCoreApplication::application()->moduleManager()->module("Annotations");
  vtkSlicerAnnotationModuleLogic* annotationLogic;
  if (annotationModule)
    {
    annotationLogic = 
      vtkSlicerAnnotationModuleLogic::SafeDownCast(annotationModule->logic());
    if (annotationLogic)
      {
      const char* topLevelID = annotationLogic->GetTopLevelHierarchyNodeID();
      
      // Create Target Hierarchy Node
      annotationLogic->AddHierarchy();
      vtkMRMLAnnotationHierarchyNode* targetHierarchy
	= annotationLogic->GetActiveHierarchyNode();
      if (targetHierarchy)
	{
	targetHierarchy->SetName("Target List");
	d->TargetPointListNodeSelector->setCurrentNode(targetHierarchy);
	}
      annotationLogic->SetActiveHierarchyNodeID(topLevelID);
      
      // Create Entry Hierarchy Node
      annotationLogic->AddHierarchy();      
      vtkMRMLAnnotationHierarchyNode* entryHierarchy
	= annotationLogic->GetActiveHierarchyNode();
      if (entryHierarchy)
	{
	entryHierarchy->SetName("Entry List");
	d->EntryPointListNodeSelector->setCurrentNode(entryHierarchy);
	}
      }
    }

  // Create new PathPlannerTrajectory Node
  vtkMRMLNode* newTrajectoryNode =
    d->TrajectoryListNodeSelector->addNode();
}


//-----------------------------------------------------------------------------
void qSlicerPathPlannerModuleWidget::
onTargetSelectionChanged()
{
  Q_D(qSlicerPathPlannerModuleWidget);

  if (!d->TargetPointWidget->getTableWidget() | 
      !d->TrajectoryTableWidget)
    {
    return;
    }

  // Check if same fiducial
  int targetRow = d->TargetPointWidget->getTableWidget()->currentRow();
  int trajectoryRow = d->TrajectoryTableWidget->currentRow();

  // Get nodes
  qSlicerPathPlannerFiducialItem* targetItem =
    dynamic_cast<qSlicerPathPlannerFiducialItem*>(d->TargetPointWidget->getTableWidget()->item(targetRow,0));
  qSlicerPathPlannerTrajectoryItem* trajectoryItem =
    dynamic_cast<qSlicerPathPlannerTrajectoryItem*>(d->TrajectoryTableWidget->item(trajectoryRow,0));

  if (targetItem && trajectoryItem)
    {
    if (targetItem->getFiducialNode() == trajectoryItem->targetPoint())
      {
      // Same. No update.
      d->UpdateButton->setEnabled(0);
      }
    else
      {
      // Different. Allow update.
      d->UpdateButton->setEnabled(1);
      }
    }
}

//-----------------------------------------------------------------------------
void qSlicerPathPlannerModuleWidget::
onEntrySelectionChanged()
{
  Q_D(qSlicerPathPlannerModuleWidget);

  if (!d->EntryPointWidget->getTableWidget() | 
      !d->TrajectoryTableWidget)
    {
    return;
    }

  // Check if same fiducial
  int entryRow = d->EntryPointWidget->getTableWidget()->currentRow();
  int trajectoryRow = d->TrajectoryTableWidget->currentRow();

  // Get nodes
  qSlicerPathPlannerFiducialItem* entryItem =
    dynamic_cast<qSlicerPathPlannerFiducialItem*>(d->EntryPointWidget->getTableWidget()->item(entryRow,0));
  qSlicerPathPlannerTrajectoryItem* trajectoryItem =
    dynamic_cast<qSlicerPathPlannerTrajectoryItem*>(d->TrajectoryTableWidget->item(trajectoryRow,0));

  if (entryItem && trajectoryItem)
    {
    if (entryItem->getFiducialNode() == trajectoryItem->entryPoint())
      {
      // Same. No update.
      d->UpdateButton->setEnabled(0);
      }
    else
      {
      // Different. Allow update.
      d->UpdateButton->setEnabled(1);
      }
    }
}

//-----------------------------------------------------------------------------
void qSlicerPathPlannerModuleWidget::
onTrajectoryCellChanged(int row, int column)
{
  Q_D(qSlicerPathPlannerModuleWidget);

  if (!d->TrajectoryTableWidget)
    {
    return;
    }

  if (column != 0)
    {
    // Only update if name changed
    return;
    }

  // Get item
  qSlicerPathPlannerTrajectoryItem* currentItem =
    dynamic_cast<qSlicerPathPlannerTrajectoryItem*>(d->TrajectoryTableWidget->item(row,0));
  if (!currentItem)
    {
    return;
    }

  // Get ruler
  vtkMRMLAnnotationRulerNode* currentRuler =
    currentItem->trajectoryNode();
  if (!currentRuler)
    {
    return;
    }

  // Set new name
  currentRuler->SetName(currentItem->text().toStdString().c_str());
}
