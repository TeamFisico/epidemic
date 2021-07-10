
########## DISPLAY  #############
add_library(graphics display.cpp ${HEADER_LIST_GRAPH})
target_include_directories(graphics PUBLIC ../../../include/simulation)
target_compile_features(graphics PUBLIC cxx_std_17)
target_compile_options(graphics PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(graphics PRIVATE -fsanitize=address)
target_link_libraries(graphics PRIVATE sfml-graphics)
target_clangformat_setup(graphics) ##allow for code formatting