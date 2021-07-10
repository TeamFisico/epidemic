
## adding a mutual main for the tests
add_library(test_main OBJECT main.cpp)
target_include_directories(test_main PUBLIC ../extern/doctest/doctest)


add_executable(test1 test1.cpp $<TARGET_OBJECTS:test_main>)

target_include_directories(test1 PUBLIC ../include/seir
                                          ../extern/doctest/doctest)
target_compile_features(test1 PRIVATE cxx_std_17)
target_compile_options(test1 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(test1 PRIVATE -fsanitize=address)
target_link_libraries(test1 PRIVATE numeric)
add_test(NAME test1 COMMAND test1)

add_executable(test2 test2.cpp $<TARGET_OBJECTS:test_main>)

target_include_directories(test2 PUBLIC ../include/simulation
                                        ../extern/doctest/doctest)
target_compile_features(test2 PRIVATE cxx_std_17)
target_compile_options(test2 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(test2 PRIVATE -fsanitize=address)
target_link_libraries(test2 PRIVATE     simulation
                                        mobility_model
                                        person
                                        world
                                        cluster
                                        group
                                        rectangle
                                        location
                                        position
                                        random)
add_test(NAME test2 COMMAND test2 )

add_executable(test3 test3.cpp $<TARGET_OBJECTS:test_main>)

target_include_directories(test3 PUBLIC ../include/simulation
        ../extern/doctest/doctest)
target_compile_features(test3 PRIVATE cxx_std_17)
target_compile_options(test3 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(test3 PRIVATE -fsanitize=address)
target_link_libraries(test3 PRIVATE     simulation
                                        mobility_model
                                        person
                                        world
                                        cluster
                                        group
                                        rectangle
                                        location
                                        position
                                        random)
add_test(NAME test3 COMMAND test3 )

