############################# 1ST PART   ################################

set(HEADER_LIST1 "${epidemic_SOURCE_DIR}/include/epidemic1/SEIR.hpp"
                 "${epidemic_SOURCE_DIR}/include/epidemic1/SEIRode.hpp")

add_subdirectory(epidemic1)

############################# 2ND PART   ################################

set(HEADER_LIST2  "${epidemic_SOURCE_DIR}/include/epidemic2/position.hpp"
                  "${epidemic_SOURCE_DIR}/include/epidemic2/person.hpp"
                  "${epidemic_SOURCE_DIR}/include/epidemic2/simulation.hpp")

add_subdirectory(epidemic2)


set(HEADER_LIST3  "${epidemic_SOURCE_DIR}/include/epidemic3/parameters.hpp"
                  "${epidemic_SOURCE_DIR}/include/epidemic3/location.hpp"
                  "${epidemic_SOURCE_DIR}/include/epidemic3/group.hpp"
                  "${epidemic_SOURCE_DIR}/include/epidemic3/cluster.hpp"
                  "${epidemic_SOURCE_DIR}/include/epidemic3/person.hpp"
                  "${epidemic_SOURCE_DIR}/include/epidemic3/simulation.hpp"
                  "${epidemic_SOURCE_DIR}/include/epidemic3/clock.hpp"
                  "${epidemic_SOURCE_DIR}/include/random.hpp")

add_subdirectory(epidemic3)
# IDE will put headers in a nice way

########## COMMON RANDOM LIBRARY  #############
add_library(random random.cpp "${epidemic_SOURCE_DIR}/include/random.hpp")
target_include_directories(random PUBLIC ../../include)
target_compile_features(random PUBLIC cxx_std_17)
target_compile_options(random PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(random PRIVATE -fsanitize=address)
target_clangformat_setup(random) ##allow for code formatting





