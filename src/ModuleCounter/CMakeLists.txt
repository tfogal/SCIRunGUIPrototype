CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)

# As of CMake 2.6 policies were introduced in order to provide a mechanism for
# adding backwards compatibility one feature at a time.  We will just specify
# that all policies will use version 2.6 semantics.
cmake_policy(VERSION 2.6)

PROJECT(SCI_Module_Counter)

ADD_SUBDIRECTORY(Utilities)

INCLUDE_DIRECTORIES(${SCI_BOOST_INCLUDE} ${TinyXML_INCLUDE})

SET(ModuleCounter_SRCS
  main.cpp
  ModuleCounter.cpp
  )
  
SET(ModuleCounter_HEADERS
  ModuleCounter.h
  )
  
ADD_EXECUTABLE(${PROJECT_NAME} ${ModuleCounter_SRCS} ${ModuleCounter_HEADERS})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SCI_BOOST_LIBRARY} ${TinyXML_Library})
