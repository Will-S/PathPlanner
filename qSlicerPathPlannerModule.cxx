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
#include <QtPlugin>

// PathPlanner Logic includes
#include <vtkSlicerPathPlannerLogic.h>

// PathPlanner includes
#include "qSlicerPathPlannerModule.h"
#include "qSlicerPathPlannerModuleWidget.h"

//-----------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(qSlicerPathPlannerModule, qSlicerPathPlannerModule);

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerPathPlannerModulePrivate
{
public:
  qSlicerPathPlannerModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerPathPlannerModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerPathPlannerModulePrivate
::qSlicerPathPlannerModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerPathPlannerModule methods

//-----------------------------------------------------------------------------
qSlicerPathPlannerModule
::qSlicerPathPlannerModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerPathPlannerModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerPathPlannerModule::~qSlicerPathPlannerModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerPathPlannerModule::helpText()const
{
  return "This is a loadable module bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicerPathPlannerModule::acknowledgementText()const
{
  return "This work was partially funded by NCIGT";
}

//-----------------------------------------------------------------------------
QStringList qSlicerPathPlannerModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Laurent Chauvin (SNR), Atsushi Yamada PhD (SNR), Junichi Tokuda (SNR)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerPathPlannerModule::icon()const
{
  return QIcon(":/Icons/PathPlanner.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerPathPlannerModule::categories() const
{
  return QStringList() << "IGT";
}

//-----------------------------------------------------------------------------
QStringList qSlicerPathPlannerModule::dependencies() const
{
  return QStringList() << "Annotations";
}

//-----------------------------------------------------------------------------
void qSlicerPathPlannerModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation * qSlicerPathPlannerModule
::createWidgetRepresentation()
{
  return new qSlicerPathPlannerModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerPathPlannerModule::createLogic()
{
  return vtkSlicerPathPlannerLogic::New();
}
