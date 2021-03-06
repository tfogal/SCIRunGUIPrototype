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

#include <QtGui>
#include <Interface/PythonTestGui/Link.h>
#include <Interface/PythonTestGui/Node.h>

Link::Link(Node* fromNode, Node* toNode)
{
  myFromNode_ = fromNode;
  myToNode_ = toNode;

  myFromNode_->addLink(this);
  myToNode_->addLink(this);

  setFlags(QGraphicsItem::ItemIsSelectable);
  setZValue(-1);

  setColor(Qt::darkRed);
  trackNodes();
}

Link::~Link()
{
  myFromNode_->removeLink(this);
  myToNode_->removeLink(this);
}

void Link::setColor(const QColor& color)
{
  setPen(QPen(color, 1.0));
}

QColor Link::color() const
{
  return pen().color();
}

void Link::trackNodes()
{
  setLine(QLineF(myFromNode_->pos(), myToNode_->pos()));
}