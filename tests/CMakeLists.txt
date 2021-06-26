

enable_testing()

add_library(test_main OBJECT test_main.cpp)
target_include_directories(test_main PUBLIC ../include)

## adding a mutual main for the tests
add_executable(app1.0.t test1.cpp $<TARGET_OBJECTS:test_main>)

target_include_directories(app1.0.t PUBLIC ../include/epidemic1)
target_compile_features(app1.0.t PRIVATE cxx_std_17)
target_compile_options(app1.0.t PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(app1.0.t PRIVATE -fsanitize=address)
target_link_libraries(app1.0.t PRIVATE standard_solver)
add_test(NAME app1.0.t COMMAND app1.0.t)

#target_clangformat_setup(app1.0.t) ##allow for code formatting


add_executable(test2 test2.cpp)

target_include_directories(test2 PUBLIC ../include/epidemic2)
target_compile_features(test2 PRIVATE cxx_std_17)
target_compile_options(test2 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(test2 PRIVATE -fsanitize=address)
target_link_libraries(test2 PRIVATE simulation2 world rectangle cluster2 mobility_model2 group2 location2 person2 position random)
add_test(NAME test2 COMMAND test2 )


