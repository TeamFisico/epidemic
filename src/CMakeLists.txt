

set(HEADER_LIST1 "${epidemic_SOURCE_DIR}/include/epidemic1/SEIR.hpp"
                 "${epidemic_SOURCE_DIR}/include/epidemic1/SEIRode.hpp")

###################################################
#####             1st part        #################
###################################################

########## STANDARD ODE DISCRETIZATION LIBRARY #############
add_library(standard_solver SEIR.cpp ${HEADER_LIST1})
target_include_directories(standard_solver PUBLIC ../include)
target_compile_features(standard_solver PUBLIC cxx_std_17)
target_compile_options(standard_solver PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(standard_solver PRIVATE -fsanitize=address)

target_clangformat_setup(standard_solver) ##allow for code formatting

########## RUNGE-KUTTA 4TH ORDER METHOD LIBRARY #############
add_library(runge_kutta_solver SEIRode.cpp ${HEADER_LIST1})
target_include_directories(runge_kutta_solver PUBLIC ../include)
target_compile_features(runge_kutta_solver PUBLIC cxx_std_17)
target_compile_options(runge_kutta_solver PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(runge_kutta_solver PRIVATE -fsanitize=address)

target_clangformat_setup(runge_kutta_solver) ##allow for code formatting

###################################################
#####             2nd part        #################
###################################################

set(HEADER_LIST2  "${epidemic_SOURCE_DIR}/include/epidemic2/position.hpp"
                  "${epidemic_SOURCE_DIR}/include/epidemic2/person.hpp"
                  "${epidemic_SOURCE_DIR}/include/epidemic2/simulation.hpp")

########## PERSON  #############
add_library(person person.cpp ${HEADER_LIST2})
target_include_directories(person PUBLIC ../include/epidemic2)
target_compile_features(person PUBLIC cxx_std_17)
target_compile_options(person PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(person PRIVATE -fsanitize=address)
target_clangformat_setup(person) ##allow for code formatting

########## POSITION  #############
add_library(position position.cpp ${HEADER_LIST2})
target_include_directories(position PUBLIC ../include/epidemic2)
target_compile_features(position PUBLIC cxx_std_17)
target_compile_options(position PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(position PRIVATE -fsanitize=address)
target_clangformat_setup(position) ##allow for code formatting

########## SIMULATION  #############
add_library(simulation simulation.cpp ${HEADER_LIST2})
target_include_directories(simulation PUBLIC ../include/epidemic2)
target_compile_features(simulation PUBLIC cxx_std_17)
target_compile_options(simulation PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(simulation PRIVATE -fsanitize=address)
target_clangformat_setup(simulation) ##allow for code formatting

########## LOCATION  #############
add_library(location location.cpp ${HEADER_LIST2})
target_include_directories(location PUBLIC ../include/epidemic2)
target_compile_features(location PUBLIC cxx_std_17)
target_compile_options(location PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(location PRIVATE -fsanitize=address)
target_clangformat_setup(location) ##allow for code formatting

########## MOBILITY_MODEL  #############
add_library(mobility_model mobility_model.cpp ${HEADER_LIST2})
target_include_directories(mobility_model PUBLIC ../include/epidemic2)
target_compile_features(mobility_model PUBLIC cxx_std_17)
target_compile_options(mobility_model PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(mobility_model PRIVATE -fsanitize=address)
target_clangformat_setup(mobility_model) ##allow for code formatting



# IDE will put headers in a nice way
source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST1}
               ${HEADER_LIST2})




