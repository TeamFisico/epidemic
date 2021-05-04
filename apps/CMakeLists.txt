
add_executable(app1 main1.cpp)
add_executable(app2 main2.cpp)


target_compile_features(app1 PRIVATE cxx_std_17)
target_compile_options(app1 PRIVATE -Wall -Wextra)
target_link_libraries(app1 PRIVATE modern_library)
target_include_directories(app1 PUBLIC "${PROJECT_BINARY_DIR}"
                                         ../include/epidemic1)


target_compile_features(app2 PRIVATE cxx_std_17)
target_compile_options(app2 PRIVATE -Wall -Wextra)
target_link_libraries(app2 PRIVATE modern_library)
target_include_directories(app2 PUBLIC "${PROJECT_BINARY_DIR}")



