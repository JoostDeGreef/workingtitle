include(Common)

project(CoreTest)

link_libraries(googletest-runner
               Core)

add_executable (${PROJECT_NAME}
                "generic/ConstantsTest.cpp"
                "generic/LimitsTest.cpp"
                "generic/NumericsTest.cpp"
                "generic/StackAllocatorTest.cpp"
                "geometry/BoundingObjectTest.cpp" 
                "geometry/FaceTest.cpp"
                "geometry/ShapeTest.cpp" 
                "geometry/VertexTest.cpp"
                "utilities/SVGTest.cpp")

