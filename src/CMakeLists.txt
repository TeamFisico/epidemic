

set(HEADER_LIST "${epidemic_SOURCE_DIR}/include/epidemic1/SEIR.hpp")

# Make an automatic library - will be static or dynamic based on user setting
# nome da dare
add_library(modern_library SEIR.cpp ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(modern_library PUBLIC ../include)

# This depends on (header only) boost
#target_link_libraries(modern_library PRIVATE Boost::boost)

# All users of this library will need at least C++17
target_compile_features(modern_library PUBLIC cxx_std_17)
target_compile_options(modern_library PRIVATE -Wall -Wextra -fsanitize=address)

# IDEs should put the headers in a nice place
source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST})



