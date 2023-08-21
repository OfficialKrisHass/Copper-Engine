message(STATUS "")
message(STATUS "BUILDING assimp")

list(APPEND assimp_sources code/Assimp.cpp code/BaseImporter.cpp code/BaseProcess.cpp code/Importer.cpp code/ImporterRegistry.cpp code/ColladaLoader.cpp
                           code/ColladaParser.cpp code/CreateAnimMesh.cpp code/PlyParser.cpp code/PlyLoader.cpp code/EmbedTexturesProcess.cpp code/ConvertToLHProcess.cpp
                           code/DefaultIOStream.cpp code/DefaultIOSystem.cpp code/DefaultLogger.cpp code/GenVertexNormalsProcess.cpp code/MaterialSystem.cpp
                           code/PostStepRegistry.cpp code/ProcessHelper.cpp code/scene.cpp code/ScenePreprocessor.cpp code/ScaleProcess.cpp code/SGSpatialSort.cpp
                           code/SkeletonMeshBuilder.cpp code/SpatialSort.cpp code/TriangulateProcess.cpp code/ValidateDataStructure.cpp code/Version.cpp code/VertexTriangleAdjacency.cpp
                           code/MakeVerboseFormat.cpp code/CalcTangentsProcess.cpp code/ScaleProcess.cpp code/EmbedTexturesProcess.cpp contrib/irrXML/irrXML.cpp code/ObjFileImporter.cpp
                           code/ObjFileMtlImporter.cpp code/ObjFileParser.cpp code/FBXImporter.cpp code/FBXParser.cpp code/FBXAnimation.cpp code/FBXBinaryTokenizer.cpp
                           code/FBXConverter.cpp code/FBXDeformer.cpp code/FBXDocument.cpp code/FBXDocumentUtil.cpp code/FBXExporter.cpp code/FBXExportNode.cpp code/FBXExportProperty.cpp
                           code/FBXMaterial.cpp code/FBXMeshGeometry.cpp code/FBXModel.cpp code/FBXNodeAttribute.cpp code/FBXProperties.cpp code/FBXTokenizer.cpp code/FBXUtil.cpp
                           code/glTFImporter.cpp code/glTF2Importer.cpp contrib/zlib/zlib.h contrib/zlib/zconf.h contrib/zlib/crc32.h contrib/zlib/deflate.h contrib/zlib/gzguts.h
                           contrib/zlib/inffast.h contrib/zlib/inffixed.h contrib/zlib/inflate.h contrib/zlib/inftrees.h contrib/zlib/trees.h contrib/zlib/zutil.h contrib/zlib/adler32.c
                           contrib/zlib/compress.c contrib/zlib/crc32.c contrib/zlib/deflate.c contrib/zlib/gzclose.c contrib/zlib/gzlib.c contrib/zlib/gzread.c contrib/zlib/gzwrite.c
                           contrib/zlib/inflate.c contrib/zlib/infback.c contrib/zlib/inftrees.c contrib/zlib/inffast.c contrib/zlib/trees.c contrib/zlib/uncompr.c contrib/zlib/zutil.c)
list(APPEND assimp_defines ASSIMP_BUILD_NO_OWN_ZLIB ASSIMP_BUILD_NO_X_IMPORTER ASSIMP_BUILD_NO_3DS_IMPORTER ASSIMP_BUILD_NO_MD3_IMPORTER ASSIMP_BUILD_NO_MDL_IMPORTER
                           ASSIMP_BUILD_NO_MD2_IMPORTER ASSIMP_BUILD_NO_ASE_IMPORTER ASSIMP_BUILD_NO_AMF_IMPORTER ASSIMP_BUILD_NO_HMP_IMPORTER ASSIMP_BUILD_NO_SMD_IMPORTER
                           ASSIMP_BUILD_NO_MDC_IMPORTER ASSIMP_BUILD_NO_MD5_IMPORTER ASSIMP_BUILD_NO_STL_IMPORTER ASSIMP_BUILD_NO_LWO_IMPORTER ASSIMP_BUILD_NO_DXF_IMPORTER
                           ASSIMP_BUILD_NO_NFF_IMPORTER ASSIMP_BUILD_NO_RAW_IMPORTER ASSIMP_BUILD_NO_OFF_IMPORTER ASSIMP_BUILD_NO_AC_IMPORTER ASSIMP_BUILD_NO_BVH_IMPORTER
                           ASSIMP_BUILD_NO_IRRMESH_IMPORTER ASSIMP_BUILD_NO_IRR_IMPORTER ASSIMP_BUILD_NO_Q3D_IMPORTER ASSIMP_BUILD_NO_B3D_IMPORTER ASSIMP_BUILD_NO_TERRAGEN_IMPORTER
                           ASSIMP_BUILD_NO_CSM_IMPORTER ASSIMP_BUILD_NO_3D_IMPORTER ASSIMP_BUILD_NO_LWS_IMPORTER ASSIMP_BUILD_NO_OGRE_IMPORTER ASSIMP_BUILD_NO_OPENGEX_IMPORTER
                           ASSIMP_BUILD_NO_MS3D_IMPORTER ASSIMP_BUILD_NO_COB_IMPORTER ASSIMP_BUILD_NO_BLEND_IMPORTER ASSIMP_BUILD_NO_Q3BSP_IMPORTER ASSIMP_BUILD_NO_NDO_IMPORTER
                           ASSIMP_BUILD_NO_IFC_IMPORTER ASSIMP_BUILD_NO_XGL_IMPORTER ASSIMP_BUILD_NO_ASSBIN_IMPORTER ASSIMP_BUILD_NO_C4D_IMPORTER ASSIMP_BUILD_NO_3MF_IMPORTER
                           ASSIMP_BUILD_NO_X3D_IMPORTER ASSIMP_BUILD_NO_MMD_IMPORTER ASSIMP_BUILD_NO_STEP_EXPORTER ASSIMP_BUILD_NO_SIB_IMPORTER ASSIMP_BUILD_NO_JOINVERTICES_PROCESS
                           ASSIMP_BUILD_NO_GENFACENORMALS_PROCESS ASSIMP_BUILD_NO_REMOVEVC_PROCESS ASSIMP_BUILD_NO_SPLITLARGEMESHES_PROCESS ASSIMP_BUILD_NO_PRETRANSFORMVERTICES_PROCESS
                           ASSIMP_BUILD_NO_LIMITBONEWEIGHTS_PROCESS ASSIMP_BUILD_NO_IMPROVECACHELOCALITY_PROCESS ASSIMP_BUILD_NO_FIXINFACINGNORMALS_PROCESS
                           ASSIMP_BUILD_NO_REMOVE_REDUNDANTMATERIALS_PROCESS ASSIMP_BUILD_NO_FINDINVALIDDATA_PROCESS ASSIMP_BUILD_NO_FINDDEGENERATES_PROCESS
                           ASSIMP_BUILD_NO_SORTBYPTYPE_PROCESS ASSIMP_BUILD_NO_GENUVCOORDS_PROCESS ASSIMP_BUILD_NO_TRANSFORMTEXCOORDS_PROCESS ASSIMP_BUILD_NO_FINDINSTANCES_PROCESS
                           ASSIMP_BUILD_NO_OPTIMIZEMESHES_PROCESS ASSIMP_BUILD_NO_OPTIMIZEGRAPH_PROCESS ASSIMP_BUILD_NO_SPLITBYBONECOUNT_PROCESS ASSIMP_BUILD_NO_DEBONE_PROCESS
                           ASSIMP_BUILD_NO_EMBEDTEXTURES_PROCESS ASSIMP_BUILD_NO_GLOBALSCALE_PROCESS)

add_library(assimp STATIC ${assimp_sources})
set_target_properties(assimp PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Copper-Engine/lib/assimp/Build/linux-x86_64-${CMAKE_BUILD_TYPE}")

target_compile_definitions(assimp PRIVATE ${assimp_defines})
target_include_directories(assimp PRIVATE include contrib/irrXML contrib/zlib contrib/rapidjson/include)