include(Common)

project(CoreTest)

link_libraries(googletest-runner
               Core)

add_executable (${PROJECT_NAME}
                "NumericsTest.cpp"
                "LimitsTest.cpp"
                "ConstantsTest.cpp"
                "VertexTest.cpp"
                "TriangleTest.cpp"
                "ShapeTest.cpp" "BoundingObjectTest.cpp")

