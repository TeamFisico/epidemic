#to be tested with multiple versions
cmake_minimum_required(VERSION 3.19)
project(epidemic
        VERSION 1.0
        DESCRIPTION "SIR model for an epidemic"
        LANGUAGES CXX)


#do the following if this is the main project
#this project -->can be included as a subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    set(CMAKE_CXX_STANDARD 17)
    #support IDE to show all the folders
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Testing only available if this is the main app. Note this needs to be done
    # in the main CMakeLists since it calls enable_testing, which must be in the
    # main CMakeLists.
    # include(CTest)

    #doxygen for documentation generation(LaTeX)
#    find_package(Doxygen)
#    if(Doxygen_FOUND)
#        add_subdirectory(docs)
#    else()
#        message(STATUS "Doxygen not found, not building docs")
#    endif()

endif()

find_package(SFML 2.5 COMPONENTS system window graphics network audio)

if(SFML_FOUND)
    message(STATUS "SFML package found")
endif()
# The compiled library code is here
add_subdirectory(src)

# The executable code is here
add_subdirectory(apps)

# Testing only available if this is the main app
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()