include(Common)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_TAG v1.16.0
    GIT_REPOSITORY https://github.com/google/googletest.git
    FIND_PACKAGE_ARGS 1.16.0 NAMES GTest COMPONENTS gmain_main
    EXCLUDE_FROM_ALL
    SYSTEM
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

project(googletest-runner)

add_library (${PROJECT_NAME}
             "src-extra/ConsoleOutput.cpp"
             "src-extra/ConsoleOutput.h"
             "src-extra/GoogleTestMain.cpp"
             "src-extra/VisualStudioOutput.cpp"
             "src-extra/VisualStudioOutput.h")

target_link_libraries(${PROJECT_NAME} PUBLIC GTest::gtest)

