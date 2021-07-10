


########## NUMERICAL METHODS LIBRARY #############
add_library(numeric seir.cpp ${HEADER_LIST_SEIR})
target_include_directories(numeric PUBLIC ../../include/seir)
target_compile_features(numeric PUBLIC cxx_std_17)
target_compile_options(numeric PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(numeric PRIVATE -fsanitize=address)
target_clangformat_setup(numeric) ##allow for code formatting

source_group(
        TREE "${PROJECT_SOURCE_DIR}/include/seir"
        PREFIX "Header Files"
        FILES ${HEADER_LIST_SEIR})
