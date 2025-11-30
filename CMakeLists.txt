cmake_minimum_required(VERSION 3.14)

# No idea what to call this yet
project ("workintitle" VERSION 0.0.1)

# Output will land here
SET(GLOBAL_PREFIX
    ${PROJECT_SOURCE_DIR}/out/${CMAKE_BUILD_TYPE} CACHE
    PATH "Directory where all the output files go." FORCE)
SET(GLOBAL_BUILD_DIRECTORY
    ${GLOBAL_PREFIX}/build CACHE
    PATH "Directory where all the builds go." FORCE)
SET(CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/cmake CACHE
    PATH "Directory where .cmake files are stored." FORCE)

include(Common)

# Include sub-projects.
add_subdirectory ("3rdParty")
add_subdirectory ("Src")

