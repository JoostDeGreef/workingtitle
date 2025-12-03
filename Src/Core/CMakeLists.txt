include(Common)

project(Core VERSION 1.0.0 DESCRIPTION "Core functions")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library (${PROJECT_NAME}
             "src/Core.cpp" 
             "src/geometry/Shape.cpp" "include/internal/geometry/BoundingSphere.h" "include/internal/geometry/IBoundingObject.h" "include/internal/geometry/Normal.h" "include/internal/geometry/Normals.h" "include/internal/geometry/Edge.h" "include/internal/geometry/Edges.h" "include/internal/geometry/FaceVisitor.h" "include/internal/geometry/ShapeFactory.h" "src/geometry/ShapeFactory.cpp")

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

