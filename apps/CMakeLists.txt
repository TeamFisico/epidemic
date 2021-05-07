
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


##############################
##      SECOND PART        ###
##############################
#add_executable(app2 main2.cpp)
#
#target_compile_features(app2 PRIVATE cxx_std_17)
#target_compile_options(app2 PRIVATE -Wall -Wextra -fsanitize=address)
#target_link_options(app2 PRIVATE -fsanitize=address)
#target_link_libraries(app2 PRIVATE modern_library)
#target_include_directories(app2 PUBLIC "${PROJECT_BINARY_DIR}")



