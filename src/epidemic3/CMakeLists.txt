

########## CLUSTER  #############
add_library(cluster3 cluster.cpp ${HEADER_LIST3})
target_include_directories(cluster3 PUBLIC ../../include/epidemic3)
target_compile_features(cluster3 PUBLIC cxx_std_17)
target_compile_options(cluster3 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(cluster3 PRIVATE -fsanitize=address)
target_clangformat_setup(cluster3) ##allow for code formatting

########## GROUP  #############
add_library(group3 group.cpp ${HEADER_LIST3})
target_include_directories(group3 PUBLIC ../../include/epidemic3)
target_compile_features(group3 PUBLIC cxx_std_17)
target_compile_options(group3 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(group3 PRIVATE -fsanitize=address)
target_clangformat_setup(group3) ##allow for code formatting

########## LOCATION  #############
add_library(location3 location.cpp ${HEADER_LIST3})
target_include_directories(location3 PUBLIC ../../include/epidemic3)
target_compile_features(location3 PUBLIC cxx_std_17)
target_compile_options(location3 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(location3 PRIVATE -fsanitize=address)
target_clangformat_setup(location3) ##allow for code formatting

########## PERSON  #############
add_library(person3 person.cpp ${HEADER_LIST3})
target_include_directories(person3 PUBLIC ../../include/epidemic3)
target_compile_features(person3 PUBLIC cxx_std_17)
target_compile_options(person3 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(person3 PRIVATE -fsanitize=address)
target_clangformat_setup(person3) ##allow for code formatting

########## SIMULATION  #############
add_library(random3 simulation.cpp ${HEADER_LIST3})
target_include_directories(random3 PUBLIC ../../include/epidemic3)
target_compile_features(random3 PUBLIC cxx_std_17)
target_compile_options(random3 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(random3 PRIVATE -fsanitize=address)
target_clangformat_setup(random3) ##allow for code formatting

########## SIMULATION  #############
add_library(simulation3 simulation.cpp ${HEADER_LIST3})
target_include_directories(simulation3 PUBLIC ../../include/epidemic3)
target_compile_features(simulation3 PUBLIC cxx_std_17)
target_compile_options(simulation3 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(simulation3 PRIVATE -fsanitize=address)
target_clangformat_setup(simulation3) ##allow for code formatting

source_group(
        TREE "${PROJECT_SOURCE_DIR}/include/epidemic3"
        PREFIX "Header Files"
        FILES ${HEADER_LIST3})





