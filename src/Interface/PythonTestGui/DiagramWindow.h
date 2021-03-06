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

#ifndef DIAGRAMWINDOW_H
#define DIAGRAMWINDOW_H

#include <QMainWindow>

class Node;
class Link;
class QMenu;
class QToolBar;
class QAction;
class QGraphicsScene;
class QGraphicsView;

class DiagramWindow : public QMainWindow
{
	Q_OBJECT
	
public:
  DiagramWindow();

public Q_SLOTS:
  void printNodeCount();
  void printEdgeCount();
  void addNode();
  std::string removeNode(const std::string& name);
  void addLink();
  std::vector<std::string> listNodeNames() const;
public:
  int numNodes() const;
  int numEdges() const;

private Q_SLOTS:
  void del();
  void cut();
  void copy();
  void paste();
  void bringToFront();
  void sendToBack();
  void properties();
  void updateActions();
  void launchPython();

private:
  typedef QPair<Node*, Node*> NodePair;
  void createActions();
  void createMenus();
  void createToolBars();
  void setZValue(int z);
  void setupNode(Node* node);
  Node* selectedNode() const;
  Link* selectedLink() const;
  NodePair selectedNodePair() const;

  QMenu* fileMenu_;
  QMenu* editMenu_;
  QToolBar* editToolBar_;
  QAction* exitAction_;
  QAction* cutAction_;
  QAction* copyAction_;
  QAction* pasteAction_;
  QAction* deleteAction_;
  QAction* addLinkAction_;
  QAction* addNodeAction_;
  QAction* bringToFrontAction_;
  QAction* sendToBackAction_;
  QAction* propertiesAction_;
  QAction* pythonConsoleAction_;
  QAction* countNodesAction_;
  QAction* countEdgesAction_;

  QGraphicsScene* scene_;
  QGraphicsView* view_;
  class PythonConsoleWidget* pythonConsole_;
  
  int minZ_;
  int maxZ_;
  int seqNumber_;
};

#endif