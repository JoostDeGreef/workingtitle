cmake_minimum_required (VERSION 3.9)

# No idea what to call this yet
project ("workintitle" VERSION 0.0.1)

SET( CMAKE_CXX_STANDARD 17 )
SET( CMAKE_CXX_STANDARD_REQUIRED ON )

set( CMAKE_INSTALL_RPATH "${ORIGIN}" )
set( CMAKE_BUILD_WITH_INSTALL_RPATH TRUE )
set( CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE )

#IF(CMAKE_COMPILER_IS_GNUCXX)
#  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopt-info-vec-all -O3")
#ENDIF(CMAKE_COMPILER_IS_GNUCXX)
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

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

# Include sub-projects.
add_subdirectory ("3rdParty")
add_subdirectory ("Src")

