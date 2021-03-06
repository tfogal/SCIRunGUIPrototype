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
#include <Dataflow/Network/Network.h>
#include <Dataflow/Network/ModuleInterface.h>
#include <Dataflow/Network/ModuleStateInterface.h>
#include <Dataflow/Network/ConnectionId.h>
#include <Core/Datatypes/DenseMatrix.h>
#include <Modules/Basic/SendTestMatrix.h>
#include <Modules/Basic/ReceiveTestMatrix.h>
#include <Modules/Math/EvaluateLinearAlgebraUnary.h>
#include <Modules/Factory/HardCodedModuleFactory.h>
#include <Core/Algorithms/Math/EvaluateLinearAlgebraUnary.h>
#include <Core/Algorithms/Math/EvaluateLinearAlgebraBinary.h>
#include <Core/Algorithms/Math/ReportMatrixInfo.h>
#include <Dataflow/Network/Tests/MockModuleState.h>
#include <Dataflow/State/SimpleMapModuleState.h>
#include <Dataflow/Engine/Scheduler/BoostGraphSerialScheduler.h>
#include <Dataflow/Engine/Scheduler/LinearSerialNetworkExecutor.h>
#include <Dataflow/Engine/Scheduler/BoostGraphParallelScheduler.h>
#include <Dataflow/Engine/Scheduler/BasicMultithreadedNetworkExecutor.h>
#include <Dataflow/Engine/Scheduler/BasicParallelExecutionStrategy.h>

#include <boost/assign.hpp>
#include <boost/config.hpp> // put this first to suppress some VC++ warnings

#include <iostream>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <ctime>

#include <boost/utility.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>

using namespace SCIRun;
using namespace SCIRun::Modules::Basic;
using namespace SCIRun::Modules::Math;
using namespace SCIRun::Modules::Factory;
using namespace SCIRun::Core::Datatypes;
using namespace SCIRun::Dataflow::Networks;
using namespace SCIRun::Dataflow::Networks::Mocks;
using namespace SCIRun::Core::Algorithms::Math;
using namespace SCIRun::Dataflow::State;
using namespace SCIRun::Dataflow::Engine;

using ::testing::_;
using ::testing::NiceMock;
using ::testing::DefaultValue;
using ::testing::Return;

using namespace std;
using namespace boost;
using namespace boost::assign;

namespace
{
  const DenseMatrix Zero(DenseMatrix::Zero(3,3));
}


class SchedulingWithBoostGraph : public ::testing::Test
{
public:
  SchedulingWithBoostGraph() :
    mf(new HardCodedModuleFactory),
    sf(new SimpleMapModuleStateFactory),
    matrixMathNetwork(mf, sf)
  {
  }
protected:
  ModuleFactoryHandle mf;
  ModuleStateFactoryHandle sf;
  Network matrixMathNetwork;
  ModuleHandle receive, report;
  DenseMatrix expected;

  virtual void SetUp()
  {
    
  }

  ModuleHandle addModuleToNetwork(Network& network, const std::string& moduleName)
  {
    ModuleLookupInfo info;
    info.module_name_ = moduleName;
    return network.add_module(info);
  }

  DenseMatrixHandle matrix1()
  {
    DenseMatrixHandle m(new DenseMatrix(3, 3));
    for (int i = 0; i < m->rows(); ++i)
      for (int j = 0; j < m->cols(); ++j)
        (*m)(i, j) = 3.0 * i + j;
    return m;
  }
  DenseMatrixHandle matrix2()
  {
    DenseMatrixHandle m(new DenseMatrix(3, 3));
    for (int i = 0; i < m->rows(); ++i)
      for (int j = 0; j < m->cols(); ++j)
        (*m)(i, j) = -2.0 * i + j;
    return m;
  }

  void setupBasicNetwork()
  {
    //Test network:
    /* 
    send m1(0)          send m2(0)
    |            |             |
    transpose(1) negate(1)    scalar mult *4(1)
    |            |             |
    |           multiply(2)
    |           |
          add(3)
          |      |
          report(4) receive(4)
    */

    expected = (-*matrix1()) * (4* *matrix2()) + matrix1()->transpose();
  
    ModuleHandle matrix1Send = addModuleToNetwork(matrixMathNetwork, "SendTestMatrix");
    ModuleHandle matrix2Send = addModuleToNetwork(matrixMathNetwork, "SendTestMatrix");
  
    ModuleHandle transpose = addModuleToNetwork(matrixMathNetwork, "EvaluateLinearAlgebraUnary");
    ModuleHandle negate = addModuleToNetwork(matrixMathNetwork, "EvaluateLinearAlgebraUnary");
    ModuleHandle scalar = addModuleToNetwork(matrixMathNetwork, "EvaluateLinearAlgebraUnary");

    ModuleHandle multiply = addModuleToNetwork(matrixMathNetwork, "EvaluateLinearAlgebraBinary");
    ModuleHandle add = addModuleToNetwork(matrixMathNetwork, "EvaluateLinearAlgebraBinary");

    report = addModuleToNetwork(matrixMathNetwork, "ReportMatrixInfo");
    receive = addModuleToNetwork(matrixMathNetwork, "ReceiveTestMatrix");
  
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
    matrixMathNetwork.connect(ConnectionOutputPort(matrix1Send, 0), ConnectionInputPort(transpose, 0));
    matrixMathNetwork.connect(ConnectionOutputPort(matrix1Send, 0), ConnectionInputPort(negate, 0));
    matrixMathNetwork.connect(ConnectionOutputPort(matrix2Send, 0), ConnectionInputPort(scalar, 0));
    matrixMathNetwork.connect(ConnectionOutputPort(negate, 0), ConnectionInputPort(multiply, 0));
    matrixMathNetwork.connect(ConnectionOutputPort(scalar, 0), ConnectionInputPort(multiply, 1));
    matrixMathNetwork.connect(ConnectionOutputPort(transpose, 0), ConnectionInputPort(add, 0));
    matrixMathNetwork.connect(ConnectionOutputPort(multiply, 0), ConnectionInputPort(add, 1));
    matrixMathNetwork.connect(ConnectionOutputPort(add, 0), ConnectionInputPort(report, 0));
    matrixMathNetwork.connect(ConnectionOutputPort(add, 0), ConnectionInputPort(receive, 0));
    EXPECT_EQ(9, matrixMathNetwork.nconnections());

    //Set module parameters.
    matrix1Send->get_state()->setTransientValue("MatrixToSend", matrix1());
    matrix2Send->get_state()->setTransientValue("MatrixToSend", matrix2());
    transpose->get_state()->setValue(EvaluateLinearAlgebraUnaryAlgorithm::OperatorName, EvaluateLinearAlgebraUnaryAlgorithm::TRANSPOSE);
    negate->get_state()->setValue(EvaluateLinearAlgebraUnaryAlgorithm::OperatorName, EvaluateLinearAlgebraUnaryAlgorithm::NEGATE);
    scalar->get_state()->setValue(EvaluateLinearAlgebraUnaryAlgorithm::OperatorName, EvaluateLinearAlgebraUnaryAlgorithm::SCALAR_MULTIPLY);
    scalar->get_state()->setValue(EvaluateLinearAlgebraUnaryAlgorithm::ScalarValue, 4.0);
    multiply->get_state()->setValue(EvaluateLinearAlgebraBinaryAlgorithm::OperatorName, EvaluateLinearAlgebraBinaryAlgorithm::MULTIPLY);
    add->get_state()->setValue(EvaluateLinearAlgebraBinaryAlgorithm::OperatorName, EvaluateLinearAlgebraBinaryAlgorithm::ADD);
  }
};

TEST_F(SchedulingWithBoostGraph, NetworkFromMatrixCalculator)
{
  setupBasicNetwork();

  BoostGraphSerialScheduler scheduler;
  ModuleExecutionOrder order = scheduler.schedule(matrixMathNetwork);
  LinearSerialNetworkExecutor executor;
  ExecutionBounds bounds;
  executor.executeAll(matrixMathNetwork, order, bounds);

  //TODO: let executor thread finish.  should be an event generated or something.
  boost::this_thread::sleep(boost::posix_time::milliseconds(100));

  //grab reporting module state
  ReportMatrixInfoAlgorithm::Outputs reportOutput = any_cast_or_default<ReportMatrixInfoAlgorithm::Outputs>(report->get_state()->getTransientValue("ReportedInfo"));
  DenseMatrixHandle receivedMatrix = any_cast_or_default<DenseMatrixHandle>(receive->get_state()->getTransientValue("ReceivedMatrix"));

  ASSERT_TRUE(receivedMatrix);
  //verify results
  EXPECT_EQ(expected, *receivedMatrix);
  EXPECT_EQ(3, reportOutput.get<1>());
  EXPECT_EQ(3, reportOutput.get<2>());
  EXPECT_EQ(9, reportOutput.get<3>());
  EXPECT_EQ(22, reportOutput.get<4>());
  EXPECT_EQ(186, reportOutput.get<5>());
}

TEST_F(SchedulingWithBoostGraph, CanDetectConnectionCycles)
{
  ModuleHandle negate = addModuleToNetwork(matrixMathNetwork, "EvaluateLinearAlgebraUnary");
  ModuleHandle scalar = addModuleToNetwork(matrixMathNetwork, "EvaluateLinearAlgebraUnary");

  EXPECT_EQ(2, matrixMathNetwork.nmodules());

  EXPECT_EQ(0, matrixMathNetwork.nconnections());
  matrixMathNetwork.connect(ConnectionOutputPort(negate, 0), ConnectionInputPort(scalar, 0));
  matrixMathNetwork.connect(ConnectionOutputPort(scalar, 0), ConnectionInputPort(negate, 0));
  EXPECT_EQ(2, matrixMathNetwork.nconnections());

  //Set module parameters.
  negate->get_state()->setValue(EvaluateLinearAlgebraUnaryAlgorithm::OperatorName,
    EvaluateLinearAlgebraUnaryAlgorithm::NEGATE);
  scalar->get_state()->setValue(EvaluateLinearAlgebraUnaryAlgorithm::OperatorName, EvaluateLinearAlgebraUnaryAlgorithm::SCALAR_MULTIPLY);
  scalar->get_state()->setValue(EvaluateLinearAlgebraUnaryAlgorithm::ScalarValue, 4.0);

  BoostGraphSerialScheduler scheduler;
  
  EXPECT_THROW(scheduler.schedule(matrixMathNetwork), NetworkHasCyclesException);
}




TEST_F(SchedulingWithBoostGraph, NetworkFromMatrixCalculatorMultiThreaded)
{
  setupBasicNetwork();

  //BoostGraphParallelScheduler scheduler;
  //auto order = scheduler.schedule(matrixMathNetwork);
  //BasicMultithreadedNetworkExecutor executor;
  //executor.executeAll(matrixMathNetwork, order, ExecutionBounds());
  BasicParallelExecutionStrategy strategy;
  strategy.executeAll(matrixMathNetwork, matrixMathNetwork);

  //TODO: let executor thread finish.  should be an event generated or something.
  boost::this_thread::sleep(boost::posix_time::milliseconds(100));

  //grab reporting module state
  ReportMatrixInfoAlgorithm::Outputs reportOutput = any_cast_or_default<ReportMatrixInfoAlgorithm::Outputs>(report->get_state()->getTransientValue("ReportedInfo"));
  DenseMatrixHandle receivedMatrix = any_cast_or_default<DenseMatrixHandle>(receive->get_state()->getTransientValue("ReceivedMatrix"));

  ASSERT_TRUE(receivedMatrix);
  //verify results
  EXPECT_EQ(expected, *receivedMatrix);
  EXPECT_EQ(3, reportOutput.get<1>());
  EXPECT_EQ(3, reportOutput.get<2>());
  EXPECT_EQ(9, reportOutput.get<3>());
  EXPECT_EQ(22, reportOutput.get<4>());
  EXPECT_EQ(186, reportOutput.get<5>());
}

TEST_F(SchedulingWithBoostGraph, SerialNetworkOrder)
{
  setupBasicNetwork();

  BoostGraphSerialScheduler scheduler;
  ModuleExecutionOrder order = scheduler.schedule(matrixMathNetwork);

  std::list<ModuleId> expected = list_of
    (ModuleId("SendTestMatrix:1"))
    (ModuleId("EvaluateLinearAlgebraUnary:4"))
    (ModuleId("SendTestMatrix:0"))
    (ModuleId("EvaluateLinearAlgebraUnary:3"))
    (ModuleId("EvaluateLinearAlgebraBinary:5"))
    (ModuleId("EvaluateLinearAlgebraUnary:2"))
    (ModuleId("EvaluateLinearAlgebraBinary:6"))
    (ModuleId("ReportMatrixInfo:7"))
    (ModuleId("ReceiveTestMatrix:8"));
  EXPECT_EQ(ModuleExecutionOrder(expected), order);
}

TEST_F(SchedulingWithBoostGraph, ParallelNetworkOrder)
{
  setupBasicNetwork();


  BoostGraphParallelScheduler scheduler;
  auto order = scheduler.schedule(matrixMathNetwork);
  auto range = std::make_pair(order.begin(), order.end());

  std::ostringstream ostr;
  BOOST_FOREACH(const ParallelModuleExecutionOrder::ModulesByGroup::value_type& v, range)
  {
    ostr << v.first << " " << v.second << std::endl;
  }

  std::cout << ostr.str() << std::endl;

  std::string expected = 
    "0 SendTestMatrix:1\n"
    "0 SendTestMatrix:0\n"
    "1 EvaluateLinearAlgebraUnary:3\n"
    "1 EvaluateLinearAlgebraUnary:4\n"
    "1 EvaluateLinearAlgebraUnary:2\n"
    "2 EvaluateLinearAlgebraBinary:5\n"
    "3 EvaluateLinearAlgebraBinary:6\n"
    "4 ReportMatrixInfo:7\n"
    "4 ReceiveTestMatrix:8\n";

  EXPECT_EQ(expected, ostr.str());
}

namespace ThreadingPrototype
{
  struct Unit
  {
    Unit(const std::string& s) : id(s),
      priority(rand() % 4),
      ready(0 == priority), done(false),
      runtime(rand() % 1000 + 1)
    {
    }
    std::string id;
    int priority;
    bool ready;
    bool done;
    int runtime;
  };

  typedef boost::shared_ptr<Unit> UnitPtr;

  bool operator<(const Unit& lhs, const Unit& rhs)
  {
    return lhs.priority < rhs.priority;
  }

  bool operator<(const UnitPtr& lhs, const UnitPtr& rhs)
  {
    return lhs->priority < rhs->priority;
  }

  std::ostream& operator<<(std::ostream& o, const Unit& u)
  {
    return o << u.id << " : "
      << u.priority << ":"
      << u.ready << ":" << u.done << ":"
      << u.runtime;
  }

  typedef std::queue<UnitPtr> WorkQueue;  //TODO: will need to be thread-safe
  typedef std::list<UnitPtr> WaitingList;
  typedef std::list<UnitPtr> DoneList;

  UnitPtr makeUnit()
  {
    return UnitPtr(new Unit(boost::lexical_cast<std::string>(rand())));
  }

  std::ostream& operator<<(std::ostream& o, const UnitPtr& u)
  {
    if (u)
      o << *u;
    return o;
  }

  TEST(MultiExecutorPrototypeTest, GenerateListOfUnits)
  {
    WaitingList list;
    std::generate_n(std::back_inserter(list), 10, makeUnit);
    std::copy(list.begin(), list.end(), std::ostream_iterator<UnitPtr>(std::cout, "\n"));
  }

  typedef boost::mutex Mutex;

  class WorkUnitProducer
  {
  public:
    WorkUnitProducer(WorkQueue& workQueue, WaitingList& list, Mutex& mutex) : work_(workQueue), waiting_(list),
      /*donePushing_(false),*/ currentPriority_(0), mutex_(mutex)
    {
      waiting_.sort();
      //std::cout << "Sorted work list:" << std::endl;
      //std::copy(list.begin(), list.end(), std::ostream_iterator<UnitPtr>(std::cout, "\n"));
    }
    void run()
    {
      //mutex_.lock();
      //std::cout << "Producer started." << std::endl;
      //mutex_.unlock();
      while (!waiting_.empty())
      {
        for (auto i = waiting_.begin(); i != waiting_.end(); )
        {
          if ((*i)->ready)
          {
            mutex_.lock();
            //std::cout << "\tProducer: Transferring ready unit " << (*i)->id << std::endl;
            work_.push(*i);
            //std::cout << "\tProducer: Done transferring ready unit " << (*i)->id << std::endl;
            mutex_.unlock();
            i = waiting_.erase(i);
          }
          else
            ++i;
        }
        if (workDone() && !waiting_.empty() && (*waiting_.begin())->priority > currentPriority_)
        {
          currentPriority_ = (*waiting_.begin())->priority;
          //mutex_.lock();
          //std::cout << "\tProducer: Setting as ready units with priority = " << currentPriority_ << std::endl;
          //mutex_.unlock();
          for (auto i = waiting_.begin(); i != waiting_.end(); ++i)
          {
            if ((*i)->priority == currentPriority_)
              (*i)->ready = true;
          }
        }
        //mutex_.lock();
        //std::cout << "\tProducer: Waiting list size = " << waiting_.size() << std::endl;
        //std::cout << "\tProducer: work queue size = " << work_.size() << std::endl;
        //mutex_.unlock();
      }
      //mutex_.lock();
      //std::cout << "Producer done." << std::endl;
      //mutex_.unlock();
    }
    bool isDone() const
    {
      boost::lock_guard<Mutex> lock(mutex_);
      return waiting_.empty();// && donePushing_;
    }
    bool workDone() const
    {
      boost::lock_guard<Mutex> lock(mutex_);
      return work_.empty();
    }
  private:
    WorkQueue& work_;
    WaitingList& waiting_;
    //bool donePushing_;
    int currentPriority_;
    Mutex& mutex_;
  };

  class WorkUnitConsumer
  {
  public:
    explicit WorkUnitConsumer(WorkQueue& workQueue, const WorkUnitProducer& producer, DoneList& done, Mutex& mutex) :
    work_(workQueue), producer_(producer), done_(done), mutex_(mutex)
    {
    }
    void run()
    {
      //mutex_.lock();
      //std::cout << "Consumer started." << std::endl;
      //mutex_.unlock();
      while (!producer_.isDone())
      {
        //mutex_.lock();
        //std::cout << "\tConsumer thinks producer is not done." << std::endl;
        //mutex_.unlock();
        while (moreWork())
        {
          //mutex_.lock();
          //std::cout << "\tConsumer thinks work queue is not empty." << std::endl;
          //mutex_.unlock();

          mutex_.lock();
          //std::cout << "\tConsumer accessing front of work queue." << std::endl;
          UnitPtr unit = work_.front();
          //std::cout << "\tConsumer popping front of work queue." << std::endl;
          work_.pop();
          mutex_.unlock();

          //mutex_.lock();
          //std::cout << "~~~Processing " << unit->id << ": sleeping for " <<
          //unit->runtime << " ms" << std::endl;
          //mutex_.unlock();

          boost::this_thread::sleep(boost::posix_time::milliseconds(unit->runtime));
          unit->done = true;

          done_.push_back(unit);

          //mutex_.lock();
          //std::cout << "\tConsumer: adding done unit, done size = " << done_.size() << std::endl;
          //mutex_.unlock();
        }
      }
      //mutex_.lock();
      //std::cout << "Consumer done." << std::endl;
      //mutex_.unlock();
    }

    bool moreWork()
    {
      boost::lock_guard<Mutex> lock(mutex_);
      return !work_.empty();
    }


  private:
    WorkQueue& work_;
    const WorkUnitProducer& producer_;
    DoneList& done_;
    Mutex& mutex_;
  };


  TEST(MultiExecutorPrototypeTest, DISABLED_Run)
  {
    const int size = 100;
    WaitingList list;
    std::generate_n(std::back_inserter(list), size, makeUnit);

    int totalSleepTime = std::accumulate(list.begin(), list.end(), 0, [](int total, UnitPtr u){ return total + u->runtime; });
    std::cout << size << " work units, will sleep for total of " << totalSleepTime / 1000 << " seconds" << std::endl;

    Mutex mutex;
    WorkQueue workQ;
    WorkUnitProducer producer(workQ, list, mutex);

    DoneList done;
    WorkUnitConsumer consumer(workQ, producer, done, mutex);

    boost::thread tR = boost::thread(boost::bind(&WorkUnitProducer::run, producer));
    boost::thread tC = boost::thread(boost::bind(&WorkUnitConsumer::run, consumer));

    tR.join();
    tC.join();

    EXPECT_EQ(size, done.size());
    std::cout << "DONE. Finished list first 10:" << std::endl;
    auto end = done.begin();
    std::advance(end, 10);
    std::copy(done.begin(), end, std::ostream_iterator<UnitPtr>(std::cout, "\n"));
    BOOST_FOREACH(const UnitPtr& u, done)
    {
      EXPECT_TRUE(u->done);
    }
  }
}