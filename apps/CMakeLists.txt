

include("${ROOT_USE_FILE}")
separate_arguments(ROOT_EXE_LINKER_FLAGS)

##############################################################################################
add_executable(seir1 main-seir1.cpp)
target_compile_features(seir1 PRIVATE cxx_std_17)
target_compile_options(seir1 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(seir1 PRIVATE -fsanitize=address)
target_link_libraries(seir1 PRIVATE standard_solver)
target_link_libraries(seir1 PRIVATE lyra)
target_link_libraries(seir1 PUBLIC ${ROOT_LIBRARIES} ${ROOT_EXE_LINKER_FLAGS})
target_include_directories(seir1 PUBLIC "${PROJECT_BINARY_DIR}"
                                        ../include/seir)
target_clangformat_setup(seir1)
##############################################################################################
add_executable(seir2 main-seir2.cpp)
target_compile_features(seir2 PRIVATE cxx_std_17)
target_compile_options(seir2 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(seir2 PRIVATE -fsanitize=address)
target_link_libraries(seir2 PRIVATE runge_kutta_solver)
target_link_libraries(seir2 PRIVATE lyra)
target_link_libraries(seir2 PUBLIC ${ROOT_LIBRARIES} ${ROOT_EXE_LINKER_FLAGS})
target_include_directories(seir2 PUBLIC "${PROJECT_BINARY_DIR}"
                                         ../include/seir)
target_clangformat_setup(seir2)
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
target_link_libraries(sim-graphics PRIVATE  simulation
                                            mobility_model
                                            person
                                            world
                                            cluster
                                            group
                                            rectangle
                                            location
                                            position
                                            random)
target_link_libraries(sim-graphics PRIVATE lyra)
target_link_libraries(sim-graphics PRIVATE sfml-graphics)
target_include_directories(sim-graphics PUBLIC "${PROJECT_BINARY_DIR}"
                                               ../include/simulation)
target_clangformat_setup(sim-graphics) ##allow for code formatting

