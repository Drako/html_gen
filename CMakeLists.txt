cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0054 NEW)
project(html_gen VERSION 0.1.0.0)

set(SOURCE_FILES main.cxx string.hxx list.hxx algorithm.hxx)
add_executable(html_gen ${SOURCE_FILES})
set_target_properties(html_gen
    PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
)
