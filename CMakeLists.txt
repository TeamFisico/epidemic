cmake_minimum_required(VERSION 3.16)
cmake_policy(VERSION 3.16)
project(epidemic
        VERSION 3.0
        DESCRIPTION "Modelling the spread of an epidemic"
        LANGUAGES CXX)

############################# PREVENT IN-SOURCE BUILD #############################
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with "
            "CMakeLists.txt file). Please make a build subdirectory. Feel free to "
            "remove CMakeCache.txt and CMakeFiles.")
endif()

############################# ALLOW EPIDEMIC TO BE INCLUDE AS A SUBDIR #############################
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

    include(ClangFormat)

    #support IDE to show all the folders
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

endif() #main project

############################# GIT SUBMODULES  #############################
## Automate submodules of this project update at build time
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Update submodules of this project")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif() #git submodules

############################# SFML GRAPHICS LIBRARY #############################
find_package(SFML 2.5 COMPONENTS system window graphics)

if(SFML_FOUND)
    message(STATUS "SFML package found")
    include_directories(${SFML_INCLUDE_DIR})
endif()

############################# ROOT CERN LIBRARY #############################
find_package(ROOT 6.16 CONFIG REQUIRED)
if(ROOT_FOUND)
    message(STATUS "ROOT package found")
endif()

# Get the generate dictionary command from ROOT
if(ROOT_VERSION VERSION_LESS 6.20)
    include("${ROOT_DIR}/modules/RootNewMacros.cmake")
else()
    include("${ROOT_DIR}/RootMacros.cmake")
endif()

# The compiled library code is here
add_subdirectory(src)

#The bundled external library code is here
add_subdirectory(extern)

# The executable code is here
add_subdirectory(apps)

# Test code is here
### Testing targets will be built only if in Debug mode
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    enable_testing()
    add_subdirectory(tests)
endif()


