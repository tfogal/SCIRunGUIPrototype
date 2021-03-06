/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2012 Scientific Computing and Imaging Institute,
   University of Utah.

   
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

#include <iostream>
#include <Interface/PythonTestGui/API/DiagramView.h>

boost::shared_ptr<DiagramViewInterface> DiagramView::impl_;

int DiagramView::numEdges()
{
  if (impl_)
    return impl_->numEdges();
  else
  {
    std::cout << "Null implementation: DiagramViewImpl" << std::endl;
    return -1;
  }
}

int DiagramView::numNodes()
{
  if (impl_)
    return impl_->numNodes();
  else
  {
    std::cout << "Null implementation: DiagramViewImpl" << std::endl;
    return -1;
  }
}

void DiagramView::addNode()
{
  if (impl_)
    impl_->addNode();
  else
  {
    std::cout << "Null implementation: DiagramViewImpl" << std::endl;
  }
}

std::string DiagramView::removeNode(const std::string& name)
{
  if (impl_)
    return impl_->removeNode(name);
  else
  {
    std::cout << "Null implementation: DiagramViewImpl" << std::endl;
    return "";
  }
}

std::string DiagramView::firstNodeName()
{
  if (impl_)
    return impl_->firstNodeName();
  else
  {
    std::cout << "Null implementation: DiagramViewImpl" << std::endl;
    return std::string();
  }
}

std::vector<std::string> DiagramView::listNodeNames()
{
  if (impl_)
    return impl_->listNodeNames();
  else
  {
    std::cout << "Null implementation: DiagramViewImpl" << std::endl;
    return std::vector<std::string>();
  }
}