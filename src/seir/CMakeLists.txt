

########## STANDARD ODE DISCRETIZATION LIBRARY #############
add_library(standard_solver SEIR.cpp ${HEADER_LIST_SEIR})
target_include_directories(standard_solver PUBLIC ../../include/seir)
target_compile_features(standard_solver PUBLIC cxx_std_17)
target_compile_options(standard_solver PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(standard_solver PRIVATE -fsanitize=address)
target_clangformat_setup(standard_solver) ##allow for code formatting

########## RUNGE-KUTTA 4TH ORDER METHOD LIBRARY #############
add_library(runge_kutta_solver SEIRode.cpp ${HEADER_LIST_SEIR})
target_include_directories(runge_kutta_solver PUBLIC ../../include/seir)
target_compile_features(runge_kutta_solver PUBLIC cxx_std_17)
target_compile_options(runge_kutta_solver PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(runge_kutta_solver PRIVATE -fsanitize=address)
target_clangformat_setup(runge_kutta_solver) ##allow for code formatting

source_group(
        TREE "${PROJECT_SOURCE_DIR}/include/seir"
        PREFIX "Header Files"
        FILES ${HEADER_LIST_SEIR})
