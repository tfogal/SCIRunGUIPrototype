###########################################
# TinyXML project

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(TinyXML)
SET(TinyXML_Library TinyXML CACHE INTERNAL "TinyXML library name." FORCE)

SET(TINYXML_SOURCES
    tinyxml.cpp
    tinyxmlparser.cpp 
    tinyxmlerror.cpp 
    tinystr.cpp
)

SET(TINYXML_HEADERS
    tinystr.h
    tinyxml.h
)

SET(TinyXML_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR} 
    CACHE INTERNAL "TinyXML include directories." FORCE)

ADD_LIBRARY(${TinyXML_Library} STATIC ${TINYXML_SOURCES} ${TINYXML_HEADERS})
