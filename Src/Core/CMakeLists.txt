include(Common)

project(Core VERSION 1.0.0 DESCRIPTION "Core functions")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library (${PROJECT_NAME}
             "src/Core.cpp" 
             "src/geometry/Shape.cpp"
             "src/geometry/ShapeFactory.cpp"
             "src/geometry/Contour.cpp" "include/internal/geometry/Transformation.h" "src/geometry/Transformation.cpp")

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

