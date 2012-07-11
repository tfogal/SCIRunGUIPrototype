/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2012 Scientific Computing and Imaging Institute,
   University of Utah.

   License for the specific language governing rights and limitations under
   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/

#include <Interface/Modules/ReceiveScalarDialog.h>
#include <Interface/Application/Logger.h>

using namespace SCIRun::Gui;
using namespace SCIRun::Domain::Networks;

ReceiveScalarDialog::ReceiveScalarDialog(const std::string& name, ModuleStateHandle state, 
  QWidget* parent /* = 0 */)
  : ModuleDialogGeneric(parent),
  state_(state)
{
  setupUi(this);
  setWindowTitle(to_QString(name) + " --Receive");
  executionTimeHorizontalSlider_->setValue(moduleExecutionTime());
  //connect(executionTimeHorizontalSlider_, SIGNAL(valueChanged(int)), this, SIGNAL(executionTimeChanged(int)));
  connect(scalarValueToSend_, SIGNAL(textChanged(const QString&)), this, SIGNAL(scalarValue(const QString&)));
}

int ReceiveScalarDialog::moduleExecutionTime()
{
  return 2000;
}

void ReceiveScalarDialog::pullScalarValueFromState(const QString& str) 
{
  double value = str.toDouble();
  (*state_)["ValueToSend"] = value;
}