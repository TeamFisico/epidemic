

set(HEADER_LIST "${epidemic_SOURCE_DIR}/include/epidemic1/SEIR.hpp"
        "${epidemic_SOURCE_DIR}/include/epidemic1/SEIRode.hpp")

########## STANDARD ODE DISCRETIZATION LIBRARY #############
add_library(standard_solver SEIR.cpp ${HEADER_LIST})
target_include_directories(standard_solver PUBLIC ../include)
target_compile_features(standard_solver PUBLIC cxx_std_17)
target_compile_options(standard_solver PRIVATE -Wall -Wextra -fsanitize=address)

########## RUNGE-KUTTA 4TH ORDER METHOD LIBRARY #############
add_library(runge_kutta_solver SEIRode.cpp ${HEADER_LIST})
target_include_directories(runge_kutta_solver PUBLIC ../include)
target_compile_features(runge_kutta_solver PUBLIC cxx_std_17)
target_compile_options(runge_kutta_solver PRIVATE -Wall -Wextra -fsanitize=address)

# IDEs should put the headers in a nice place
source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST})



