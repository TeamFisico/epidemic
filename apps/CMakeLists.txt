

include("${ROOT_USE_FILE}")
separate_arguments(ROOT_EXE_LINKER_FLAGS)

##############################################################################################
add_executable(seir main-seir.cpp)
target_compile_features(seir PRIVATE cxx_std_17)
target_compile_options(seir PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(seir PRIVATE -fsanitize=address)
target_link_libraries(seir PRIVATE numeric)
target_link_libraries(seir PRIVATE lyra)
target_link_libraries(seir PUBLIC ${ROOT_LIBRARIES} ${ROOT_EXE_LINKER_FLAGS})
target_include_directories(seir PUBLIC "${PROJECT_BINARY_DIR}"
                                         ../include/seir
                                         ../include/simulation)
target_clangformat_setup(seir)
##############################################################################################
add_executable(sim main-sim.cpp)
target_compile_features(sim PRIVATE cxx_std_17)
target_compile_options(sim PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(sim PRIVATE -fsanitize=address)
target_link_libraries(sim PRIVATE   simulation
                                    mobility_model
                                    person
                                    world
                                    cluster
                                    group
                                    rectangle
                                    location
                                    position
                                    random)
target_link_libraries(sim PUBLIC ${ROOT_LIBRARIES} ${ROOT_EXE_LINKER_FLAGS})
target_link_libraries(sim PRIVATE lyra)
target_include_directories(sim PUBLIC "${PROJECT_BINARY_DIR}"
                                      ../include/simulation)
target_clangformat_setup(sim) ##allow for code formatting
##############################################################################################
add_executable(sim-graphics main-sim-graphics.cpp)
target_compile_features(sim-graphics PRIVATE cxx_std_17)
target_compile_options(sim-graphics PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(sim-graphics PRIVATE -fsanitize=address)
target_link_libraries(sim-graphics PRIVATE  graphics
                                            simulation
                                            mobility_model
                                            person
                                            world
                                            cluster
                                            group
                                            rectangle
                                            location
                                            position
                                            random)
target_link_libraries(sim-graphics PUBLIC ${ROOT_LIBRARIES} ${ROOT_EXE_LINKER_FLAGS})
target_link_libraries(sim-graphics PRIVATE lyra)
target_link_libraries(sim-graphics PRIVATE sfml-graphics)
target_include_directories(sim-graphics PUBLIC "${PROJECT_BINARY_DIR}"
                                               ../include/simulation
                                                ../src/simulation/graphics)
target_clangformat_setup(sim-graphics) ##allow for code formatting

