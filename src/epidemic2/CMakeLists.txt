


########## PERSON  #############
add_library(person2 person.cpp ${HEADER_LIST2})
target_include_directories(person2 PUBLIC ../../include/epidemic2
                                          ../../include/)
target_compile_features(person2 PUBLIC cxx_std_17)
target_compile_options(person2 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(person2 PRIVATE -fsanitize=address)
target_clangformat_setup(person2) ##allow for code formatting

########## POSITION  #############
add_library(position position.cpp ${HEADER_LIST2})
target_include_directories(position PUBLIC ../../include/epidemic2
                                           ../../include/)
target_compile_features(position PUBLIC cxx_std_17)
target_compile_options(position PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(position PRIVATE -fsanitize=address)
target_clangformat_setup(position) ##allow for code formatting

########## SIMULATION  #############
add_library(simulation2 simulation.cpp ${HEADER_LIST2})
target_include_directories(simulation2 PUBLIC ../../include/epidemic2
                                              ../../include/)
target_compile_features(simulation2 PUBLIC cxx_std_17)
target_compile_options(simulation2 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(simulation2 PRIVATE -fsanitize=address)
target_clangformat_setup(simulation2) ##allow for code formatting

########## RECTANGLE  #############
add_library(rectangle rectangle.cpp ${HEADER_LIST2})
target_include_directories(rectangle PUBLIC ../../include/epidemic2
                                            ../../include/)
target_compile_features(rectangle PUBLIC cxx_std_17)
target_compile_options(rectangle PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(rectangle PRIVATE -fsanitize=address)
target_clangformat_setup(rectangle) ##allow for code formatting


########## LOCATION  #############
add_library(location2 location.cpp ${HEADER_LIST2})
target_include_directories(location2 PUBLIC ../../include/epidemic2
                                            ../../include/)
target_compile_features(location2 PUBLIC cxx_std_17)
target_compile_options(location2 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(location2 PRIVATE -fsanitize=address)
target_clangformat_setup(location2) ##allow for code formatting

########## GROUP  #############
add_library(group2 group.cpp ${HEADER_LIST2})
target_include_directories(group2 PUBLIC ../../include/epidemic2
                                         ../../include/)
target_compile_features(group2 PUBLIC cxx_std_17)
target_compile_options(group2 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(group2 PRIVATE -fsanitize=address)
target_clangformat_setup(group2) ##allow for code formatting

########## CLUSTER  #############
add_library(cluster2 cluster.cpp ${HEADER_LIST2})
target_include_directories(cluster2 PUBLIC ../../include/epidemic2
                                           ../../include/)
target_compile_features(cluster2 PUBLIC cxx_std_17)
target_compile_options(cluster2 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(cluster2 PRIVATE -fsanitize=address)
target_clangformat_setup(cluster2) ##allow for code formatting

########## WORLD  #############
add_library(world world.cpp ${HEADER_LIST2})
target_include_directories(world PUBLIC ../../include/epidemic2
                                        ../../include/)
target_compile_features(world PUBLIC cxx_std_17)
target_compile_options(world PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(world PRIVATE -fsanitize=address)
target_clangformat_setup(world) ##allow for code formatting

########## MOBILITY_MODEL  #############
add_library(mobility_model2 mobility_model.cpp ${HEADER_LIST2})
target_include_directories(mobility_model2 PUBLIC ../../include/epidemic2
                                                  ../../include/)
target_compile_features(mobility_model2 PUBLIC cxx_std_17)
target_compile_options(mobility_model2 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(mobility_model2 PRIVATE -fsanitize=address)
target_clangformat_setup(mobility_model2) ##allow for code formatting

source_group(
        TREE "${PROJECT_SOURCE_DIR}/include/epidemic2"
        PREFIX "Header Files"
        FILES ${HEADER_LIST2})

