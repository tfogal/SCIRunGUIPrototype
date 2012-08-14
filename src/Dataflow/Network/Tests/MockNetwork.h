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

#ifndef MOCK_NETWORK_H
#define MOCK_NETWORK_H

#include <Dataflow/Network/NetworkInterface.h>
#include <Dataflow/Network/ConnectionId.h>
#include <Dataflow/Network/ModuleDescription.h>
#include <gmock/gmock.h>

namespace SCIRun {
  namespace Domain {
    namespace Networks {
      namespace Mocks
      {
        class MockNetwork : public NetworkInterface
        {
        public:
          MOCK_METHOD1(add_module, ModuleHandle(const ModuleLookupInfo&));
          MOCK_METHOD1(remove_module, bool(const std::string&));
          MOCK_CONST_METHOD0(nmodules, size_t());
          MOCK_CONST_METHOD1(module, ModuleHandle(size_t));
          MOCK_CONST_METHOD1(lookupModule, ModuleHandle(const std::string&));
          MOCK_CONST_METHOD1(lookupExecutable, ExecutableObject*(const std::string&));
          MOCK_METHOD4(connect, ConnectionId(ModuleHandle, size_t, ModuleHandle, size_t));
          MOCK_METHOD1(disconnect, bool(const ConnectionId&));
          MOCK_CONST_METHOD0(nconnections, size_t());
          MOCK_METHOD1(disable_connection, void(const ConnectionId&));
          MOCK_CONST_METHOD0(toString, std::string());
          MOCK_CONST_METHOD0(connections, ConnectionDescriptionList());
        };

        typedef boost::shared_ptr<MockNetwork> MockNetworkPtr;
      }
    }
  }
}

#endif