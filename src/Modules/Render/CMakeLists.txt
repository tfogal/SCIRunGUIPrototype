#
#  For more information, please see: http://software.sci.utah.edu
# 
#  The MIT License
# 
#  Copyright (c) 2012 Scientific Computing and Imaging Institute,
#  University of Utah.
# 
#  
#  Permission is hereby granted, free of charge, to any person obtaining a
#  copy of this software and associated documentation files (the "Software"),
#  to deal in the Software without restriction, including without limitation
#  the rights to use, copy, modify, merge, publish, distribute, sublicense,
#  and/or sell copies of the Software, and to permit persons to whom the
#  Software is furnished to do so, subject to the following conditions:
# 
#  The above copyright notice and this permission notice shall be included
#  in all copies or substantial portions of the Software. 
# 
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
#  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#  DEALINGS IN THE SOFTWARE.
#

SET(Modules_Render_SRCS
  ViewScene.cc
)

SET(Modules_Render_HEADERS
  ViewScene.h
  share.h
)

FIND_PACKAGE(OpenGL REQUIRED)

# Framework for spire
if(UNIX)
  if(APPLE)
    find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
    set(SCI_SPIRE_EXTRA_LIBRARIES ${CORE_FOUNDATION_FRAMEWORK})
  endif()
endif()

ADD_LIBRARY(Modules_Render 
  ${Modules_Render_HEADERS}
  ${Modules_Render_SRCS}
)

TARGET_LINK_LIBRARIES(Modules_Render
  Dataflow_Network
  Core_Datatypes
  ${OPENGL_LIBRARIES}
  ${SCI_SPIRE_LIBRARY}
  ${SCI_SPIRE_EXTRA_LIBRARIES}
)

IF(BUILD_SHARED_LIBS)
  ADD_DEFINITIONS(-DBUILD_Modules_Render)
ENDIF(BUILD_SHARED_LIBS)

ADD_SUBDIRECTORY(Tests)

