cmake_minimum_required(VERSION 3.21)
project(search_engine)

set(CMAKE_CXX_STANDARD 14)

set(JSON_BuildTests OFF CACHE INTERNAL "")

include(FetchContent)
FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG v3.10.5
)

FetchContent_MakeAvailable(json)

add_subdirectory(tests)

add_executable(search_engine main.cpp Headers/InvertedIndex.h Source/InvertedIndex.cpp Headers/ConvertJSON.h Source/ConvertJSON.cpp Headers/SearchServer.h Source/SearchServer.cpp Tests/test_searchServer.cpp Tests/test_Invertedindex.cpp)

target_link_libraries(search_engine PRIVATE gtest_main nlohmann_json)

gtest_discover_tests(search_engine)

