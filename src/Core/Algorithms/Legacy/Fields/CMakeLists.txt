#
#  For more information, please see: http://software.sci.utah.edu
# 
#  The MIT License
# 
#  Copyright (c) 2009 Scientific Computing and Imaging Institute,
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

SET(Core_Algorithms_Legacy_Fields_HEADERS
  FieldData/CalculateFieldDataMetric.h         
  FieldData/ConvertIndicesToFieldData.h
  FieldData/CalculateGradients.h               
  FieldData/CalculateVectorMagnitudes.h        
  FieldData/ConvertMappingMatrixToFieldData.h
  FieldData/ConvertFieldDataType.h             
  FieldData/ConvertFieldBasisType.h            
  FieldData/CalculateLatVolGradientsAtNodes.h  
  FieldData/GetFieldData.h
  FieldData/SetFieldData.h
  FieldData/SetFieldDataToConstantValue.h
  FieldData/SmoothVecFieldMedian.h
)

SET(Core_Algorithms_Legacy_Fields_SRCS
  #RegisterWithCorrespondences.cc
  #MeshData/FlipSurfaceNormals.cc
  #Cleanup/CleanupTetMesh.cc
  #Cleanup/RemoveUnusedNodes.cc
  #ClipMesh/ClipMeshByIsovalue.cc
  #ClipMesh/ClipMeshBySelection.cc
  #CollectFields/CollectPointClouds.cc
  #ConvertMeshType/ConvertMeshToPointCloudMesh.cc
  #ConvertMeshType/ConvertMeshToTetVolMesh.cc
  #ConvertMeshType/ConvertMeshToTriSurfMesh.cc
  #ConvertMeshType/ConvertMeshToUnstructuredMesh.cc
  #ConvertMeshType/ConvertMeshToIrregularMesh.cc
  #ConvertMeshType/ConvertLatVolDataFromElemToNode.cc
  #ConvertMeshType/ConvertLatVolDataFromNodeToElem.cc
  #CompareFields/CompareFields.cc
  #CompareFields/SimilarMeshes.cc
  #CreateMesh/CreateMeshFromNrrd.cc
  #DistanceField/CalculateDistanceField.cc
  #DistanceField/CalculateIsInsideField.cc
  #DistanceField/CalculateInsideWhichField.cc
  #DistanceField/CalculateSignedDistanceField.cc
  #DomainFields/GetDomainBoundary.cc
  #DomainFields/GetDomainStructure.cc
  #DomainFields/MatchDomainLabels.cc
  #DomainFields/SplitNodesByDomain.cc
  #DomainFields/SplitFieldByDomain.cc
  #FieldData/CalculateGradients.cc
  #FieldData/CalculateLatVolGradientsAtNodes.cc
  #FieldData/CalculateVectorMagnitudes.cc
  #FieldData/CalculateFieldDataMetric.cc
  #FieldData/ConvertFieldDataType.cc
  #FieldData/ConvertFieldBasisType.cc
  #FieldData/ConvertIndicesToFieldData.cc
  #FieldData/GetFieldData.cc
  #FieldData/ConvertMappingMatrixToFieldData.cc
  #FieldData/SetFieldData.cc
  FieldData/SetFieldDataToConstantValue.cc
  #FieldData/SmoothVecFieldMedian.cc
  #FilterFieldData/DilateFieldData.cc
  #FilterFieldData/ErodeFieldData.cc
  #FilterFieldData/TriSurfPhaseFilter.cc
  #FindNodes/FindClosestNode.cc
  #FindNodes/FindClosestNodeByValue.cc
  #Mapping/ApplyMappingMatrix.cc
  #Mapping/BuildMappingMatrix.cc
  #Mapping/MapFieldDataFromElemToNode.cc
  #Mapping/MapFieldDataFromNodeToElem.cc
  #Mapping/MapFieldDataFromSourceToDestination.cc
  #Mapping/MappingDataSource.cc
  #Mapping/MapFieldDataOntoNodes.cc
  #Mapping/MapFieldDataOntoElems.cc
  #Mapping/MapFromPointField.cc
  #Mapping/FindClosestNodesFromPointField.cc
  #MarchingCubes/BaseMC.cc
  #MarchingCubes/EdgeMC.cc
  #MarchingCubes/HexMC.cc
  #MarchingCubes/MarchingCubes.cc
  #MarchingCubes/mcube2.cc
  #MarchingCubes/PrismMC.cc
  #MarchingCubes/QuadMC.cc
  #MarchingCubes/TetMC.cc
  #MarchingCubes/TriMC.cc
  #MarchingCubes/UHexMC.cc
  #MedialAxis/MedialAxis.cc
  #MedialAxis/MedialAxis2.cc
  #MergeFields/AppendFields.cc
  #MergeFields/JoinFields.cc
  #MeshData/GetMeshNodes.cc
  #MeshData/SetMeshNodes.cc
  #MeshData/GetSurfaceNodeNormals.cc
  #MeshData/GetSurfaceElemNormals.cc
  #MeshData/GetMeshQualityField.cc
  #MeshDerivatives/CalculateMeshConnector.cc
  #MeshDerivatives/CalculateMeshCenter.cc
  #MeshDerivatives/GetCentroids.cc
  #MeshDerivatives/GetFieldBoundary.cc
  #MeshDerivatives/GetBoundingBox.cc
  #MeshDerivatives/SplitByConnectedRegion.cc
  #RefineMesh/RefineMesh.cc
  #ResampleMesh/ResampleRegularMesh.cc
  #ResampleMesh/PadRegularMesh.cc
  #SampleField/GeneratePointSamplesFromField.cc
  #SmoothMesh/FairMesh.cc
  #StreamLines/StreamLineIntegrators.cc
  #StreamLines/GenerateStreamLines.cc
  #TransformMesh/AlignMeshBoundingBoxes.cc
  #TransformMesh/TransformMeshWithTransform.cc
  #TransformMesh/ConvertMeshCoordinateSystem.cc
  #TransformMesh/ScaleFieldMeshAndData.cc
  #TransformMesh/TransformPlanarMesh.cc
  #TransformMesh/GeneratePolarProjection.cc
  #TransformMesh/ProjectPointsOntoMesh.cc
  #TransformMesh/PolarProjectionTools.cc
  #TransformMesh/GeneratePolarProjectionImage.cc
  #TracePoints/TracePoints.cc
)

IF(WITH_TETGEN)
  SET(Core_Algorithms_Legacy_Fields_SRCS ${Core_Algorithms_Legacy_Fields_SRCS}
    TetGen/InterfaceWithTetGen.cc)
ENDIF(WITH_TETGEN)

ADD_LIBRARY(Core_Algorithms_Legacy_Fields 
  ${Core_Algorithms_Legacy_Fields_HEADERS}
  ${Core_Algorithms_Legacy_Fields_SRCS}
)

TARGET_LINK_LIBRARIES(Core_Algorithms_Legacy_Fields 
  Core_Datatypes 
  Core_Datatypes_Legacy_Field
  Algorithms_Base
  #Core_Util 
  Core_Exceptions_Legacy
  Core_Thread 
  #Core_Geom 
  Core_Geometry_Primitives
  #Core_Algorithms_Legacy_Converter
  #Core_Algorithms_Legacy_Math
  #Core_Algorithms_Util 
  #Core_Persistent 
  Core_Basis 
  #${SCI_TEEM_LIBRARY}
)

IF(WITH_TETGEN)
  TARGET_LINK_LIBRARIES(Core_Algorithms_Legacy_Fields
     ${SCI_TETGEN_LIBRARY}
  )
ENDIF(WITH_TETGEN)

IF(BUILD_SHARED_LIBS)
  ADD_DEFINITIONS(-DBUILD_Core_Algorithms_Legacy_Fields)
ENDIF(BUILD_SHARED_LIBS)
