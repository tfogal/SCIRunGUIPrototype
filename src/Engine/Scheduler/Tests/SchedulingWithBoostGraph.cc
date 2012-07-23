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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Core/Dataflow/Network/Network.h>
#include <Core/Dataflow/Network/ModuleInterface.h>
#include <Core/Dataflow/Network/ModuleStateInterface.h>
#include <Core/Dataflow/Network/ConnectionId.h>
#include <Core/Dataflow/Network/Tests/MockNetwork.h>
#include <Core/Datatypes/DenseMatrix.h>
#include <Core/Datatypes/MatrixComparison.h>
#include <Core/Datatypes/MatrixIO.h>
#include <Modules/Basic/SendTestMatrix.h>
#include <Modules/Basic/ReceiveTestMatrix.h>
#include <Modules/Math/EvaluateLinearAlgebraUnary.h>
#include <Modules/Factory/HardCodedModuleFactory.h>
#include <Algorithms/Math/EvaluateLinearAlgebraUnary.h>
#include <Algorithms/Math/EvaluateLinearAlgebraBinary.h>
#include <Algorithms/Math/ReportMatrixInfo.h>
#include <Core/Dataflow/Network/Tests/MockModuleState.h>
#include <Engine/State/SimpleMapModuleState.h>
#include <Engine/Scheduler/BoostGraphSerialScheduler.h>
#include <Engine/Scheduler/LinearSerialNetworkExecutor.h>
#include <boost/config.hpp> // put this first to suppress some VC++ warnings

#include <iostream>
#include <iterator>
#include <algorithm>
#include <time.h>

#include <boost/utility.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/foreach.hpp>

using namespace SCIRun;
using namespace SCIRun::Modules::Basic;
using namespace SCIRun::Modules::Math;
using namespace SCIRun::Modules::Factory;
using namespace SCIRun::Domain::Datatypes;
using namespace SCIRun::Domain::Networks;
using namespace SCIRun::Domain::Networks::Mocks;
using namespace SCIRun::Algorithms::Math;
using namespace SCIRun::Domain::State;
using namespace SCIRun::Engine;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::DefaultValue;
using ::testing::Return;

using namespace std;
using namespace boost;

#if 0

struct cycle_detector : public dfs_visitor<>
{
  cycle_detector(bool& has_cycle) 
    : m_has_cycle(has_cycle) { }

  template <class Edge, class Graph>
  void back_edge(Edge, Graph&) { m_has_cycle = true; }
protected:
  bool& m_has_cycle;
};
#endif

namespace
{
  DenseMatrixHandle matrix1()
  {
    DenseMatrixHandle m(new DenseMatrix(3, 3));
    for (size_t i = 0; i < m->nrows(); ++i)
      for (size_t j = 0; j < m->ncols(); ++j)
        (*m)(i, j) = 3.0 * i + j;
    return m;
  }
  DenseMatrixHandle matrix2()
  {
    DenseMatrixHandle m(new DenseMatrix(3, 3));
    for (size_t i = 0; i < m->nrows(); ++i)
      for (size_t j = 0; j < m->ncols(); ++j)
        (*m)(i, j) = -2.0 * i + j;
    return m;
  }
  const DenseMatrix Zero(DenseMatrix::zero_matrix(3,3));

  ModuleHandle addModuleToNetwork(Network& network, const std::string& moduleName)
  {
    ModuleLookupInfo info;
    info.module_name_ = moduleName;
    return network.add_module(info);
  }
}

TEST(SchedulingWithBoostGraph, NetworkFromMatrixCalculator)
{
  DenseMatrix expected = (-*matrix1()) * (4* *matrix2()) + transpose(*matrix1());

  //Test network:
  /* 
  send m1             send m2
  |         |         |
  transpose negate    scalar mult *4
  |         |         |
  |           multiply
  |           |
        add
        |      |
        report receive
  */

  ModuleFactoryHandle mf(new HardCodedModuleFactory);
  ModuleStateFactoryHandle sf(new SimpleMapModuleStateFactory);
  Network matrixMathNetwork(mf, sf);
  ModuleHandle matrix1Send = addModuleToNetwork(matrixMathNetwork, "SendTestMatrix");
  ModuleHandle matrix2Send = addModuleToNetwork(matrixMathNetwork, "SendTestMatrix");
  
  ModuleHandle transpose = addModuleToNetwork(matrixMathNetwork, "EvaluateLinearAlgebraUnary");
  ModuleHandle negate = addModuleToNetwork(matrixMathNetwork, "EvaluateLinearAlgebraUnary");
  ModuleHandle scalar = addModuleToNetwork(matrixMathNetwork, "EvaluateLinearAlgebraUnary");

  ModuleHandle multiply = addModuleToNetwork(matrixMathNetwork, "EvaluateLinearAlgebraBinary");
  ModuleHandle add = addModuleToNetwork(matrixMathNetwork, "EvaluateLinearAlgebraBinary");

  ModuleHandle report = addModuleToNetwork(matrixMathNetwork, "ReportMatrixInfo");
  ModuleHandle receive = addModuleToNetwork(matrixMathNetwork, "ReceiveTestMatrix");
  
  EXPECT_EQ(9, matrixMathNetwork.nmodules());

  //TODO: turn this into a convenience network printing function
  //for (size_t i = 0; i < matrixMathNetwork.nmodules(); ++i)
  //{
  //  ModuleHandle m = matrixMathNetwork.module(i);
  //  std::cout << m->get_module_name() << std::endl;
  //  std::cout << "inputs: " << m->num_input_ports() << std::endl;
  //  std::cout << "outputs: " << m->num_output_ports() << std::endl;
  //  std::cout << "has ui: " << m->has_ui() << std::endl;
  //}

  EXPECT_EQ(0, matrixMathNetwork.nconnections());
  matrixMathNetwork.connect(matrix1Send, 0, transpose, 0);
  matrixMathNetwork.connect(matrix1Send, 0, negate, 0);
  matrixMathNetwork.connect(matrix2Send, 0, scalar, 0);
  matrixMathNetwork.connect(negate, 0, multiply, 0);
  matrixMathNetwork.connect(scalar, 0, multiply, 1);
  matrixMathNetwork.connect(transpose, 0, add, 0);
  matrixMathNetwork.connect(multiply, 0, add, 1);
  matrixMathNetwork.connect(add, 0, report, 0);
  matrixMathNetwork.connect(add, 0, receive, 0);
  EXPECT_EQ(9, matrixMathNetwork.nconnections());

  //Set module parameters.
  matrix1Send->get_state()->setValue("MatrixToSend", matrix1());
  matrix2Send->get_state()->setValue("MatrixToSend", matrix2());
  transpose->get_state()->setValue("Operation", EvaluateLinearAlgebraUnaryAlgorithm::Parameters(EvaluateLinearAlgebraUnaryAlgorithm::TRANSPOSE));
  negate->get_state()->setValue("Operation", EvaluateLinearAlgebraUnaryAlgorithm::Parameters(EvaluateLinearAlgebraUnaryAlgorithm::NEGATE));
  scalar->get_state()->setValue("Operation", EvaluateLinearAlgebraUnaryAlgorithm::Parameters(EvaluateLinearAlgebraUnaryAlgorithm::SCALAR_MULTIPLY, 4.0));
  multiply->get_state()->setValue("Operation", EvaluateLinearAlgebraBinaryAlgorithm::Parameters(EvaluateLinearAlgebraBinaryAlgorithm::MULTIPLY));
  add->get_state()->setValue("Operation", EvaluateLinearAlgebraBinaryAlgorithm::Parameters(EvaluateLinearAlgebraBinaryAlgorithm::ADD));

  BoostGraphSerialScheduler scheduler;
  ModuleExecutionOrder order = scheduler.schedule(matrixMathNetwork);
  LinearSerialNetworkExecutor executor;
  executor.executeAll(matrixMathNetwork, order);

  //grab reporting module state
  ReportMatrixInfoAlgorithm::Outputs reportOutput = boost::any_cast<ReportMatrixInfoAlgorithm::Outputs>(report->get_state()->getValue("ReportedInfo"));
  DenseMatrixHandle receivedMatrix = boost::any_cast<DenseMatrixHandle>(receive->get_state()->getValue("ReceivedMatrix"));

  ASSERT_TRUE(receivedMatrix);
  //verify results
  EXPECT_EQ(expected, *receivedMatrix);
  EXPECT_EQ(3, reportOutput.get<1>());
  EXPECT_EQ(3, reportOutput.get<2>());
  EXPECT_EQ(9, reportOutput.get<3>());
  EXPECT_EQ(22, reportOutput.get<4>());
  EXPECT_EQ(186, reportOutput.get<5>());
}

TEST(SchedulingWithBoostGraph, CanDetectConnectionCycles)
{
  ModuleFactoryHandle mf(new HardCodedModuleFactory);
  ModuleStateFactoryHandle sf(new SimpleMapModuleStateFactory);
  Network matrixMathNetwork(mf, sf);
  
  ModuleHandle negate = addModuleToNetwork(matrixMathNetwork, "EvaluateLinearAlgebraUnary");
  ModuleHandle scalar = addModuleToNetwork(matrixMathNetwork, "EvaluateLinearAlgebraUnary");

  EXPECT_EQ(2, matrixMathNetwork.nmodules());

  EXPECT_EQ(0, matrixMathNetwork.nconnections());
  matrixMathNetwork.connect(negate, 0, scalar, 0);
  matrixMathNetwork.connect(scalar, 0, negate, 0);
  EXPECT_EQ(2, matrixMathNetwork.nconnections());

  //Set module parameters.
  negate->get_state()->setValue("Operation", EvaluateLinearAlgebraUnaryAlgorithm::Parameters(EvaluateLinearAlgebraUnaryAlgorithm::NEGATE));
  scalar->get_state()->setValue("Operation", EvaluateLinearAlgebraUnaryAlgorithm::Parameters(EvaluateLinearAlgebraUnaryAlgorithm::SCALAR_MULTIPLY, 4.0));

  BoostGraphSerialScheduler scheduler;
  
  EXPECT_THROW(scheduler.schedule(matrixMathNetwork), NetworkHasCyclesException);
}