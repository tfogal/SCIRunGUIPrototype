/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2009 Scientific Computing and Imaging Institute,
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


/*
 *
 *  Written by:
 *   Steven G. Parker
 *   Department of Computer Science
 *   University of Utah
 *   July 1994
 *
 */

#ifndef MODULES_DATAIO_GENERIC_READER_H
#define MODULES_DATAIO_GENERIC_READER_H

#include <boost/filesystem.hpp>
#include <Core/Datatypes/String.h>
#include <Dataflow/Network/Module.h>

namespace SCIRun {
  namespace Modules {
    namespace DataIO {

template <class HType, class PortTag> 
class GenericReader : public SCIRun::Dataflow::Networks::Module,
  public Has1InputPort<StringPortTag>,
  public Has2OutputPorts<PortTag, StringPortTag>
{
public:
  GenericReader(const std::string &name, const std::string &category, const std::string &package, const std::string& stateFilename);

  virtual void execute();
  INPUT_PORT(0, Filename, String);
  //OUTPUT_PORT(0, Object, PortType);
  OUTPUT_PORT(1, FileLoaded, String);

protected:
  std::string filename_;
  Core::Algorithms::AlgorithmParameterName stateFilename_;
  PortName<typename HType::element_type, 0> objectPortName_;
  //GuiFilename gui_filename_;
  //GuiString gui_from_env_;

  time_t old_filemodification_;

  bool importing_;

  virtual bool call_importer(const std::string &filename, HType & handle);
};


template <class HType, class PortTag> 
GenericReader<HType, PortTag>::GenericReader(const std::string &name,
				    const std::string &cat, const std::string &pack, const std::string& stateFilename)
  : SCIRun::Dataflow::Networks::Module(SCIRun::Dataflow::Networks::ModuleLookupInfo(name, cat, pack)),
    //gui_filename_(get_ctx()->subVar("filename"), ""),
    //gui_from_env_(get_ctx()->subVar("from-env"),""),
    stateFilename_(stateFilename),
    old_filemodification_(0),
    importing_(false)
{
}


template <class HType, class PortTag> 
bool
GenericReader<HType, PortTag>::call_importer(const std::string &/*filename*/,
				    HType & /*handle*/ )
{
  return false;
}


template <class HType, class PortTag> 
void
GenericReader<HType, PortTag>::execute()
{
#ifdef SCIRUN4_CODE_TO_BE_ENABLED_LATER
  bool filename_changed = gui_filename_.changed();
  
  if (gui_from_env_.get() != "")
  {
    std::string filename_from_env = gui_from_env_.get(); 
    if (sci_getenv(filename_from_env))
    {
      std::string envfilename = sci_getenv(filename_from_env);
      gui_filename_.set(envfilename);
      get_ctx()->reset();
      filename_changed = true;
    }
  }
#endif

  // If there is an optional input string set the filename to it in the GUI.
  //TODO: this will be a common pattern for file loading. Perhaps it will be a base class method someday...
  auto fileOption = getOptionalInput(Filename);
  if (!fileOption)
    filename_ = get_state()->getValue(stateFilename_).getString();
  else
    filename_ = (*fileOption)->value();
  
  // Read the status of this file so we can compare modification timestamps

  if(filename_.empty()) 
  {
    error("No file has been selected.  Please choose a file.");
    return;
  } 
  else if (!boost::filesystem::exists(filename_)) 
  {
    if (!importing_)
    {
      error("File '" + filename_ + "' not found.");
      return;
    }
    else
    {
      warning("File '" + filename_ + "' not found.  Maybe the plugin can find it?");
    }
  }

  // If we haven't read yet, or if it's a new filename, 
  //  or if the datestamp has changed -- then read...

  time_t new_filemodification = boost::filesystem::last_write_time(filename_);

  if( 
#ifdef SCIRUN4_CODE_TO_BE_ENABLED_LATER
    inputs_changed_ || filename_changed ||
      new_filemodification != old_filemodification_ ||
      !oport_cached(0) ||
      !oport_cached("Filename") 
#else
    true
#endif
      )
  {
#ifdef SCIRUN4_CODE_TO_BE_ENABLED_LATER
    update_state(Executing);  
#endif
    old_filemodification_ = new_filemodification;

    HType handle;

    remark("loading file " +filename_);
    
    if (importing_)
    {
      if (!call_importer(filename_, handle))
      {
        error("Import failed.");
        return;
      }
    }
    else
    {
      PiostreamPtr stream = auto_istream(filename_, getLogger());
      if (!stream)
      {
        error("Error reading file '" + filename_ + "'.");
        return;
      }
    
      // Read the file
      Pio(*stream, handle);

      if (!handle || stream->error())
      {
        error("Error reading data from file '" + filename_ +"'.");
        return;
      }
    }

    Core::Datatypes::StringHandle shandle(new Core::Datatypes::String(filename_));
    sendOutput(Filename, shandle);
    sendOutput(objectPortName_, handle);
  }

}

} }}

#endif