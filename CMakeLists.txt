cmake_minimum_required(VERSION 3.16)

project(
    ip_filter
    VERSION 0.0.0
    DESCRIPTION "Parsing, sorting, filtering of IPv4 adresses list"
    LANGUAGES CXX
)

add_executable(ip_filter src/main.cpp)
add_executable(ip_filter_test test/main.cpp)

# set targets C++ standard
set_target_properties(ip_filter ip_filter_test PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

# enable compile warnings
target_compile_options(ip_filter      PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Werror)
target_compile_options(ip_filter_test PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Werror)

enable_testing()
add_test(ip_filter_test ip_filter_test)

install(TARGETS ip_filter RUNTIME DESTINATION bin)
