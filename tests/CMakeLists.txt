# Testing library
FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/onqtam/doctest.git
        GIT_TAG 2.4.6)
FetchContent_MakeAvailable(doctest)

# Tests need to be added as executables first
add_executable(testlib testlib.cpp)

target_compile_features(test PRIVATE cxx_std_17)
target_compile_options(test PRIVATE -Wall -Wextra -fsanitize=address)
target_link_libraries(test PRIVATE doctest::doctest)

# If you register a test, then ctest and make test will run it. You can also run
# examples and check the output, as well.
add_test(NAME testlibtest COMMAND testlib) # Command can be a target