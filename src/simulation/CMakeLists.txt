
########## RANDOM  #############
add_library(random random.cpp ${HEADER_LIST_SIM})
target_include_directories(random PUBLIC ../../include/simulation)
target_compile_features(random PUBLIC cxx_std_17)
target_compile_options(random PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(random PRIVATE -fsanitize=address)
target_clangformat_setup(random) ##allow for code formatting

########## POSITION  #############
add_library(position position.cpp ${HEADER_LIST_SIM})
target_include_directories(position PUBLIC ../../include/simulation)
target_compile_features(position PUBLIC cxx_std_17)
target_compile_options(position PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(position PRIVATE -fsanitize=address)
target_clangformat_setup(position) ##allow for code formatting

########## LOCATION  #############
add_library(location location.cpp ${HEADER_LIST_SIM})
target_include_directories(location PUBLIC ../../include/simulation)
target_compile_features(location PUBLIC cxx_std_17)
target_compile_options(location PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(location PRIVATE -fsanitize=address)
target_clangformat_setup(location) ##allow for code formatting

########## RECTANGLE  #############
add_library(rectangle rectangle.cpp ${HEADER_LIST_SIM})
target_include_directories(rectangle PUBLIC ../../include/simulation)
target_compile_features(rectangle PUBLIC cxx_std_17)
target_compile_options(rectangle PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(rectangle PRIVATE -fsanitize=address)
target_clangformat_setup(rectangle) ##allow for code formatting

########## GROUP  #############
add_library(group group.cpp ${HEADER_LIST_SIM})
target_include_directories(group PUBLIC ../../include/simulation)
target_compile_features(group PUBLIC cxx_std_17)
target_compile_options(group PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(group PRIVATE -fsanitize=address)
target_clangformat_setup(group) ##allow for code formatting

########## CLUSTER  #############
add_library(cluster cluster.cpp ${HEADER_LIST_SIM})
target_include_directories(cluster PUBLIC ../../include/simulation)
target_compile_features(cluster PUBLIC cxx_std_17)
target_compile_options(cluster PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(cluster PRIVATE -fsanitize=address)
target_clangformat_setup(cluster) ##allow for code formatting

########## WORLD  #############
add_library(world world.cpp ${HEADER_LIST_SIM})
target_include_directories(world PUBLIC ../../include/simulation)
target_compile_features(world PUBLIC cxx_std_17)
target_compile_options(world PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(world PRIVATE -fsanitize=address)
target_clangformat_setup(world) ##allow for code formatting

########## PERSON  #############
add_library(person person.cpp ${HEADER_LIST_SIM})
target_include_directories(person PUBLIC ../../include/simulation)
target_compile_features(person PUBLIC cxx_std_17)
target_compile_options(person PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(person PRIVATE -fsanitize=address)
target_clangformat_setup(person) ##allow for code formatting

########## MOBILITY_MODEL  #############
add_library(mobility_model mobility_model.cpp ${HEADER_LIST_SIM})
target_include_directories(mobility_model PUBLIC ../../include/simulation)
target_compile_features(mobility_model PUBLIC cxx_std_17)
target_compile_options(mobility_model PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(mobility_model PRIVATE -fsanitize=address)
target_clangformat_setup(mobility_model) ##allow for code formatting

########## SIMULATION  #############
add_library(simulation simulation.cpp ${HEADER_LIST_SIM})
target_include_directories(simulation PUBLIC ../../include/simulation)
target_compile_features(simulation PUBLIC cxx_std_17)
target_compile_options(simulation PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(simulation PRIVATE -fsanitize=address)
target_clangformat_setup(simulation) ##allow for code formatting

source_group(
        TREE "${PROJECT_SOURCE_DIR}/include/simulation"
        PREFIX "Header Files"
        FILES ${HEADER_LIST_SIM})

