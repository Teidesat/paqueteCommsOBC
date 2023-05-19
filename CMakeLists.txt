cmake_minimum_required(VERSION 3.10.0)
project(paqueteCnC VERSION 0.1.0)
add_library(paqueteCnCLibrary
  src/packet.cpp
  src/packet_buffer_io.cpp
  src/packet_builder.cpp
  src/packet_builder_director.cpp
  src/packet_extended/packet_extended_basic.cpp
  src/packet_extended/packet_extended_verification_1.cpp
  src/packet_extended/packet_extended_housekeeping_2.cpp
  src/packet_extended/packet_extended_housekeeping_25.cpp
  src/packet_extended/packet_extended_housekeeping_basic.cpp
  src/packet_extended/packet_extended_large_data_1.cpp
  src/packet_extended/packet_extended_large_data_4.cpp
  src/packet_extended/packet_extended_large_data_5.cpp)

add_executable(paqueteCnC src/main.cpp)
target_link_libraries(paqueteCnC paqueteCnCLibrary)
target_compile_features(paqueteCnC PRIVATE cxx_std_20)
target_compile_features(paqueteCnCLibrary PRIVATE cxx_std_20)
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    add_compile_options(-Wall -pedantic -Wextra)
endif(CMAKE_COMPILER_IS_GNUCXX)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  tests_paqueteCnC
  test/packet_buffer_io_test.cpp
)
target_link_libraries(
  tests_paqueteCnC
  paqueteCnCLibrary
  GTest::gtest_main
)
target_compile_features(tests_paqueteCnC PRIVATE cxx_std_20)
include(GoogleTest)
gtest_discover_tests(tests_paqueteCnC)
