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

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <Dataflow/Network/ModuleDescription.h>
#include <Core/Utils/Exception.h>

using namespace SCIRun::Dataflow::Networks;

ModuleDescription::ModuleDescription()
{
}

ModuleDescription::~ModuleDescription()
{
}

ModuleLookupInfo::ModuleLookupInfo() {}

ModuleLookupInfo::ModuleLookupInfo(const std::string& mod, const std::string& cat, const std::string& pack)
  : package_name_(pack), category_name_(cat), module_name_(mod) 
{}

ModuleId::ModuleId() : name_("<Unknown>"), id_("<Invalid>"), idNumber_(-1) {}

ModuleId::ModuleId(const std::string& name, int idNumber)
  : name_(name), idNumber_(idNumber)
{
  id_ = name + ':' + boost::lexical_cast<std::string>(idNumber);
}

ModuleId::ModuleId(const std::string& nameIdStr)
  : id_(nameIdStr)
{
  static boost::regex r("(.+?):?(\\d+)");
  boost::smatch what;
  if (!regex_match(id_, what, r))
    THROW_INVALID_ARGUMENT("Invalid Module Id");
  name_ = std::string(what[1]);
  idNumber_ = boost::lexical_cast<int>((std::string)what[2]);
}

bool SCIRun::Dataflow::Networks::operator==(const ModuleId& lhs, const ModuleId& rhs)
{
  return lhs.id_ == rhs.id_;
}

bool SCIRun::Dataflow::Networks::operator!=(const ModuleId& lhs, const ModuleId& rhs)
{
  return !(lhs == rhs);
}

bool SCIRun::Dataflow::Networks::operator<(const ModuleId& lhs, const ModuleId& rhs)
{
  return lhs.id_ < rhs.id_;
}

std::ostream& SCIRun::Dataflow::Networks::operator<<(std::ostream& o, const ModuleId& id)
{
  return o << id.id_;
}