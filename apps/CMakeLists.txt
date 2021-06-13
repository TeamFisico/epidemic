
add_executable(app1.0 main_std1.cpp)
add_executable(app1.1 main_rks.cpp)

include("${ROOT_USE_FILE}")

separate_arguments(ROOT_EXE_LINKER_FLAGS)

target_compile_features(app1.0 PRIVATE cxx_std_17)
target_compile_options(app1.0 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(app1.0 PRIVATE -fsanitize=address)
target_link_libraries(app1.0 PRIVATE standard_solver)
target_link_libraries(app1.0 PUBLIC ${ROOT_LIBRARIES} ${ROOT_EXE_LINKER_FLAGS})

target_include_directories(app1.0 PUBLIC "${PROJECT_BINARY_DIR}"
                                         ../include/epidemic1)
target_clangformat_setup(app1.0) ##allow for code formatting



target_compile_features(app1.1 PRIVATE cxx_std_17)
target_compile_options(app1.1 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(app1.1 PRIVATE -fsanitize=address)
target_link_libraries(app1.1 PRIVATE runge_kutta_solver)
target_link_libraries(app1.1 PUBLIC ${ROOT_LIBRARIES} ${ROOT_EXE_LINKER_FLAGS})

target_include_directories(app1.1 PUBLIC "${PROJECT_BINARY_DIR}"
                                          ../include/epidemic1)
target_clangformat_setup(app1.1) ##allow for code formatting


####################################################################################
############                    2nd part                          ##################
####################################################################################
add_executable(app2 main2.cpp)

target_compile_features(app2 PRIVATE cxx_std_17)
target_compile_options(app2 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(app2 PRIVATE -fsanitize=address)
target_link_libraries(app2 PRIVATE position
                                   person2
                                   simulation2
                                   random)
target_include_directories(app2 PUBLIC "${PROJECT_BINARY_DIR}"
                                    ../include/epidemic2)
target_clangformat_setup(app2) ##allow for code formatting

add_executable(app3 main3.cpp)

target_compile_features(app3 PRIVATE cxx_std_17)
target_compile_options(app3 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(app3 PRIVATE -fsanitize=address)
target_link_libraries(app3 PRIVATE   position3
                                     location3
                                     person3
                                     group3
                                     cluster3
                                     simulation3
                                     clock3
                                     random)
target_include_directories(app3 PUBLIC "${PROJECT_BINARY_DIR}" ../include/epidemic3
                                                               ../include)
target_clangformat_setup(app3) ##allow for code formatting

