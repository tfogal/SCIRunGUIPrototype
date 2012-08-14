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


#ifndef CORE_SERIALIZATION_NETWORK_MODULE_DESCRIPTION_SERIALIZATION_H
#define CORE_SERIALIZATION_NETWORK_MODULE_DESCRIPTION_SERIALIZATION_H 

#include <Dataflow/Network/ModuleDescription.h>
#include <Dataflow/Network/ConnectionId.h>
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>
#include <Dataflow/Serialization/Network/Share.h>

namespace SCIRun {
namespace Domain {
namespace Networks {

  class SCISHARE ModuleLookupInfoXML : public ModuleLookupInfo
  {
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      ar & BOOST_SERIALIZATION_NVP(package_name_);
      ar & BOOST_SERIALIZATION_NVP(category_name_);
      ar & BOOST_SERIALIZATION_NVP(module_name_);
    }

  public:
    ModuleLookupInfoXML();
    ModuleLookupInfoXML(const ModuleLookupInfoXML& rhs);
    ModuleLookupInfoXML(const ModuleLookupInfo& rhs);
  };

  class SCISHARE ConnectionDescriptionXML : public ConnectionDescription
  {
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      ar & BOOST_SERIALIZATION_NVP(moduleId1_);
      ar & BOOST_SERIALIZATION_NVP(port1_);
      ar & BOOST_SERIALIZATION_NVP(moduleId2_);
      ar & BOOST_SERIALIZATION_NVP(port2_);
    }
  public:
    ConnectionDescriptionXML();
    ConnectionDescriptionXML(const ConnectionDescriptionXML& rhs);
    ConnectionDescriptionXML(const ConnectionDescription& rhs);
  };

}}}

#endif