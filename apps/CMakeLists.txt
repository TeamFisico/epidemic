
add_executable(app1 main1.cpp)
add_executable(app2 main2.cpp)

include("${ROOT_USE_FILE}")

separate_arguments(ROOT_EXE_LINKER_FLAGS)

target_compile_features(app1 PRIVATE cxx_std_17)
target_compile_options(app1 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(app1 PRIVATE -fsanitize=address)
target_link_libraries(app1 PRIVATE modern_library)
target_link_libraries(app1 PUBLIC ${ROOT_LIBRARIES} ${ROOT_EXE_LINKER_FLAGS})

target_include_directories(app1 PUBLIC "${PROJECT_BINARY_DIR}"
                                         ../include/epidemic1)


target_compile_features(app2 PRIVATE cxx_std_17)
target_compile_options(app2 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(app2 PRIVATE -fsanitize=address)
target_link_libraries(app2 PRIVATE modern_library)
target_include_directories(app2 PUBLIC "${PROJECT_BINARY_DIR}")



